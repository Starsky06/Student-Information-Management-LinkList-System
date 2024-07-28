#ifndef Node_type
#define Node_type

#include	"Student.h"

using type = Student;

struct Node{
	type item;
	Node *next;
	Node(type);
};


#endif