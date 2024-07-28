#ifndef Exam_type
#define Exam_type

#include <iostream>
#include <cstring>
#include <iomanip>
#include "Subject.h"


using namespace std;

struct Exam {		
	int trimester; //1-jan, 5-may and 10-oct
	int year;
	double gpa;
	int numOfSubjects; //for one trimester
	Subject sub[6]; //list of subjects taken by student in a trimester

	Exam();
	bool calculateGPA();
	void print(ostream &);
	const char *printTrimester();
};



#endif