#pragma once
#include<string>
#include<iostream>

class route
{
private:
	std::string r_dest;
	int r_port;
	int r_cost;


public:
	route();
	route(std::string dest, int port, int cost);
	void printRoute();
	std::string getDest();
	int getCost();
	int getPort();
	void setCost(int cost);
	void setPort(int port);
	void setDest(std::string dest);
	~route();
};
