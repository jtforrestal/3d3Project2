Basic Idea
----------

The **DVT** (_Distance Vector Table_) is a linked list of **DVTEntries**.
Each **DVTEntry** contains values for the name of the destination _dvt_dest_ , the port number to take to reach that destination _dvt_port_ and the link cost to the destination _dvt_cost_.

Methods
=======


All Methods
-----------

###DVT

#####Private:

_DVTEntry* getNewNode(string dest, int port, int cost)_

Returns  a pointer to a new DVTEntry with those values.
Used by other methods to build the table.

#####Public:

_void insertAtFront(string dest, int port, int cost)_

Creates new DVTEntry adds it to the top of the table.

_void insertAtBack(std::string dest, int port, int cost)_

Creates new DVTEntry adds it to the bottom of the table.

_bool removeFromFront()_

Removes DVTEntry from top of list
Returns false if list is empty and true if successful

_bool removeFromBack()_

Removes DVTEntry from bottom of list
Returns false if list is empty and true if successful

_bool isEmpty()_

Returns true if the list is isEmpty

_bool getByIndex(int n, string& dest, int& port, int& cost)_

Loads the values from the nth slot of the table into dest, port and cost.
Returns false if list is empty or n is not found.

_bool searchDest(string term, string& dest, int& port, int& cost)_

Searches the list for an entry with destination matching term
Loads the value from the table into dest, port ,cost.
Returns false if list is empty or term not found

_bool setPortforDest( string dest, int port)_

Searches for a DVTEntry by dest, sets that entry's port number to port.

_bool setPortforDest( string dest, int port, int&cost)_

Overloaded version of previous method whick also returns the link cost of the entry to cost.

_bool setCostforDest( string dest, int cost)_

Searches for a DVTEntry by dest, sets that entry's cost number to cost.

_bool setCostforDest( string dest, int& port, int cost)_

Sames as above but also returns the port number to port.

_bool getPtrforDest(  DVTEntry** pointer, string dest)_

Takes a derefferenced pointer as a parameter.
Searches the table for an entry matching dest
Returns a pointer to that entry
Pointer can be used to directly set values whilst within the DVT

**usage:**  In main:
DVTEntry* PtrHolder; // Declare pointer to entry
forwardingTable.getPtrforDest(&PtrHolder,"E"); //Set pointer to point to E
PtrHolder->setPort(999); // May now set E's values directly


_void print()_

Prints the values in the table to the command line


###DVTEntry

Getters:

_std::string getDest()_
_int getPort()_
_int getCost()_


Setters:

_void setDest(std::string dest)_
_void setPort(int port)_
_void setCost(int cost)_


_void printDVTEntry()_

Prints the values in the entry
