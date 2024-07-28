#include <iostream>
#include "Node.h"


using type = Student;

Node::Node(type newItem)
{
	item = newItem;
	next = NULL;
}