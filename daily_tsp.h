#include "types.h"
#include <iterator>

graph_t preprocess(graph_t G, distance_t M, predecessor_matrix_t & P)
{
	// replace all distances larger than M with infinity
	for (auto & row : G)
		for (auto & distance : row) {
			if (distance > M)
				distance = std::numeric_limits<distance_t>::max();
		}
		
	// compute shortest paths
	auto V = G.size();
	for (city_t k = 0; k < V; k++)
		for (city_t i = 0; i < V; i++)
			for (city_t j = 0; j < V; j++) 
			{
				if (G[i][k] + G[k][j] < G[i][j]) {
					G[i][j] = G[i][k] + G[k][j];
					P[i][j] = P[k][j];
				}
			}
	return G;
}

route_t reconstruct(const graph_t & G, route_t route, const predecessor_matrix_t & P)
{
	route_t new_route;
	for (auto i=std::next(route.begin()); i != route.end(); i++)
	{
		auto city_a = *(std::prev(i));
		auto city_b = *i;

		while (city_a != city_b) {
			new_route.push_back(city_a);
			city_a = P[city_a][city_b];
		}
	}
	return new_route;
}

template<class Solver, class Divider>
days_t solve_tsp_daily(graph_t G, distance_t M, Solver tsp_solver, Divider divider)
{
	predecessor_matrix_t P;
	graph_t Gprime = preprocess(G, M, P);
	route_t Rprime = tsp_solver(G);
	route_t R = reconstruct(G, Rprime, P);
	days_t D = divider(R, M);
	return D;
}