#ifndef DVTENTRY_H
#define DVTENTRY_H
class DVT;

#include<string>
#include<iostream>



class DVTEntry
{
	friend class DVT;

public:
	DVTEntry();
	DVTEntry(std::string dest, int port, int cost);
	// can also return routes from
	std::string getDest();
	int getPort();
	int getCost();

	void setDest(std::string dest);
	void setPort(int port);
	void setCost(int cost);

	void printDVTEntry();
	~DVTEntry();
private:
	std::string dvt_dest;
	int dvt_port;
	int dvt_cost;
	DVTEntry* nextPtr;
};

#endif
