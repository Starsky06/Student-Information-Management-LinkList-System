#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <cctype>
#include "List.h"
#include "Student.h"

using namespace std;

bool CreateStuList(const char* filename, List* list);
bool DeleteStudent(List* list, const char* id);
bool PrintList(List& list, int outputMethod);
bool InsertExamResult(const char* filename, List* list);
bool PrintStatistic(List& list);
bool FilterStudent(List& source, List* destination, const char* course, int year, int totalCredit);
bool UpdateIDandPhone(List* list);
bool FindPotentialFirstClass(List& source, List* destination, const char* course);
int menu();

template <size_t N>
void copyText(char (&destination)[N], const string& source) {
    strncpy(destination, source.c_str(), N - 1);
    destination[N - 1] = '\0';
}

string trim(const string& value) {
    size_t first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

string valueAfterEquals(const string& line) {
    size_t position = line.find('=');
    return position == string::npos ? "" : trim(line.substr(position + 1));
}

Node* findStudentNode(List* list, const char* id) {
    for (Node* current = list->head; current != NULL; current = current->next) {
        if (strcmp(current->item.id, id) == 0) return current;
    }
    return NULL;
}

int enrollmentYear(const char* id) {
    size_t length = strlen(id);
    for (size_t i = 0; i + 1 < length; ++i) {
        if (isdigit(static_cast<unsigned char>(id[i])) && isdigit(static_cast<unsigned char>(id[i + 1]))) {
            return 2000 + (id[i] - '0') * 10 + (id[i + 1] - '0');
        }
    }
    return 0;
}

int semesterCredits(const Exam& exam) {
    int credits = 0;
    for (int i = 0; i < exam.numOfSubjects; ++i) {
        credits += exam.sub[i].credit_hours;
    }
    return credits;
}

bool CreateStuList(const char* filename, List* list) {
    ifstream input(filename);
    if (!input.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return false;
    }

    int inserted = 0;
    string idLine, nameLine, courseLine, phoneLine;

    while (getline(input, idLine)) {
        if (trim(idLine).empty()) continue;

        if (!getline(input, nameLine) || !getline(input, courseLine) || !getline(input, phoneLine)) {
            cout << "Invalid student record format.\n";
            return false;
        }

        Student student;
        string id = valueAfterEquals(idLine);
        string name = valueAfterEquals(nameLine);
        string course = valueAfterEquals(courseLine);
        string phone = valueAfterEquals(phoneLine);

        if (id.empty() || name.empty() || course.empty() || phone.empty()) {
            cout << "Skipped an incomplete student record.\n";
            continue;
        }

        if (id.length() >= sizeof(student.id) || name.length() >= sizeof(student.name) ||
            course.length() >= sizeof(student.course) || phone.length() >= sizeof(student.phone_no)) {
            cout << "Skipped " << id << ": one or more fields exceed the allowed size.\n";
            continue;
        }

        copyText(student.id, id);
        copyText(student.name, name);
        copyText(student.course, course);
        copyText(student.phone_no, phone);

        if (findStudentNode(list, student.id) != NULL) {
            cout << student.id << " already exists; skipped.\n";
            continue;
        }

        list->insert(student);
        ++inserted;
    }

    cout << inserted << " student record(s) added.\n";
    return inserted > 0;
}

bool DeleteStudent(List* list, const char* id) {
    if (list == NULL || list->empty()) {
        cout << "The student list is empty.\n";
        return false;
    }

    Node* previous = NULL;
    Node* current = list->head;

    while (current != NULL) {
        if (strcmp(current->item.id, id) == 0) {
            if (previous == NULL) list->head = current->next;
            else previous->next = current->next;

            delete current;
            --list->count;
            cout << "Student deleted successfully.\n";
            return true;
        }
        previous = current;
        current = current->next;
    }

    cout << "Student ID not found.\n";
    return false;
}

bool PrintList(List& list, int outputMethod) {
    if (list.empty()) {
        cout << "The student list is empty.\n";
        return false;
    }

    ostream* output = &cout;
    ofstream file;

    if (outputMethod == 2) {
        file.open("student_result.txt");
        if (!file.is_open()) {
            cout << "Unable to create student_result.txt.\n";
            return false;
        }
        output = &file;
    } else if (outputMethod != 1) {
        cout << "Invalid print method. Enter 1 for screen or 2 for file.\n";
        return false;
    }

    int number = 1;
    for (Node* current = list.head; current != NULL; current = current->next, ++number) {
        Student& student = current->item;
        *output << "\n****************************************************** STUDENT " << number
                << " ******************************************************\n";
        student.print(*output);
        *output << "\n------------------------------- PAST EXAM RESULTS -------------------------------\n";

        if (student.exam_cnt == 0) {
            *output << "This student has not taken any exam yet.\n";
        } else {
            for (int i = 0; i < student.exam_cnt; ++i) {
                student.exam[i].print(*output);
            }
        }
    }

    if (outputMethod == 2) cout << "Student list saved to student_result.txt.\n";
    return true;
}

bool InsertExamResult(const char* filename, List* list) {
    if (list == NULL || list->empty()) {
        cout << "Create the student list before importing exam results.\n";
        return false;
    }

    ifstream input(filename);
    if (!input.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return false;
    }

    int inserted = 0;
    char id[12];
    int trimester, year, subjectCount;

    while (input >> id >> trimester >> year >> subjectCount) {
        Exam exam;
        exam.trimester = trimester;
        exam.year = year;
        exam.numOfSubjects = subjectCount;

        bool validRecord = subjectCount > 0 && subjectCount <= 6;

        for (int i = 0; i < subjectCount; ++i) {
            Subject subject;
            if (!(input >> subject.subject_code >> subject.subject_name >> subject.credit_hours >> subject.marks)) {
                cout << "Invalid exam record format.\n";
                return false;
            }

            for (int j = 0; subject.subject_name[j] != '\0'; ++j) {
                if (subject.subject_name[j] == '_') subject.subject_name[j] = ' ';
            }

            if (i < 6) exam.sub[i] = subject;
        }

        if (!validRecord) {
            cout << "Skipped exam record for " << id << ": a trimester supports 1 to 6 subjects.\n";
            continue;
        }

        Node* studentNode = findStudentNode(list, id);
        if (studentNode == NULL) {
            cout << "Skipped exam record: student " << id << " was not found.\n";
            continue;
        }

        Student& student = studentNode->item;
        if (student.exam_cnt >= 10) {
            cout << "Skipped " << id << ": maximum of 10 trimester records reached.\n";
            continue;
        }

        bool duplicateSemester = false;
        for (int i = 0; i < student.exam_cnt; ++i) {
            if (student.exam[i].trimester == trimester && student.exam[i].year == year) {
                duplicateSemester = true;
                break;
            }
        }

        if (duplicateSemester) {
            cout << "Skipped duplicate exam record for " << id << ".\n";
            continue;
        }

        student.exam[student.exam_cnt] = exam;
        student.exam[student.exam_cnt].calculateGPA();
        ++student.exam_cnt;
        ++inserted;
    }

    for (Node* current = list->head; current != NULL; current = current->next) {
        if (current->item.exam_cnt > 0) current->item.calculateCurrentCGPA();
    }

    cout << inserted << " exam record(s) inserted.\n";
    return inserted > 0;
}

bool PrintStatistic(List& list) {
    if (list.empty()) {
        cout << "The student list is empty.\n";
        return false;
    }

    int cs = 0, ia = 0, ib = 0, cn = 0, ct = 0;
    int studentsWithExams = 0, totalSemesters = 0, totalSubjects = 0, totalCredits = 0;
    double totalCGPA = 0.0;

    for (Node* current = list.head; current != NULL; current = current->next) {
        const Student& student = current->item;

        if (strcmp(student.course, "CS") == 0) ++cs;
        else if (strcmp(student.course, "IA") == 0) ++ia;
        else if (strcmp(student.course, "IB") == 0) ++ib;
        else if (strcmp(student.course, "CN") == 0) ++cn;
        else if (strcmp(student.course, "CT") == 0) ++ct;

        if (student.exam_cnt > 0) {
            ++studentsWithExams;
            totalCGPA += student.current_cgpa;
            totalSemesters += student.exam_cnt;

            for (int i = 0; i < student.exam_cnt; ++i) {
                totalSubjects += student.exam[i].numOfSubjects;
                totalCredits += semesterCredits(student.exam[i]);
            }
        }
    }

    cout << fixed << setprecision(2);
    cout << "Total Students: " << list.size() << '\n';
    cout << "  CS Students - " << cs << '\n';
    cout << "  IA Students - " << ia << '\n';
    cout << "  IB Students - " << ib << '\n';
    cout << "  CN Students - " << cn << '\n';
    cout << "  CT Students - " << ct << '\n';

    if (studentsWithExams == 0) {
        cout << "No exam results are available for statistics.\n";
        return true;
    }

    cout << "Average CGPA: " << totalCGPA / studentsWithExams << '\n';
    cout << "Average Subjects Taken Per Semester: " << static_cast<double>(totalSubjects) / totalSemesters << '\n';
    cout << "Average Credits Earned Per Semester: " << static_cast<double>(totalCredits) / totalSemesters << '\n';
    return true;
}

bool FilterStudent(List& source, List* destination, const char* course, int year, int totalCredit) {
    if (source.empty()) {
        cout << "Create the student list before filtering.\n";
        return false;
    }
    if (!destination->empty()) {
        cout << "The destination list must be empty.\n";
        return false;
    }

    for (Node* current = source.head; current != NULL; current = current->next) {
        const Student& student = current->item;
        if (strcmp(course, student.course) == 0 && enrollmentYear(student.id) == year &&
            student.totalCreditsEarned >= totalCredit) {
            destination->insert(destination->size() + 1, student);
        }
    }

    return true;
}

bool UpdateIDandPhone(List* list) {
    if (list == NULL || list->empty()) {
        cout << "The student list is empty.\n";
        return false;
    }

    int updated = 0;
    for (Node* current = list->head; current != NULL; current = current->next) {
        Student& student = current->item;

        if (student.id[0] != 'B') {
            string course = student.course;
            string newId = "B" + course + student.id;
            if (newId.length() < sizeof(student.id)) {
                copyText(student.id, newId);
                ++updated;
            } else {
                cout << "Skipped ID update for " << student.name << ": ID would exceed the allowed size.\n";
            }
        }

        if (student.phone_no[0] != '0') {
            string digits;
            for (size_t i = 0; student.phone_no[i] != '\0'; ++i) {
                if (isdigit(static_cast<unsigned char>(student.phone_no[i]))) digits += student.phone_no[i];
            }

            if (!digits.empty()) {
                string prefix = ((digits[0] - '0') % 2 == 0) ? "02" : "01";
                string newPhone = prefix + digits;

                if (newPhone.length() < sizeof(student.phone_no)) {
                    copyText(student.phone_no, newPhone);
                    ++updated;
                } else {
                    cout << "Skipped phone update for " << student.name << ": number would exceed the allowed size.\n";
                }
            }
        }
    }

    cout << updated << " field(s) updated.\n";
    return true;
}

bool FindPotentialFirstClass(List& source, List* destination, const char* course) {
    if (source.empty()) {
        cout << "The student list is empty.\n";
        return false;
    }
    if (!destination->empty()) {
        cout << "The destination list must be empty.\n";
        return false;
    }

    for (Node* current = source.head; current != NULL; current = current->next) {
        const Student& student = current->item;
        if (strcmp(student.course, course) != 0 || student.exam_cnt < 3) continue;

        int lowGPA = 0;
        int strongSemester = 0;

        for (int i = 0; i < student.exam_cnt; ++i) {
            const Exam& exam = student.exam[i];
            if (exam.gpa < 3.5) ++lowGPA;
            if (exam.gpa >= 3.75 && semesterCredits(exam) >= 12) ++strongSemester;
        }

        if (lowGPA == 0 && strongSemester >= 3) {
            destination->insert(destination->size() + 1, student);
        }
    }

    return true;
}

int menu() {
    int choice;
    cout << "\n1. Create student list\n";
    cout << "2. Delete student\n";
    cout << "3. Print student list\n";
    cout << "4. Insert exam result\n";
    cout << "5. Print exam statistics\n";
    cout << "6. Filter students\n";
    cout << "7. Update student ID and phone\n";
    cout << "8. Find potential first-class students\n";
    cout << "9. Exit\n";
    cout << "Enter your choice (1-9): ";
    cin >> choice;
    return choice;
}

int main() {
    List studentList;
    const char* studentFilename = "../Textfile/student.txt";
    const char* examFilename = "../Textfile/exam.txt";
    int selection;

    do {
        selection = menu();

        switch (selection) {
        case 1:
            CreateStuList(studentFilename, &studentList);
            break;

        case 2: {
            char id[12];
            cout << "Enter the student ID to delete: ";
            cin >> id;
            DeleteStudent(&studentList, id);
            break;
        }

        case 3: {
            int outputMethod;
            cout << "Enter 1 to print on screen or 2 to save to file: ";
            cin >> outputMethod;
            PrintList(studentList, outputMethod);
            break;
        }

        case 4:
            InsertExamResult(examFilename, &studentList);
            break;

        case 5:
            PrintStatistic(studentList);
            break;

        case 6: {
            char course[3];
            int year, totalCredit;
            cout << "Enter course code (CS, IA, IB, CN, or CT): ";
            cin >> course;
            cout << "Enter enrollment year: ";
            cin >> year;
            cout << "Enter minimum total credit hours: ";
            cin >> totalCredit;

            List filteredStudents;
            if (FilterStudent(studentList, &filteredStudents, course, year, totalCredit)) {
                if (filteredStudents.empty()) cout << "No students match the selected criteria.\n";
                else PrintList(filteredStudents, 1);
            }
            break;
        }

        case 7:
            UpdateIDandPhone(&studentList);
            break;

        case 8: {
            char course[3];
            cout << "Enter course code (CS, IA, IB, CN, or CT): ";
            cin >> course;

            List potentialStudents;
            if (FindPotentialFirstClass(studentList, &potentialStudents, course)) {
                if (potentialStudents.empty()) cout << "No potential first-class students found.\n";
                else PrintList(potentialStudents, 1);
            }
            break;
        }

        case 9:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice. Please enter a number from 1 to 9.\n";
            break;
        }
    } while (selection != 9);

    return 0;
}
