#ifndef List_type
#define List_type

#include	"Node.h"
#include	"Student.h"


struct List
{
	int count;
	Node *head;
	Node *find(int);
	List();
	bool empty();
	int size();
	bool get(int, type&);
	bool set(int, type);
	bool insert(int, type);  // insert item according to the specified position
	bool remove(int);
	bool insert(type); // insert item in ascending order


};



#endif