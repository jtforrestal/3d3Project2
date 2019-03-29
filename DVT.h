/*Derived from Linked Lists Implmentation in C++ How To Program Tenth Ed. published by Peasron */
#ifndef DVT_H
#define DVT_H

#include <iostream>
#include <string>
#include "DVTEntry.h"




class DVT{
private:
	DVTEntry* firstPtr;
	DVTEntry* lastPtr;
	DVTEntry* getNewNode(std::string dest, int port, int cost) {
	  return new DVTEntry{dest, port, cost};
	}
public:
	DVT();
		//Destructor for the route list
	~DVT();

	// insert a route object at the front of the list
	void insertAtFront(std::string dest, int port, int cost);
	// insert a route object at the back
	void insertAtBack(std::string dest, int port, int cost);
	// load the route object into value and the nremove it from the list
	bool removeFromFront();
	// load the route object into the value and remove from the list
	bool removeFromBack();
	// check if the lis tis empty
	bool isEmpty()const	;
	// load entry number entry into value and return true on success
	// count from 0
	bool getByIndex(int& index, std::string& dest, int& port, int& cost)const ;
	// search based on destination name, takes string and route, returns false if not found
	// if found returns true and fills value with the whole route
	// All things going to plan each table should have only one of each destination
	bool searchDest(std::string term, std::string& dest, int& port, int& cost);

	//Set destination to port set a new port
	bool setPortforDest( std::string dest, int port);
	// Overloaded function to return whole route if needed
	bool setPortforDest(  std::string dest, int port, int&cost);

	//Set destination to port set a new port
	bool setCostforDest( std::string dest, int cost);
	// Overloaded function to return whole route if needed
	bool setCostforDest( std::string dest, int& port, int cost);
	//############
	//######This method sets the pointer value to point to the entry object in the list
	//######We may use this to access the entry directly
	bool getPtrforDest(  DVTEntry** pointer, std::string dest);
	// prints all objects in a List
	void print()const;




};

#endif
