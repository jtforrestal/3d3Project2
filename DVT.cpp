#include "DVT.h"
#include "DVTEntry.h"

DVT::DVT(){
  firstPtr =nullptr;
  lastPtr = nullptr;
}

// insert a DVTEntry object at the front of the list
void DVT::insertAtFront(std::string dest, int port, int cost) {
  DVTEntry* newPtr{ getNewNode(dest, port,cost) };

  if (isEmpty()) {
    firstPtr = lastPtr = newPtr;

  }
  else {
    lastPtr->nextPtr = newPtr;
    firstPtr = newPtr;
  }
}
// insert a route object at the back
void DVT::insertAtBack(std::string dest, int port, int cost) {
  DVTEntry* newPtr{getNewNode(dest,port,cost) };

  if (isEmpty()) {
    firstPtr = lastPtr = newPtr;

  }
  else {
    lastPtr->nextPtr = newPtr;
    lastPtr = newPtr;
  }
}
//
bool DVT::removeFromFront() {
  if (isEmpty()) {
    return false;
  }
  else {
    DVTEntry* tempPtr{ firstPtr };
    if (firstPtr == lastPtr) {
      firstPtr = lastPtr = nullptr;
    }
    else {
      firstPtr = firstPtr->nextPtr;

    }

    delete tempPtr;
    return true;
  }
}

bool DVT::removeFromBack() {
  if (isEmpty()) {
    return false;
  }
  else {
    DVTEntry* tempPtr{ lastPtr };
    if (firstPtr == lastPtr) {
      firstPtr = lastPtr = nullptr;
    }
    else {
      DVTEntry* currentPtr{ firstPtr };
      while (currentPtr->nextPtr != lastPtr) {
        currentPtr = currentPtr->nextPtr;

      }
      lastPtr = currentPtr;
      currentPtr->nextPtr = nullptr;
    }

    delete tempPtr;
    return true;
  }
}

// check if the lis tis empty
bool DVT::isEmpty() const{
  return firstPtr == nullptr;
}

bool DVT::getByIndex(int& index, std::string& dest, int& port, int& cost) const{
  int step = 0;
  if (isEmpty()) {
    //std::cout << "The table is empty \n";
    return false;
  }
  else{
    DVTEntry* currentPtr{firstPtr};
    while(currentPtr!=nullptr){
      if(step == index){
         dest = currentPtr->dvt_dest;
         port = currentPtr->dvt_port;
         cost = currentPtr->dvt_cost;
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
bool DVT::searchDest(std::string term, std::string& dest, int& port, int& cost){
  //std::cout << "search Term is " << term << "\n";
  if (isEmpty()) {
    //std::cout << "The table is empty \n";
    return false;
  }
  else{
    DVTEntry* currentPtr{firstPtr};
    while(currentPtr!=nullptr){
      //std::cout << currentPtr->entry.getDest()<< "\n";
      if( term == currentPtr->dvt_dest){
        dest = currentPtr->dvt_dest;
        port = currentPtr->dvt_port;
        cost = currentPtr->dvt_cost;
        return true;
      }
      currentPtr = currentPtr->nextPtr;
    }
    return false;
  }
}
//Set destination to port set a new port
bool DVT::setPortforDest( std::string dest, int port){

  if (isEmpty()) {
    //std::cout << "The table is empty \n";
    return false;
  }
  else{
    DVTEntry* currentPtr{firstPtr};
    while(currentPtr!=nullptr){
      //std::cout << currentPtr->entry.getDest()<< "\n";
      if( dest == currentPtr->dvt_dest){
        currentPtr->setPort(port);
        //std::cout << "Dest  " << dest  <<" set to port "<< port<< "\n";
        return true;
      }
      currentPtr = currentPtr->nextPtr;
    }
    return false;
  }
}
//Overloaded version of the function which returns the cost too
bool DVT::setPortforDest( std::string dest, int port, int&cost){

  if (isEmpty()) {
    //std::cout << "The table is empty \n";
    return false;
  }
  else{
    DVTEntry* currentPtr{firstPtr};
    while(currentPtr!=nullptr){
      //std::cout << currentPtr->entry.getDest()<< "\n";
      if( dest == currentPtr->dvt_dest){
        currentPtr->setPort(port);
      cost = currentPtr->dvt_cost;
        //std::cout << "Dest  " << dest  <<" set to port "<< port<< "\n";
        return true;
      }
      currentPtr = currentPtr->nextPtr;
    }
    return false;
  }
}
//Set destination to port set a new port
bool DVT::setCostforDest( std::string dest, int cost){

  if (isEmpty()) {
    // << "The table is empty \n";
    return false;
  }
  else{
    DVTEntry* currentPtr{firstPtr};
    while(currentPtr!=nullptr){
      ////std::cout << currentPtr->entry.getDest()<< "\n";
      if( dest == currentPtr->dvt_dest){
        currentPtr->setCost(cost);
        //std::cout << "Dest  " << dest  <<" set to cost "<< cost<< "\n";
        return true;
      }
      currentPtr = currentPtr->nextPtr;
    }
    return false;
  }
}

//Overloaded version of the function which returns the port
bool DVT::setCostforDest( std::string dest, int& port, int cost){

  if (isEmpty()) {
    // << "The table is empty \n";
    return false;
  }
  else{
    DVTEntry* currentPtr{firstPtr};
    while(currentPtr!=nullptr){
      ////std::cout << currentPtr->entry.getDest()<< "\n";
      if( dest == currentPtr->dvt_dest){
        currentPtr->setCost(cost);
        port = currentPtr->dvt_port;
        //std::cout << "Dest  " << dest  <<" set to cost "<< cost<< "\n";
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
bool DVT::getPtrforDest(  DVTEntry** pointer, std::string dest){

  if (isEmpty()) {
    //std::cout << "The table is empty \n";
    return false;
  }
  else{
    DVTEntry* currentPtr{firstPtr};
    while(currentPtr!=nullptr){
      //std::cout << currentPtr->dvt_dest<< "\n";
      if( dest == currentPtr->dvt_dest){

        *pointer = currentPtr;
        //std::cout << "Dest  " << dest  <<" set to port "<< port<< "\n";
        return true;
      }
      currentPtr = currentPtr->nextPtr;
    }
    return false;
  }
}

// prints all objects in a List
void DVT::print() const {
  if (isEmpty()) {
    std::cout << "The table is empty \n";
    return;
  }
  else {
    DVTEntry* currentPtr{ firstPtr };

    std::cout << "The Table is : \n";
    while (currentPtr != nullptr) {
      currentPtr->printDVTEntry();
      currentPtr = currentPtr->nextPtr;
    }
    std::cout << "\n\n";
  }

}

DVTEntry* getNewNode(std::string dest, int port, int cost) {
  return new DVTEntry(dest, port, cost);
}

DVT::~DVT(){
  if (!isEmpty()) {
    std::cout << "Destroying nodes \n";

    DVTEntry* currentPtr{ firstPtr };
    DVTEntry* tempPtr{ nullptr };

    while (currentPtr != nullptr) {
      tempPtr = currentPtr;
      tempPtr->printDVTEntry();
      currentPtr = currentPtr->nextPtr;
      delete tempPtr;
    }
  }
  std::cout << "All nodes Destroyed \n";
}
