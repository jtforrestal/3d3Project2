Basic Idea
----------

The **DVT** (_Distance Vector Table_) is a linked list of **DVTEntries**.
Each **DVTEntry** contains values for the name of the destination _dvt_dest_ , the port number to take to reach that destination _dvt**port_ and the link cost to the destination _dvt_cost_.

Methods
=======


All Methods
-----------

DVT
---

Private:
........

**DVTEntry* getNewNode(string dest, int port, int cost)**

Returns  a pointer to a new DVTEntry with those values.
Used by other methods to build the table.

Public:
.......

**void insertAtFront(string dest, int port, int cost)**

Creates new DVTEntry adds it to the top of the table.

**void insertAtBack(std::string dest, int port, int cost)**

Creates new DVTEntry adds it to the bottom of the table.

**bool removeFromFront()**

Removes DVTEntry from top of list
Returns false if list is empty and true if successful

**bool removeFromBack()**

Removes DVTEntry from bottom of list
Returns false if list is empty and true if successful

**bool isEmpty()**

Returns true if the list is isEmpty

**bool getByIndex(int n, string& dest, int& port, int& cost)**

Loads the values from the nth slot of the table into dest, port and cost.
Returns false if list is empty or n is not found.

**bool searchDest(string term, string& dest, int& port, int& cost)**

Searches the list for an entry with destination matching term
Loads the value from the table into dest, port ,cost.
Returns false if list is empty or term not found

**bool setPortforDest( string dest, int port)**

Searches for a DVTEntry by dest, sets that entry's port number to port.

**bool setPortforDest( string dest, int port, int&cost)**

Overloaded version of previous method whick also returns the link cost of the entry to cost.

**bool setCostforDest( string dest, int cost)**

Searches for a DVTEntry by dest, sets that entry's cost number to cost.

**bool setCostforDest( string dest, int& port, int cost)**

Same as above but also returns the port number to port.

**bool getPtrforDest(  DVTEntry** pointer, string dest)**

Takes a derefferenced pointer as a parameter.
Searches the table for an entry matching dest
Returns a pointer to that entry
Pointer can be used to directly set values whilst within the DVT

**usage:**  In main:  
DVTEntry* PtrHolder; // Declare pointer to entry  
forwardingTable.getPtrforDest(&PtrHolder,"E"); //Set pointer to point to E  
PtrHolder->setPort(999); // May now set E's values directly


**void print()**

Prints the values in the table to the command line


DVTEntry
---------

Getters:

**std::string getDest()**  
**int getPort()**  
**int getCost()**


Setters:

**void setDest(string dest)**  
**void setPort(int port)**  
**void setCost(int cost)** ,


**void printDVTEntry()**

Prints the values in the entry
