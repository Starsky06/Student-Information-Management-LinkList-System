#ifndef Subject_type
#define Subject_type

#include <iostream>
#include <cstring>
#include <iomanip>

using namespace std;

struct Subject{

	char subject_code[10];
	char subject_name[256];
	int credit_hours;
	double marks;

	Subject();
	const char *getGrade();
	double getGradePoint();
	void print(ostream &);
};




#endif