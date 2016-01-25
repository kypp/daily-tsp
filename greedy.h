#include "types.h"
#include <unordered_set>
#include <limits>

struct solve_tsp_greedy {
	route_t operator()(const graph_t & G) {
		std::unordered_set<city_t> unvisited_cities;
		for (city_t c=0; c < G.size(); c++)
			unvisited_cities.insert(c);

		city_t first_city = 0;
		city_t last_city = first_city;
		route_t route;
		route.push_back(last_city);
		unvisited_cities.erase(last_city);

		while (!unvisited_cities.empty()) {
			distance_t min_distance = std::numeric_limits<distance_t>::max();
			city_t closest_city = *unvisited_cities.begin();
			for (auto city : unvisited_cities) {
				auto current_dist = G[last_city][city];
				if (current_dist < min_distance) {
					closest_city = city;
					min_distance = current_dist;
				}
			}
			route.push_back(closest_city);
			unvisited_cities.erase(closest_city);
			last_city = closest_city;
		}

		route.push_back(first_city);
		return route;
	}
};

struct divide_route_greedy {
	days_t operator()(const graph_t & G, route_t route, distance_t M)
	{
		days_t divided;
		route_t current_day;
		distance_t current_length {0};
		
		city_t previous_city {route.front()};
		route.pop_front();
		current_day.push_back(previous_city);

		while (!route.empty()) {
			auto next_city = route.front();
			route.pop_front();
			auto next_distance = G[previous_city][next_city];
			if (next_distance + current_length > M) {
				divided.push_back(current_day);
				current_day = {previous_city, next_city};
				current_length = next_distance;				
			}
			else {
				current_day.push_back(next_city);
				current_length += next_distance;
			}
			previous_city = next_city;
		}

		if (current_day.size() > 1)
			divided.push_back(current_day);

		return divided;
	}
};

