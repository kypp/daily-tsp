#include "types.h"
#include "daily_tsp_helpers.h"
#include "greedy.h"
#include <set>

const int MAX_ITERS = 10000;

struct divide_route_tabu {
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