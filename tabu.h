#include "types.h"
#include "daily_tsp_helpers.h"
#include "greedy.h"
#include <set>


struct solve_tsp_tabu {
	const int MAX_ITERS = 10000;
	solve_tsp_tabu(int max_iters = 10000) : MAX_ITERS{max_iters}
	{

	}

	route_t operator()(const graph_t & G) {
		auto initial_solution = solve_tsp_greedy{}(G);
		
		auto cost = [&G](const routev_t & route) {
			distance_t sum{ 0 };
			for (int i = 0; i < route.size() - 1; i++)
				sum += G[route[i]][route[i + 1]];
			return sum;
		};

		routev_t best_route{ initial_solution.begin(), initial_solution.end() };
		distance_t best_cost = cost(best_route);
		std::set<routev_t> tabu_list;
		tabu_list.insert(best_route);
		
		routev_t current_route{ best_route };

		int iter = 0;
		while (iter < MAX_ITERS) {
			iter++;
			routev_t current_candidate;
			distance_t current_distance = std::numeric_limits<distance_t>::max();

			for (int i = 0; i < current_route.size() - 1; i++) {
				routev_t new_route = current_route;
				std::swap(new_route[i], new_route[i + 1]);
				if (tabu_list.count(new_route) == 1)
					continue;
				auto new_distance = cost(new_route);
				if (new_distance < current_distance) {
					current_candidate = new_route;
					current_distance = new_distance;
				}
			}

			if (current_candidate.empty())
				tabu_list.clear();
			else {
				current_route = current_candidate;
				tabu_list.insert(current_route);

				if (current_distance < best_cost) {
					best_cost = current_distance;
					best_route = current_route;
				}
			}
		}
		return{ best_route.begin(), best_route.end() };
	}
};

struct divide_route_tabu {
	const int MAX_ITERS = 10000;
	divide_route_tabu(int max_iters = 10000) : MAX_ITERS{ max_iters }
	{

	}

	days_t operator()(const graph_t & G, route_t route, distance_t M)
	{
		graph_t subdistances = create_graph(route.size());
		routev_t routev{ route.begin(), route.end() };
		for (int i = 0; i < routev.size() - 1; i++) {
			subdistances[i][i + 1] = G[routev[i]][routev[i + 1]];
		}
		for (int window = 2; window < routev.size() - 1; window++) {
			for (int i = 0; i < routev.size() - window; i++) {
				subdistances[i][i + window] = subdistances[i][i + window - 1] + subdistances[i + window - 1][i + window];
			}
		}

		auto greedy_division = divide_route_greedy{}(G, route, M);

		std::vector<city_t> sleep_indices;
		int current_index{ 0 };
		for (auto day : greedy_division) {
			sleep_indices.push_back(current_index);
			current_index += day.size()-1;
		}
		sleep_indices.push_back(route.size() - 1);

		auto validate = [M, &subdistances](const std::vector<city_t> & sleep_indices) {
			for (int i = 0; i < sleep_indices.size()-1; i++) {
				if (subdistances[sleep_indices[i]][sleep_indices[i + 1]] > M)
					return false;
			}
			return true;
		};

		auto calc_cost = [M, &subdistances](const std::vector<city_t> & sleep_indices) {
			distance_t cost{ 0 };
			for (int i = 0; i < sleep_indices.size() - 1; i++) {
				auto factor = subdistances[sleep_indices[i]][sleep_indices[i + 1]] - M;
				cost += factor*factor;
			}
			return cost;
		};

		auto best_division = sleep_indices;
		distance_t best_cost = calc_cost(best_division);

		std::set<std::vector<city_t>> tabu_list;
		tabu_list.insert(best_division);
		
		auto current_division = best_division;

		int iter = 0;
		while (iter < MAX_ITERS) {
			iter++;
			std::vector<city_t> current_candidate;
			distance_t current_cost = std::numeric_limits<distance_t>::max();
			for (int swappos = 1; swappos < current_division.size() - 1; swappos++) {
				for (auto dir : { -1, 1 }) {
					if (current_division[swappos] + dir == current_division[swappos + dir])
						continue;
					auto division = current_division;
					division[swappos] += dir;
					if (tabu_list.count(division) != 0)
						continue;
					auto division_cost = calc_cost(division);
					if (division_cost < current_cost) {
						current_candidate = division;
						current_cost = division_cost;
					}
					if (validate(division) && division_cost < best_cost) {
						best_cost = division_cost;
						best_division = division;
					}
				}
			}
			if (current_candidate.empty())
				tabu_list.clear();
			else {
				current_division = current_candidate;
				tabu_list.insert(current_division);
			}
		}

		days_t days;
		for (int i = 0; i < best_division.size() - 1; i++) {
			route_t r;
			for (int idx = best_division[i]; idx <= best_division[i + 1]; idx++) {
				r.push_back(routev[idx]);
			}
			days.push_back(r);
		}
		return days;
	}
};