#ifndef Student_type
#define Student_type

#include	<cstring>
#include	"Exam.h"
#include    "Subject.h"

struct Student {		

	char name[30];	
   	char id[12];
	char course[3];
	char phone_no[10];
	double current_cgpa; //use calculateCurrentCGPA() to update
	int totalCreditsEarned; //use calculateCurrentCGPA() to update
	Exam exam[10]; //can store up to 10 trimester exam result
	int exam_cnt; //keep track of how many trimester exam the student has taken
	
	Student();
	bool compareName1(Student); //use >= to compare name
	bool compareID(Student); //use == to compare id
	bool calculateCurrentCGPA();
	void print(ostream &);

};





#endif