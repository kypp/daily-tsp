#include "daily_tsp.h"
#include "greedy.h"
#include "GRASP.h"
#include "tabu.h"
#include <ctime>
#include <cstdio>

FILE *tests_descr;

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    tests_descr=fopen("tests.txt","r");
    int n_tests;
    fscanf(tests_descr,"%d",&n_tests);

    for(int n=0;n<n_tests;n++){
        distance_t opt_cost,day_limit;
        char filename[1<<10];
        fscanf(tests_descr,"%s %f %f\n",filename,&opt_cost,&day_limit);
        auto G = graph_from_file_2D_coords(filename);
        auto M = day_limit;
        if(G.size()==0)continue;
        std::cout<<filename<<" "<<G.size()<<" "<<opt_cost<<" ";

        //auto route = solve_tsp_daily(G, M, solve_tsp_greedy{}, divide_route_greedy{});
        //std::cout << "route:\n";
        //print_route(route);
        //auto cost = calculate_cost(route, G, M);
        //std::cout << opt_cost<<" ";
        //std::cout /*<< "route cost: "*/ << cost.first<<" ";/* <<"("<<100.*(cost.first-opt_cost)/opt_cost<<"% err)\n";
        //std::cout << "total cost: " << cost.second << "\n";*/
        {
            auto route = solve_tsp_daily(G, M, solve_tsp_greedy{}, divide_route_greedy{});
            auto cost = calculate_cost(route, G, M);
            std::cout << cost.first <<" ";
        }
        {
            auto route = solve_tsp_daily(G, M, solve_tsp_tabu{}, divide_route_greedy{});
            auto cost = calculate_cost(route, G, M);
            std::cout << cost.first <<" ";
        }
        {
            auto route = solve_tsp_daily(G, M, solve_tsp_GRASP{}, divide_route_greedy{});
            auto cost = calculate_cost(route, G, M);
            std::cout << cost.first <<" ";
        }

        std::cout <<"\n";
    }
	return 0;
}
