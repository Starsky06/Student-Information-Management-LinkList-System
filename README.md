# Student Management System

A C++ Student Management System developed for a Data Structures and Algorithm assignment. The program uses a **Linked List** to store student records and supports student management, examination results, GPA/CGPA calculation, filtering, statistics, and student classification.

## Features

The program provides the following menu options:

1. **Create Student List**
   - Reads student information from `Textfile/student.txt`
   - Stores students in a Linked List
   - Inserts students according to their names in ascending order

2. **Delete Student**
   - Deletes a student using their Student ID

3. **Print Student List**
   - Displays all student information on screen
   - Can also save the results to `student_result.txt`
   - Displays previous examination results

4. **Insert Exam Result**
   - Reads examination results from `Textfile/exam.txt`
   - Matches exam results with students using Student ID
   - Calculates GPA for each trimester
   - Updates the student's current CGPA and total credits earned

5. **Print Exam Statistics**
   - Displays total number of students
   - Shows the number of students in each course
   - Calculates average CGPA
   - Calculates average subjects taken per semester
   - Calculates average credits earned per semester

6. **Filter Students**
   - Filters students based on:
     - Course
     - Enrollment year
     - Minimum total credit hours

7. **Update Student ID and Phone**
   - Updates Student ID by adding the required course prefix
   - Converts phone numbers into the required format

8. **Find Potential First-Class Students**
   - Searches students from a selected course
   - Requires at least 3 trimester results
   - Checks GPA and semester credit requirements
   - Displays students who meet the first-class criteria

9. **Exit**
   - Closes the program

## Project Structure

```text
Assignment1/
│
├── app.cpp
├── Student.h
├── Student.cpp
├── Exam.h
├── Exam.cpp
├── Subject.h
├── Subject.cpp
├── Node.h
├── Node.cpp
├── List.h
├── List.cpp
│
└── Textfile/
    ├── student.txt
    └── exam.txt
```

## Main Data Structure

The program uses a **Singly Linked List**.

```text
Student
   ↓
Node → Node → Node → NULL
```

Each `Node` stores one `Student` object and a pointer to the next node.

### Node

```cpp
struct Node {
    type item;
    Node *next;
};
```

### List

The `List` class manages the linked list using:

- `head` → points to the first node
- `count` → stores the number of students

## Classes / Structures

### Student

Stores:

- Student name
- Student ID
- Course
- Phone number
- Current CGPA
- Total credits earned
- Examination records

A student can store up to **10 trimester exam records**.

### Exam

Stores:

- Trimester
- Year
- GPA
- Number of subjects
- Up to 6 subjects

### Subject

Stores:

- Subject code
- Subject name
- Credit hours
- Marks

It also calculates:

- Grade
- Grade Point

## GPA Calculation

The GPA is calculated using the credit-hour weighted formula:

```text
GPA =
Σ(Grade Point × Credit Hours)
------------------------------
       Total Credit Hours
```

The student's CGPA is then calculated using the GPA and total credits from all completed trimesters.

## File Input / Output

### Input

```text
Textfile/student.txt
Textfile/exam.txt
```

### Output

```text
student_result.txt
```

The program can print the student list either to the screen or to `student_result.txt`.

## Technologies

- C++
- Object-Oriented Programming
- Structures
- Singly Linked List
- File Input / Output
- GPA / CGPA Calculation

## How to Run

Compile all `.cpp` files together:

```bash
g++ app.cpp Student.cpp Exam.cpp Subject.cpp Node.cpp List.cpp -o Assignment1
```

Then run:

```bash
Assignment1
```

Make sure the following files exist in the correct location:

```text
Textfile/student.txt
Textfile/exam.txt
```

## Program Flow

```text
Start
  ↓
Create Student List
  ↓
Read student.txt
  ↓
Store Students in Linked List
  ↓
Display Menu
  ↓
User selects an option
  ↓
Perform operation
  ↓
Return to Menu
  ↓
Option 9
  ↓
Exit
```

## Author

Developed as a C++ Data Structures and Algorithm assignment.
