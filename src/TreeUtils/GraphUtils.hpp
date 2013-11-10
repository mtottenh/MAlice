#ifndef __GRAPH_UTILS__
#define __GRAPH_UTILS__
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
	
struct Vertex{ std::string node_name; std::string node_type;};

//Define the graph using those classes
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, Vertex, boost::no_property> Graph;
//Some typedefs for simplicity
typedef boost::graph_traits<Graph>::vertex_descriptor vertex_t;
typedef boost::graph_traits<Graph>::edge_descriptor edge_t;


#endif
