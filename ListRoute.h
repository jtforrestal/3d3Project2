/*Derived from Linked Lists Implmentation in C++ How To Program Tenth Ed. published by Peasron */
#ifndef LISTROUTE_H
#define LISTROUTE_H

#include <iostream>
#include"ListNodeRoute.h"
#include "route.h"


class ListRoute{
public:
		//Destructor for the route list
	~ListRoute() {
		if (!isEmpty()) {
			std::cout << "Destroying nodes \n";

			ListNodeRoute* currentPtr{ firstPtr };
			ListNodeRoute* tempPtr{ nullptr };

			while (currentPtr != nullptr) {
				tempPtr = currentPtr;
				tempPtr->entry.printRoute();
				currentPtr = currentPtr->nextPtr;
				delete tempPtr;
			}
		}
		std::cout << "All nodes Destroyed \n";
	}

	// insert a route object at the front of the list
	void insertAtFront(const route& value) {
		ListNodeRoute* newPtr{ getNewNode(value) };

		if (isEmpty()) {
			firstPtr = lastPtr = newPtr;

		}
		else {
			lastPtr->nextPtr = newPtr;
			firstPtr = newPtr;
		}
	}
	// insert a route object at the back
	void insertAtBack(const route& value) {
		ListNodeRoute* newPtr{ getNewNode(value) };

		if (isEmpty()) {
			firstPtr = lastPtr = newPtr;

		}
		else {
			lastPtr->nextPtr = newPtr;
			lastPtr = newPtr;
		}
	}
	// load the route object into value and the nremove it from the list
	bool removeFromFront(route& value) {
		if (isEmpty()) {
			return false;
		}
		else {
			ListNodeRoute* tempPtr{ firstPtr };
			if (firstPtr == lastPtr) {
				firstPtr = lastPtr = nullptr;
			}
			else {
				firstPtr = firstPtr->nextPtr;

			}
			value = tempPtr->entry;
			delete tempPtr;
			return true;
		}
	}
	// load the route object into the value and remove from the list
	bool removeFromBack(route& value) {
		if (isEmpty()) {
			return false;
		}
		else {
			ListNodeRoute* tempPtr{ lastPtr };
			if (firstPtr == lastPtr) {
				firstPtr = lastPtr = nullptr;
			}
			else {
				ListNodeRoute* currentPtr{ firstPtr };
				while (currentPtr->nextPtr != lastPtr) {
					currentPtr = currentPtr->nextPtr;

				}
				lastPtr = currentPtr;
				currentPtr->nextPtr = nullptr;
			}
			value = tempPtr->entry;
			delete tempPtr;
			return true;
		}
	}
	// check if the lis tis empty
	bool isEmpty() const{
		return firstPtr == nullptr;
	}
	// load entry number entry into value and return true on success
	// count from 0
	bool getEntry(route& value,int entry) const{
		int step = 0;
		if (isEmpty()) {
			//std::cout << "The table is empty \n";
			return false;
		}
		else{
			ListNodeRoute* currentPtr{firstPtr};
			while(currentPtr!=nullptr){
				if(step == entry){
					value = currentPtr->entry;
					return true;
				}
				currentPtr = currentPtr->nextPtr;
				step++;
			}
			return false;
		}
	}
	// search based on destination name, takes string and route, returns false if not found
	// if found returns true and fills value with the whole route
	// All things going to plan each table should have only one of each destination
	bool searchDest(route& value, std::string term){
		//std::cout << "search Term is " << term << "\n";
		if (isEmpty()) {
			//std::cout << "The table is empty \n";
			return false;
		}
		else{
			ListNodeRoute* currentPtr{firstPtr};
			while(currentPtr!=nullptr){
				//std::cout << currentPtr->entry.getDest()<< "\n";
				if( term == currentPtr->entry.getDest()){
					value = currentPtr->entry;
					return true;
				}
				currentPtr = currentPtr->nextPtr;
			}
			return false;
		}
	}

	//Set destination to port set a new port
	bool setPortforDest( std::string dest, int port){

		if (isEmpty()) {
			//std::cout << "The table is empty \n";
			return false;
		}
		else{
			ListNodeRoute* currentPtr{firstPtr};
			while(currentPtr!=nullptr){
				//std::cout << currentPtr->entry.getDest()<< "\n";
				if( dest == currentPtr->entry.getDest()){
					currentPtr->entry.setPort(port);
					//std::cout << "Dest  " << dest  <<" set to port "<< port<< "\n";
					return true;
				}
				currentPtr = currentPtr->nextPtr;
			}
			return false;
		}
	}
	// Overloaded function to return whole route if needed
	bool setPortforDest( route& value, std::string dest, int port){

		if (isEmpty()) {
			//std::cout << "The table is empty \n";
			return false;
		}
		else{
			ListNodeRoute* currentPtr{firstPtr};
			while(currentPtr!=nullptr){
				//std::cout << currentPtr->entry.getDest()<< "\n";
				if( dest == currentPtr->entry.getDest()){
					currentPtr->entry.setPort(port);
					value = currentPtr->entry;
					//std::cout << "Dest  " << dest  <<" set to port "<< port<< "\n";
					return true;
				}
				currentPtr = currentPtr->nextPtr;
			}
			return false;
		}
	}

	//Set destination to port set a new port
	bool setCostforDest( std::string dest, int cost){

		if (isEmpty()) {
			// << "The table is empty \n";
			return false;
		}
		else{
			ListNodeRoute* currentPtr{firstPtr};
			while(currentPtr!=nullptr){
				////std::cout << currentPtr->entry.getDest()<< "\n";
				if( dest == currentPtr->entry.getDest()){
					currentPtr->entry.setCost(cost);
					//std::cout << "Dest  " << dest  <<" set to cost "<< cost<< "\n";
					return true;
				}
				currentPtr = currentPtr->nextPtr;
			}
			return false;
		}
	}
	// Overloaded function to return whole route if needed
	bool setCostforDest( route& value, std::string dest, int cost){

		if (isEmpty()) {
			//std::cout << "The table is empty \n";
			return false;
		}
		else{
			ListNodeRoute* currentPtr{firstPtr};
			while(currentPtr!=nullptr){
				////std::cout << currentPtr->entry.getDest()<< "\n";
				if( dest == currentPtr->entry.getDest()){
					currentPtr->entry.setCost(cost);
					value = currentPtr->entry;
					//std::cout << "Dest  " << dest <<" set to cost "<< cost<< "\n";
					return true;
				}
				currentPtr = currentPtr->nextPtr;
			}
			return false;
		}
	}
	//############
	//######This method sets the pointer value to point to the entry object in the list
	//######We may use this to access the entry directly
	bool getPtrforDest( route** pointer, std::string dest){

		if (isEmpty()) {
			//std::cout << "The table is empty \n";
			return false;
		}
		else{
			ListNodeRoute* currentPtr{firstPtr};
			while(currentPtr!=nullptr){
				//std::cout << currentPtr->entry.getDest()<< "\n";
				if( dest == currentPtr->entry.getDest()){

					*pointer = &currentPtr->entry;
					//std::cout << "Dest  " << dest  <<" set to port "<< port<< "\n";
					return true;
				}
				currentPtr = currentPtr->nextPtr;
			}
			return false;
		}
	}
	// prints all objects in a List
	void print() const {
		if (isEmpty()) {
			std::cout << "The table is empty \n";
			return;
		}
		else {
			ListNodeRoute* currentPtr{ firstPtr };

			std::cout << "The Table is : \n";
			while (currentPtr != nullptr) {
				currentPtr->entry.printRoute();
				currentPtr = currentPtr->nextPtr;
			}
			std::cout << "\n\n";
		}

	}



	private:
		ListNodeRoute* firstPtr{ nullptr };
		ListNodeRoute* lastPtr{ nullptr };

		ListNodeRoute* getNewNode(const route value) {
			return new ListNodeRoute{ value };
		}
};

#endif
