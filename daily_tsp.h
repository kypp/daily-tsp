#include "types.h"

graph_t preprocess(graph_t G, distance_t M)
{

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