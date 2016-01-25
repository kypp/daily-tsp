#include "daily_tsp.h"
#include "greedy.h"

int main(int argc, char const *argv[])
{
	if (argc < 2) {
		std::cout << "usage: greedy_tsp graph_description\n";
		return 0;
	}

	auto g_d = graph_from_file(argv[1]);
	auto G = g_d.first;
	auto M = g_d.second;

	auto route = solve_tsp_daily(G, M, solve_tsp_greedy{}, divide_route_greedy{});

	std::cout << "route:\n";
	print_route(route);
	auto cost = calculate_cost(route, G, M);
	std::cout << "route cost: " << cost.first << "\n";
	std::cout << "total cost: " << cost.second << "\n";

	return 0;
}