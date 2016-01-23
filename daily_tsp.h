#include "types.h"

graph_t preprocess(graph_t G, distance_t M)
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
				if (G[i][k] + G[k][j] < G[i][j])
					G[i][j] = G[i][k] + G[k][j];
			}
	return G;
}

route_t reconstruct(graph_t G, route_t R)
{
	
}

template<class Solver, class Divider>
days_t solve_tsp_daily(graph_t G, distance_t M, Solver tsp_solver, Divider divider)
{
	graph_t Gprime = preprocess(G, M);
	route_t Rprime = tsp_solver(G);
	route_t R = reconstruct(G, Rprime);
	days_t D = divider(R, M);
	return D;
}