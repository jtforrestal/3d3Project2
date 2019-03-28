#ifndef LISTNODE_H
#define LISTNODE_H
#include "route.h"
class ListRoute;


class ListNodeRoute
{
	friend class ListRoute;

public:
	explicit ListNodeRoute(const route& info)
		: entry{ info }, nextPtr{ nullptr } {}
	// can also return routes from 	
	route getEntry() const { return entry; }

private:
	route entry;
	ListNodeRoute* nextPtr;

};

#endif
