#include <list>
#include <vector>

using city_t = int;
using route_t = std::list<city_t>;
using routev_t = std::vector<city_t>;
using distance_t = float;
using graph_t = std::vector<std::vector<distance_t>>;
using days_t = std::vector<route_t>;
using predecessor_matrix_t = std::vector<std::vector<city_t>>;
