#include	<iostream>
#include 	<fstream>
#include	<string>
#include	<cstdlib>
#include	"List.h"
#include	"Student.h"



bool CreateStuList(const char *, List *);
bool DeleteStudent(List *, char *);
bool PrintList(List, int); 
bool InsertExamResult(const char *, List *);
bool PrintStatistic(List);
bool FilterStudent(List, List *, char *, int, int) ;
bool UpdateIDandPhone(List *);
bool FindPotentialFirstClass(List, List *, char *);
int  menu();


using namespace std;


bool CreateStuList(const char* filename, List* list) {

    ifstream inFile(filename);
    Student* stu = new Student();
    Student* check = new Student();
    string line;
    bool duplicate = true;
    //check the file can successfully open or not?
    if (!inFile.is_open()) {
        cout << "Error opening the file !!!" << filename << endl;
        return false;
    }
    // read file from student.txt
    while (!inFile.eof()) {
        //read the word in file until need
        inFile >> line >> line >> line >> stu->id;
        inFile >> line >> line;
        inFile.ignore();
        inFile.getline(stu->name, 30);
        inFile >> line >> line >> stu->course;
        inFile >> line >> line >> line >> stu->phone_no;

        //find duplicate or not?
        for (int i = 1; i <= list->size(); i++) {
            list->get(i, *check);
            if (check->compareID(*stu)) {
                duplicate = false;
                cout << stu->id << " already exists!!!" << endl;
                break;
            }
        }

        //check the length of student id 
        if (strlen(stu->id) == 0) {
            break;
        }

        if (duplicate) {
            list->insert(*stu);
        }

        //check list empty or not?
        if (list->empty()) {
            cout << "The file is still empty now!!!" << endl;
            return false;
        }
    }

    inFile.close();
    delete stu;
    delete check;
    return true;
}

bool DeleteStudent(List* list, char* id) {

    Node* cur_location = list->head;
   
    int position = 1;
    //check the node is empty or not ?
    if (cur_location == NULL) {
        cout << "This list is NULL not any value exists" << endl;
        return false;
    }
    //find the id and remove it
    while (cur_location != NULL) {
        if (strcmp(cur_location -> item.id, id) == 0) {
            list->remove(position);
            return true;
        }
        position++;
        //point to next node
        cur_location = cur_location -> next;
        
        
    }

   
    return false;
}

bool PrintList(List list, int n) {

    Student* stu = new Student();
    //check list is empty or not?
    if (list.empty()) {
        cout << "The list is still in empty now !!! " << endl;
        return false;
    }
    else {
        //select 1 perform print screen
        if (n == 1) {
            //print to screen
            for (int i = 1; i <= list.size(); i++) {
                cout << "******************************************************STUDENT " << i << "*********************************************************" << endl;
                list.get(i, *stu);  
                stu->print(cout);

                cout << "--------------------------------------------------PAST EXAM RESULT:-----------------------------------------------------" << endl;;
                //print exam result to screen
                if (stu->exam_cnt != 0) {
                    for (int j = 0; j < stu->exam_cnt; j++) {
                        stu->exam[j].print(cout);
                    }
                }
                else {
                    cout << "This student hasn't taken any exam yet !!!" << endl;;
                }
                cout << "******************************************************STUDENT " << i << "*********************************************************" << endl;;
            }
            delete stu;
            return true;
        }
        else if (n == 2) {
            //select 2 perform print file

            ofstream outFile("student_result.txt");

            if (!outFile.is_open()) {
                cout << "Error to opening  the file " << endl;
                return false;
            }
            //print to file
            for (int i = 1; i <= list.size(); i++) {
                outFile << "******************************************************STUDENT " << i << "*******************************************************" << endl;
                list.get(i, *stu);  
                stu->print(outFile);


                outFile << "--------------------------------------------------PAST EXAM RESULT:---------------------------------------------------" << endl;
                //print exam result to file
                if (stu->exam_cnt != 0) {
                    for (int j = 0; j < stu->exam_cnt; j++) {
                        stu->exam[j].print(outFile);
                    }
                }
                else {
                    outFile << "This student hasn't taken any exam yet !!!" << endl;;
                }
                outFile << "******************************************************STUDENT " << i << "*********************************************************" << endl;
            }
            cout << "Student list has been successfully saved to the file !!! " << endl;
            delete stu;
            
        }
    }
    return true;
}

bool InsertExamResult(const char* filename, List* list) {
    ifstream inFile(filename);
    char id[10];

    if (!list->empty() && inFile.is_open()) {
        while (!inFile.eof()) {
            Exam exam;
            Subject subject;
            Student stu;
            // read file from exam.txt 
            inFile >> id;

            Node* cur = list->head;
            while (cur != NULL) {
                if (strcmp(cur->item.id, id) == 0) {
                    exam.numOfSubjects = 0;
                    inFile >> exam.trimester 
                           >> exam.year 
                           >> exam.numOfSubjects;

                    for (int j = 0; j < exam.numOfSubjects; j++) {
                        inFile >> subject.subject_code 
                               >> subject.subject_name;

                        for (int k = 0; subject.subject_name[k] != '\0'; k++) {
                            //check subject name and replace  symbols
                            if (subject.subject_name[k] == '_') {
                                subject.subject_name[k] = ' ';
                            }
                        }
                        inFile >> subject.credit_hours 
                               >> subject.marks;

                        exam.sub[j] = subject;
                    }
                    //add the exam to the student's exam array 
                    exam = cur->item.exam[cur->item.exam_cnt++];
                    //calculate the GPA of them
                    cur->item.exam[cur->item.exam_cnt - 1].gpa = exam.calculateGPA();
                    break;
                }
                cur = cur->next;
            }
        }

        Node* cur = list->head;
        //calculate cgpa
        while (cur != NULL) {
            if (cur->item.exam_cnt > 0) {
                cur->item.calculateCurrentCGPA();
            }
            cur = cur->next;
        }

        inFile.close();
        return true;
    }

    return false;
}

bool PrintStatistic(List list)
{
    if (list.empty()) {
        cout << "Error to the list as is empty, please try again !!!" << endl;
        return false;
    }
    else {
        
        Node* cur = list.head;  
        int totalStudents = list.size();
        
        
        int csStudents = 0, iaStudents = 0, ibStudents = 0, cnStudents = 0, ctStudents = 0;
        double totalCGPA = 0.0, totalSubjects = 0.0, avgSubjectsTaken = 0, totalCredits = 0.0, avgCreditsEarned = 0.0, current_cgpa = 0.0;
        string course; // Course enrolled in by the student
        

        if (cur != NULL)
            const Student& student = cur->item; // replace with function to get course
        //calculate how mant student inside
        if (course == "CS") {
            csStudents++;
        }
        else if (course == "IA") {
            iaStudents++;
        }
        else if (course == "IB") {
            ibStudents++;
        }
        else if (course == "CN") {
            cnStudents++;
        }
        else if (course == "CT") {
            ctStudents++;
        }
        else {

        }


        totalCGPA += cur->item.current_cgpa;
        totalSubjects = totalSubjects + (cur->item.exam->numOfSubjects / cur->item.exam_cnt);
        totalSubjects = totalSubjects + (cur->item.totalCreditsEarned / cur->item.exam_cnt);
        cur = cur->next;

        // Calculating averages
        double averageCGPA = totalCGPA / totalStudents;
        double averageSubjects = static_cast<double> (totalSubjects) / totalStudents;
        double averageCredits = static_cast<double> (totalCredits) / totalStudents;

        //print statistics
        cout << "Total Students: " << totalStudents << endl;
        cout << "  CS Students - " << csStudents << endl;
        cout << "  IA Students - " << iaStudents << endl;
        cout << "  IB Students - " << ibStudents << endl;
        cout << "  CN Students - " << cnStudents << endl;
        cout << "  CT Students - " << ctStudents << endl;
        cout << "Average CGPA: " << fixed << setprecision(5) << averageCGPA << endl;
        cout << "  Average Subjects Taken Per Semester: " << fixed << setprecision(2) << averageSubjects << endl;
        cout << "  Average Credits Earned Per Semester: " << fixed << setprecision(2) << averageCredits << endl;

        return true;
    }
}

bool FilterStudent(List list1, List* list2, char* course, int year, int totalcredit)
{
    Node* current = list1.head;
    int yearEnrolled = 0;
    int count = 0;

    // Check list1 is empty ?
    if (list1.empty()) {
        cout << "The student list is empty now !!! Please create the list first ,TQ !!!" << endl;
        return false;
    }

    // Check list2 is not empty ? Make sure is empty
    if (!list2->empty()) {
        cout << "Error about the List2 is not empty" << endl;
        return false;
    }
    else {

        for (int i = 1; i <= list1.size(); i++) {
            int yearEnrolled = 0;
            //check the item.id
            if (current->item.id[0] >= '0' && current->item.id[0] <= '9' && current->item.id[1] >= '0' && current->item.id[1] <= '9') {
                yearEnrolled = 2000 + ((current->item.id[0] - '0') * 10) + (current->item.id[1] - '0');
            }
            else {
                yearEnrolled = 2000 + ((current->item.id[3] - '0') * 10) + (current->item.id[4] - '0');
            }

            // check if the current student meets the filtering criteria
            if (strcmp(course, current->item.course) == 0 && year == yearEnrolled && current->item.totalCreditsEarned >= totalcredit) {
                list2->insert(count, current->item); //insert that student into list2       
                count++;
            }
            current = current->next;
        }

        return true;
    }
}

bool UpdateIDandPhone(List* list) {
    //check the list empty or not ?
    if (list == NULL || list->empty())
        return false;

    else {
        Node* cur;
        string courseAndID;
        cur = list->head;
        
        for (int i = 1; i <= list->size(); i++) {
            // declaration of an array named courseTemp
            // 12 represent the max. length of course code
            // 4 is represent the min.length of course code
            // 1 use for null terminator (cool things)
            char courseTemp[12 + 4 + 1];
            //add the B in front of course name after check
            if (cur->item.id[0] != 'B') {
                if (strcmp(cur->item.course, "CS") == 0) {
                    strcpy(courseTemp, "BCS");
                }

                if (strcmp(cur->item.course, "IA") == 0) {
                    strcpy(courseTemp, "BIA");
                }

                if (strcmp(cur->item.course, "IB") == 0) {
                    strcpy(courseTemp, "BIB");
                }

                if (strcmp(cur->item.course, "CN") == 0) {
                    strcpy(courseTemp, "BCN");
                }

                if (strcmp(cur->item.course, "CT") == 0) {
                    strcpy(courseTemp, "BCT");
                }

                strcat(courseTemp, cur->item.id);
                strcpy(cur->item.id, courseTemp);
            }

            // Update phone number 
            char phone_no[10 + 3 + 1];
            if (cur->item.phone_no[0] != '0') {
                // check the first character of phoneNo is even or not ?
                if ((cur->item.phone_no[0] % 2) == 0) {
                    strcpy(phone_no, "02");
                }
                else {
                    strcpy(phone_no, "01");
                }

                strcat(phone_no, cur->item.phone_no);
                // terminate the new string with a null character
                for (int j = 0; phone_no[j] != '\0'; j++) {
                    if (phone_no[j] == '-') {
                        for (int k = j; phone_no[k] != '\0'; k++) {
                            phone_no[k] = phone_no[k + 1];
                        }
                        break;
                    }
                }
                strcpy(cur->item.phone_no, phone_no);
            }
            cur = cur->next;
        }
        return true;
    }
}

bool FindPotentialFirstClass(List list1, List* list2, char* course) {
    Node* cur = list1.head;
    //check is empty or not?
    if (list1.empty()) {
        cout << "There is no student in " << course << " that has potential to get first class." << endl;
        return false;
    }
    // check if list 2 is not empty
    if (!list2->empty()) {	// for safety warning
        cout << "The list is not empty!" << endl << endl;
        return false;
    }
    else {

        Student* stu = new Student();
        int stuNo = 0;
        int cnt1, cnt2, credit;
        // check if the student is in the specified course or not?
        for (int i = 1; i <= list1.size(); ++i) {
            list1.get(i, *stu);  // stu
            if (strcmp(stu->course, course) == 0) {
                cnt1 = 0;	// to count number of trimester GPA < 3.5
                cnt2 = 0;	// to count number of trimester GPA >= 3.75

                // only test for students who taken at least 3 exams
                if (stu->exam_cnt >= 3) {

                    // test every semester exam
                    for (int j = 0; j < stu->exam_cnt; ++j) {

                        // calculate total credit hours per semester
                        credit = 0;
                        for (int k = 0; k < stu->exam[j].numOfSubjects; ++k)
                            credit += stu->exam[j].sub[k].credit_hours;

                        // test gpa each semester exam
                        if (stu->exam[j].gpa < 3.5)
                            ++cnt1;
                        else if (stu->exam[j].gpa >= 3.75 && credit >= 12)
                            ++cnt2;
                    }

                    if (cnt1 > 0)
                        continue;
                    else if (cnt2 >= 3) {
                        stuNo++;
                        list2->insert(stuNo, *stu);		// Update the first class student into list2 for printing
                    }
                }
            }
        }
        cur = cur->next;
        delete stu;
        return true;
    }
}

int menu() {

    int choice;

        cout << "1. Create student list" << endl;
        cout << "2. Delete Student" << endl;
        cout << "3. Print student list" << endl;
        cout << "4. Insert exam result" << endl;
        cout << "5. Print Exam Static" << endl;
        cout << "6. Filter Student" << endl;
        cout << "7. Update Student's ID and Phone" << endl;
        cout << "8. Find Potential First Class Student" << endl;
        cout << "9. Exit" << endl;
        cout << "------------------------------------------------------------------------------------------------------------------------" << endl;
        cout << "Kindly enter your choice ( 1-9 ) : ";
        cin >> choice;
        
        return choice;
    
}

int main() {

    List studentList;
    const char* filename = "student.txt";
    const char* examFilename = "exam.txt";

    int select;
    string selection;
    do {
        
        select = menu();
        system("cls");
        
        switch (select) {

        case 1:
        {
            bool CreateStuList(const char* filename, List * list);

            if (!CreateStuList(filename, &studentList)) {
                cout << "Error to create student list !!!" << endl;
                return 1;
            }
            cout << "Created Successfully !!!" << endl;
            break;
        }
        case 2:
        {
            char idToDelete[12];
            cout << "Kindly enter the student ID of the student you want to delete: ";
            cin >> idToDelete;

            if (!DeleteStudent(&studentList, idToDelete)) {
                cout << "Error to delete student list !!!" << endl;
                return 1;
            }
            break;
        }
        case 3:
        {
            int n;
            cout << "Kindly enter the method you want to print the list: ";
            cin >> n;

            if (!PrintList(studentList, n)) {
                cout << "Error to print the list !!!" << endl;
                return 1;
            }
            break;
        }
        case 4:
        {
            if (!InsertExamResult(const_cast<char*>(examFilename), &studentList)) {
                cout << "Error to insert the exam results !!!" << endl;
                return 1;
            }
            cout << "Inserted Successfully !!!" << endl;
            break;
        }
        case 5:
        {

            if (!PrintStatistic(studentList)) {
                cout << "Error to print exam statistics !!!" << endl;
                return 1;
            }
            break;
        }
            
        case 6:
        {
            {
                char filterCourse[3];
                int filterYear, filterTotalCredit;
                cout << "Enter the course code (e.g., CS, IA, IB, CN, CT): ";
                cin >> filterCourse;
                cout << "Enter the enrollment year: ";
                cin >> filterYear;
                cout << "Enter the total credit hours earned: ";
                cin >> filterTotalCredit;

                List filteredStudents;
                if (!FilterStudent(studentList, &filteredStudents, filterCourse, filterYear, filterTotalCredit)) {
                    cout << "Error filtering students!" << endl;
                    return 1;
                }

                cout << "Filtered students:" << endl;
                if (!PrintList(filteredStudents, 1)) {
                    cout << "Error printing filtered students!" << endl;
                    return 1;
                }

                break;
            }
        }

        case 7:
        {
            if (!UpdateIDandPhone(&studentList)) {
                cout << "Error to update student ID and phones !!! " << endl;
                return 1;
            }
            cout << "Student ID and phones updated successfully !!!" << endl;

            break;
        }

        case 8:
        {
            char firstClassCourse[3];
            cout << "Kindly enter course code (e.g., CS, IA, IB, CN, CT): ";
            cin >> firstClassCourse;

            List potentialFirstClassStudents;
            if (!FindPotentialFirstClass(studentList, &potentialFirstClassStudents, firstClassCourse)) {
                cout << "Error to find potential first class students !!!" << endl;
                return 1;
            }

            cout << "Potential first class students in " << firstClassCourse << ":" << endl;
            if (!PrintList(potentialFirstClassStudents, 1)) {
                cout << "Error to print potential first class students !!!" << endl;
                return 1;
            }

            break;
        }
        case 9:
        {
            cout << "Exiting..................................................................................." << endl;
            return 0;
            break;
        }
        default:
        {
            cout << "Invalid choice. Please try again !!!" << endl;
            break;
        }
        }

    


        cout << "Do you want continue to operate ? (Y/N)" << endl;
        cin >> selection;

    } while (selection == "y" || selection == "Y");


        
    system("pause");
    return 0;
}



