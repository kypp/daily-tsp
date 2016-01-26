#include "types.h"
#include <unordered_set>
#include <queue>
#include <limits>
#include <algorithm>
#include <random>

#define REPEAT_NO   5

struct solve_tsp_GRASP {
    //UWAGA: zakładam, że G[a][b]=G[b][a]
	route_t operator()(const graph_t & G) {
	    routev_t best_route;
	    distance_t best_distance=std::numeric_limits<distance_t>::max();
        for(int k=0;k<REPEAT_NO;k++){
            //std::cout<<"dupa\n";
            std::unordered_set<city_t> unvisited_cities;
            for (city_t c=0; c < G.size(); c++)
                unvisited_cities.insert(c);

            const int candidate_list_range=std::max(2,(int)(G.size()*0.05));
            std::priority_queue<std::pair<distance_t,city_t>> candidate_list;

            city_t first_city=rand()%G.size();
            city_t last_city=first_city;
            routev_t routev;
            routev.push_back(last_city);
            unvisited_cities.erase(last_city);

            distance_t current_distance=0;
            while (!unvisited_cities.empty()) {
                distance_t min_distance = std::numeric_limits<distance_t>::max();
                for (auto city : unvisited_cities) {
                    candidate_list.push(std::pair<distance_t,city_t>{-G[last_city][city],city});
                }
                int clsm1=candidate_list.size()-1;
                int selected_city_index=rand()%(std::min(clsm1,candidate_list_range)+1);
                while(selected_city_index--){
                    //std::cout<<candidate_list.top().first<<" ";
                    candidate_list.pop();
                }
                //std::cout<<std::endl;
                city_t closest_city=candidate_list.top().second;
                current_distance+=-candidate_list.top().first;
                routev.push_back(closest_city);
                unvisited_cities.erase(closest_city);
                last_city = closest_city;
                while(candidate_list.size())candidate_list.pop();
            }
            current_distance+=G[routev.back()][first_city];
            routev.push_back(first_city);
            //for(int i=0;i<routev.size();i++)std::cout<<routev[i]<<" ";
            //std::cout<<"\n";
            //2-opt optimization:

            int while_flag=1;
            while(while_flag){
                while_flag=0;
                for(city_t p1=0;p1<routev.size()-1;p1++){
                    for(city_t p2=p1+1;p2<routev.size()-1;p2++){
                        city_t c1a=routev[p1],c1b=routev[p1+1];
                        city_t c2a=routev[p2],c2b=routev[p2+1];
                        distance_t dist_diff=G[c1a][c1b]+G[c2a][c2b]-(G[c1a][c2a]+G[c1b][c2b]);
                        if(dist_diff>0){
                            std::reverse(routev.begin()+p1+1,routev.begin()+p2+1);
                            current_distance-=dist_diff;
                            while_flag=1;
                        }
                    }
                }
            }

            //std::cout<<"found path with length "<<current_distance<<"\n";
            //for(int i=0;i<routev.size();i++)std::cout<<routev[i]<<" ";
            //std::cout<<"\n";
            if(best_distance>current_distance){
                std::swap(best_distance,current_distance);
                std::swap(best_route,routev);
            }
        }
		route_t route;
		for(int i=0;i<best_route.size();i++)route.push_back(best_route[i]);
		return route;
	}
};


