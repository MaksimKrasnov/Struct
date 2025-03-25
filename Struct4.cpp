#include <iostream>
#include <cstring>
#include<fstream>
using namespace std;

const int SIZE = 5;
struct Adress
{
    char street[100];
    char city[50];
    char postalCode[20];

};

struct Grade
{
    char subject[50];
    int grade;

};

struct Student
{
    char firstName[50];
    char lastname[50];
    Adress address;
    Grade grades[SIZE];

};

struct StudentList {
    Student* students;
    int capasity;
    int size = 0;


};

void Input_Students(Student& student, const char* firstName, const char* lastname, const char* street, const char* city,
    const char* postalCode, int grade[], const char* subject[]) {

    strncpy_s(student.firstName, firstName, sizeof(student.firstName) - 1);
    strncpy_s(student.lastname, lastname, sizeof(student.lastname) - 1);
    strncpy_s(student.address.city, city, sizeof(student.address.city) - 1);
    strncpy_s(student.address.street, street, sizeof(student.address.street) - 1);
    strncpy_s(student.address.postalCode, postalCode, sizeof(student.address.postalCode) - 1);
    for (size_t i = 0; i < SIZE; i++)
    {
        student.grades[i].grade = grade[i];
        strncpy_s(student.grades[i].subject, subject[i], sizeof(student.grades[i].subject) - 1);
    }
    
   

}
void printStudent(Student student) {
    cout << "Student: " << student.firstName << " " << student.lastname<<endl
        << "Address: " << student.address.city << " " << student.address.street
        << " " << student.address.postalCode<< endl;
    for (int i = 0; i < SIZE; i++)
    {
        cout << "Subject: " << student.grades[i].subject << " Grade: "
            << student.grades[i].grade << endl;
    }
    cout << endl;
}
void addStudent(StudentList& studentList, Student student)
{
    if (studentList.size == studentList.capasity)
    {
        studentList.capasity *= 2;
        Student* newStudentList = new Student[studentList.capasity];
        for (int i = 0; i < studentList.size; i++)
        {
            newStudentList[i] = studentList.students[i];
        }
        delete[] studentList.students;
        studentList.students = newStudentList;
    }
    studentList.students[studentList.size++] = student;

}
void printList(StudentList studentList)
{
    for (int i = 0; i < studentList.size; i++)
    {
        printStudent(studentList.students[i]);
    }
}
void removeStudent(StudentList& studentList, const char* firstName, const char* lastName)
{
    int indexForRemove = -1;
    for (int i = 0; i < studentList.size; i++)
    {
        if (strcmp(studentList.students[i].firstName, firstName) == 0 &&
            strcmp(studentList.students[i].lastname, lastName) == 0)
        {
            indexForRemove = i;
        }
    }
    if (indexForRemove == -1)
        cout << "Такого студента нет." << endl;
    else
    {
        for (int i = indexForRemove; i < studentList.size-1; i++)
        {
            studentList.students[i] = studentList.students[i + 1];
        }
        studentList.size--;
        cout << "Студент " << firstName << " " << lastName << " удален из списка." << endl;
    }
}
double calculateAverageGrade(Student student)
{
    int averageGrade = 0;
    for (int i = 0; i < SIZE; i++)
    {
        averageGrade += student.grades[i].grade;
    }
    return (double)averageGrade / SIZE;
}
void findTopStudent(StudentList studentList)
{
    double maxAverageGrade = -1;
    Student* topStudent = nullptr;
    for (int i = 0; i < studentList.size; i++)
    {
        double averageGrade = calculateAverageGrade(studentList.students[i]);
        if (averageGrade > maxAverageGrade)
        {
            maxAverageGrade = averageGrade;
            topStudent = &studentList.students[i];
        }
    }
    cout << "Студент с наибольшим средним баллом:" << endl;
    printStudent(*topStudent);
}

void saveStudentsToTextFile(StudentList studentList, const char* filename) {
    ofstream file(filename);
    if (!file.is_open()) {
     
        cout << "Не удалось открыть файл";
        return;
    }
    file << studentList.size << endl;
    for (int i = 0; i < studentList.size; i++)
    {
        file << studentList.students[i].firstName << " "
            << studentList.students[i].lastname << endl
            << studentList.students[i].address.city << endl
            << studentList.students[i].address.street << endl
            << studentList.students[i].address.postalCode << endl;
        for (int j = 0; j < SIZE; j++)
        {
            file << studentList.students[i].grades[j].subject << " "
                << studentList.students[i].grades[j].grade << endl;
        }
    }
    file.close();

}
void appendStudentToTextFile(Student student, const char* filename) {
    fstream file(filename, ios::in | ios::out);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл";
        return;
    }
    int size;
    file >> size;

    size++;
    file.seekp(0, ios::beg);
    file << size << endl;

    file.seekp(0, ios::end);

    file << student.firstName << " "
        << student.lastname << endl
        << student.address.city << endl
        << student.address.street << endl
        << student.address.postalCode << endl;
    for (int i = 0; i < SIZE; i++)
    {
        file << student.grades[i].subject << " "
            << student.grades[i].grade << endl;
    }
    file.close();
}

void LoadStudentsFromFile(StudentList& studentList, const char* filename){
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Не удалось открыть файл";
        return;
    }
    file >> studentList.size;
    studentList.capasity = studentList.size > 0 ? studentList.size * 2 : 2;
    studentList.students = new Student[studentList.capasity];
    for (int i = 0; i < studentList.size; i++)
    {
        file >> studentList.students[i].firstName
            >> studentList.students[i].lastname;
        file.ignore();
        file.getline(studentList.students[i].address.city, 50);
        file.getline(studentList.students[i].address.street, 100);
        file.getline(studentList.students[i].address.postalCode, 20);

        for (int j = 0; j < SIZE; j++)
        {
            file >> studentList.students[i].grades[j].subject
                >> studentList.students[i].grades[j].grade;
        }  
    }
    file.close();

}
int main()
{
    setlocale(LC_ALL, "ru");
    const char* subjects[] = { "Mathematic","English","History", "Fysik","C++" };
    int grades[] = { 3,4,2,5,4 };
    Student student1;
    Input_Students(student1, "Ivan", "Ivanov", "Street", "City", "6351", grades, subjects);
    //printStudent(student1);
    StudentList studentList;
    studentList.capasity = 2;
    studentList.students = new Student[studentList.capasity];
    addStudent(studentList, student1);

    Student student2;
    Input_Students(student2, "Nikolay", "Petrov", "Street2", "City2", "63512", grades, subjects);
   // appendStudentToTextFile(student2, "students.txt");
    //  printList(studentList);
    //removeStudent(studentList, "Ivan", "Ivanov");
    //printList(studentList);
    //findTopStudent(studentList);
    //saveStudentsToTextFile(studentList, "students.txt");
    StudentList list;
    LoadStudentsFromFile(list, "students.txt");
    printList(list);
    //jhvbjlhbljhbjb
    //jvbbljbb
}
    
