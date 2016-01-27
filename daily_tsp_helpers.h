#pragma once
#include "types.h"
#include <fstream>
#include <iostream>

template<typename T>
std::vector<std::vector<T>> create_vector_2d(std::size_t size, T defval=T{})
{
	return std::vector<std::vector<T>>(
		size, std::vector<T>(size, defval));	
}

graph_t create_graph(int num_cities)
{
	return create_vector_2d<distance_t>(num_cities);
}

std::pair<graph_t, distance_t> graph_from_file(std::string filename)
{
	std::ifstream file{filename};
	int cities_count{};
	file >> cities_count;
	distance_t daily_distance{};
	file >> daily_distance;

	auto G = create_graph(cities_count);
	for (int y=0; y < cities_count; y++)
	for (int x=0; x < cities_count; x++) {
		distance_t dist{};
		file >> dist;
		G[x][y] = dist;
	}

	return {G,daily_distance};
}

std::pair<distance_t,distance_t> calculate_cost(const days_t & days_route, const graph_t & G, const distance_t M) 
{
	distance_t total_cost{0};
	distance_t total_cost_days{0};

	for (const auto & day : days_route) {
		distance_t day_cost{0};
		city_t prev_city = day.front();
		for (auto city : day) {
			day_cost += G[prev_city][city];
			prev_city = city;
		}
		total_cost_days += (day_cost - M)*(day_cost - M);
		total_cost += day_cost;
	}

	return {total_cost, total_cost_days};
}

void print_route(days_t days_route)
{
	for (const auto & day : days_route) {
		for (auto city : day)
			std::cout << city << " ";
		std::cout << "\n";
	}
}