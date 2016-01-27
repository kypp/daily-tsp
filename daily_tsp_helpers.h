#pragma once
#include "types.h"
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstring>

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

int EUC_2D_dist(point2D p1,point2D p2){
    //according to 3.1 @ ftp://ftp.cse.msu.edu/pub/GA/programs/Genocop-doc/TSP/READ_ME
    double xd = p1.first-p2.first;
    double yd = p1.second-p2.second;
    return (int)( sqrt( xd*xd + yd*yd) + 0.5 );
}

graph_t graph_from_file_2D_coords(std::string filename)
{
    FILE *f=fopen(("test/"+filename).c_str(),"r");

    int cities_count=0;
    auto G = create_graph(cities_count);

    char descr[1<<10];
    while(1){
        fscanf(f,"%s",descr);
        if(strncmp(descr,"NODE_COORD_SECTION",10)==0)break;

        do{
            char c=getc(f);
            if(c==':'||c==' ')continue;
            ungetc(c,f);
            break;
        }while(1);

        //std::cout<<"dupa:"<<descr<<"\n";
        if(strncmp(descr,"EDGE_WEIGHT_TYPE",10)==0){
            fscanf(f," %s",descr);
            if(strcmp(descr,"EUC_2D")!=0){
                std::cout<<"DUPA: "<<filename<<"\n";
                return G;
            }
        }else if(strncmp(descr,"DIMENSION",8)==0){
            fscanf(f,"%d",&cities_count);
            //std::cout<<"dim found\n";
        }
        while(getc(f)!='\n');
    }
    //std::cout<<"OK: "<<cities_count<<"\n";

    G = create_graph(cities_count);
	std::vector<point2D> points;
	for(int i=0;i<cities_count;i++){
        double px,py;
        fscanf(f,"%*d %lf%lf",&px,&py);
        points.push_back({px,py});
	}
    fclose(f);

    for (int y=0; y < cities_count; y++)
	for (int x=0; x < cities_count; x++) {
		G[x][y] = EUC_2D_dist(points[x],points[y]);
	}
    return G;
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
