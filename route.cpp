
#include "route.h"




route::route(std::string dest, int port, int cost) {
	r_dest = dest;
	r_port = port;
	r_cost = cost;
}
route::route() {
	r_dest = "dst";
	r_port = 0;
	r_cost = 999;
}

std::string route::getDest(){
	return r_dest;
}

int route::getPort(){
	return r_port;
}
int route::getCost(){
	return r_cost;
}


void route::printRoute() {
	std::cout<< r_dest << " " << r_port << " "<< r_cost << "\n";
}

void route::setCost(int cost){
	r_cost = cost;
}
void route::setPort(int port){
	r_port = port;
}
void route::setDest(std::string dest){
	r_dest = dest;
}

route::~route()
{
}
