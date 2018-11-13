#pragma comment(lib, "libmySQL.lib")
#pragma comment(lib, "ws2_32.lib")
#include <my_global.h>
#include "mysql.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

// Question 1.
int login(MYSQL *);

// Question 2.
void current_courses(MYSQL *, int);

// Question 3.
void transcript(MYSQL *, int);
void all_courses(MYSQL *, int);
void course_details(MYSQL *, int, string, string, string);

// Question 4.
void enroll(MYSQL *, int);
void possible_enroll_courses(MYSQL *, int);
bool check_quarter(MYSQL *, string, string, int);
bool check_maximum_preferred(MYSQL *, string, string, int);
bool check_prerequisite(MYSQL *, int, string);
bool check_maximum(MYSQL *, string, string, int);
bool check_prev_course(MYSQL *, int, string);
void course_add(MYSQL *, int, string, string, int);

// Question 5.
void withdraw(MYSQL *, int);
void possible_withdraw_courses(MYSQL *, int);
bool check_course_withdraw(MYSQL *, int, string, string, int);
void course_withdraw(MYSQL *, int, string, string, int);

// Question 6.
void personal_details(MYSQL *, int);
void change_details(MYSQL *, int, int, string);

int maximum = 0;
string classtype = "";

int main(int argc, char **argv)
{
	MYSQL *conn = NULL;
	MYSQL_RES *result = NULL;
	int id(0);
	char menu = '0';

	conn = mysql_init(NULL);
	if(!mysql_real_connect(
		conn, "localhost", "root", "tkdrin4730",
		NULL, 3306, NULL, CLIENT_MULTI_RESULTS))
	{
		cout << mysql_error(conn);
		exit(1);
	}
	mysql_select_db(conn, "project3-nudb");

	while(id == 0)
	{
		cout << "==================== LOGIN ====================" << endl;
		id = login(conn);
		while(id != 0)
		{
			cout << "\n<<<<<<<<<< Current Courses >>>>>>>>>>" << endl;
			current_courses(conn, id);
			cout << "==================== STUDENT MENU ====================" << endl;
			cout << "1. Transcript" << endl;
			cout << "2. Enroll" << endl;
			cout << "3. Withdraw" << endl;
			cout << "4. Personal Details" << endl;
			cout << "5. Logout" << endl;
			cout << "Select Menu : ";
			cin >> menu;
			if(!isdigit(menu))
			{
				fflush(stdin);
				continue;
			}

			switch(menu)
			{
			case '1':
				transcript(conn, id);
				break;
			case '2':
				enroll(conn, id);
				break;
			case '3':
				withdraw(conn, id);
				break;
			case '4':
				personal_details(conn, id);
				break;
			default:
				break;
			}
			if(menu == '5')
			{
				id = 0;
			    fflush(stdin);
				cout << endl;
				break;
			}
		}
	}
	mysql_close(conn);
}

int login(MYSQL *conn)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string username, password;
	string query;

	cout << "Enter your username : ";
	getline(cin, username);
	cout << "Enter your password : ";
	getline(cin, password);
	
	query = "select * from student where name = \"" + username + "\" and password = \"" + password + "\";";
	
	if(mysql_query(conn, query.c_str()))
	{
		cout << mysql_error(conn);
		exit(1);
	}

	result = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(result);
	if(numrows == 0)
	{
		cout << "Please enter your name and password again!" << endl << endl;
		return 0;
	}
	row = mysql_fetch_row(result);
	
	if(row[4] != NULL)
		maximum = atoi(row[4]);
	if(row[5] != NULL)
		classtype = row[5];
	if(username == row[1] && password == row[2])
		return atoi(row[0]);
	else 
	{
		cout << "Please enter your name and password again!" << endl << endl;
		return 0;
	}
	mysql_free_result(result);
}

void current_courses(MYSQL *conn, int id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;
	time_t timer = time(NULL);
	struct tm *t = localtime(&timer);
	int year = t->tm_year + 1900;
	int month = t->tm_mon + 1;
	int day = t->tm_mday;
	string semester;
	
	if(month >= 9 && month <= 12)
		semester = "Q1";
	else if(month >= 1 && month <= 3)
		semester = "Q2";
	else if(month >= 4 && month <= 6)
		semester = "Q3";
	else
		semester = "Q4";

	query = "select * from transcript a, unitofstudy b where a.uoscode = b.uoscode and a.studid = " + to_string(id) + " and a.semester = \"" + semester
		+ "\" and a.year = " + to_string(year) + " and a.grade is null";

	if(mysql_query(conn, query.c_str()))
	{
		cout << mysql_error(conn);
		exit(1);
	}

	result = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(result);
	if(numrows == 0)
		cout << "No courses are taken now!" << endl;

	for(int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(result);
		if(row != NULL)
			cout << to_string(i+1) + ". Course Number = " + row[1] + ", Course Name = " + row[7] + ", Quarter = " + row[2] + ", Year = " + row[3] << endl;
	}
	mysql_free_result(result);
}

void transcript(MYSQL *conn, int id)
{
	string course_num;
	string quarter;
	string year;

	while(true)
	{
		cout << endl << "<<<<<<<<<< All Courses & Grades >>>>>>>>>>" << endl;
		all_courses(conn, id);

		cout << "==================== TRANSCRIPT MENU ====================" << endl;
		cout << "Enter Course Number(Enter 0 to STUDENT MENU) : ";
		cin >> course_num;
		
		if(course_num == "0")
			break;
		cout << "Enter Quarter to see : ";
		cin >> quarter;
		cout << "Enter Year to see : ";
		cin >> year;
		
		course_details(conn, id, course_num, quarter, year);
	}
}

void all_courses(MYSQL *conn, int id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;

	query = "select * from transcript where studid = " + to_string(id);

	if(mysql_query(conn, query.c_str()))
	{
		cout << mysql_error(conn);
		exit(1);
	}

	result = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(result);
	if(numrows == 0)
	{
		cout << "No courses is registered now!" << endl;
	}

	for(int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(result);
		if(row != NULL)
		{
			if(row[4] != NULL)
				cout << to_string(i+1) + ". Course Number = " + row[1] + ", Quarter = " + row[2] + ", Year = " + row[3] + ", Grade = " + row[4] << endl;
			else
				cout << to_string(i+1) + ". Course Number = " + row[1] + ", Quarter = " + row[2] + ", Year = " + row[3] + ", Grade = Not Applicable" << endl;
		}
	}
	mysql_free_result(result);
}

void course_details(MYSQL *conn, int id, string course_num, string quarter, string year)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;

	query = "select a.uoscode, b.uosname, a.semester, a.year, c.enrollment, c.maxenrollment, d.name, a.grade from transcript a, unitofstudy b, uosoffering c, faculty d where a.uoscode = b.uoscode and a.uoscode = c.uoscode and a.semester = c.semester and a.year = c.year and c.instructorid = d.id and a.studid = " 
		+ to_string(id) + " and a.uoscode = \"" + course_num + "\" and a.semester = \"" + quarter + "\" and a.year = " + year;

	if(mysql_query(conn, query.c_str()))
	{
		cout << "Please enter correct information!" << endl;
		return;
	}

	result = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(result);
	if(numrows == 0)
	{
		cout << "No information, Please try again!" << endl;
		return;
	}

	cout << endl << "<<<<<<<<<< Course Details >>>>>>>>>>" << endl;
	for(int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(result);
		if(row != NULL)
		{
			string season;
			if(strcmp(row[2],"Q1") == 0)
				season = "Fall";
			else if(strcmp(row[2],"Q2") == 0)
				season = "Winter";
			else if(strcmp(row[2],"Q3") == 0)
				season = "Spring";
			else
				season = "Summer";

			cout << to_string(i+1) + ". Course Number : " + row[0] << endl;
			cout << to_string(i+2) + ". Course Title : " + row[1] << endl;
			cout << to_string(i+3) + ". Quarter : " + row[2] << endl;
			cout << to_string(i+4) + ". Year : " + row[3] << endl;
			cout << to_string(i+5) + ". Season : " + season << endl;
			cout << to_string(i+6) + ". Number of Enrolled Students : " + row[4] << endl;
			cout << to_string(i+7) + ". Maximum Enrollment : " + row[5] << endl;
			cout << to_string(i+8) + ". Lecturer : " + row[6] << endl;
			if(row[7] != NULL)
				cout << to_string(i+9) + ". Grade : " + row[7] << endl;
			else
				cout << to_string(i+9) + ". Grade : Not Applicable" << endl;
		}
	}
	mysql_free_result(result);
}

void enroll(MYSQL *conn, int id)
{
	string course_num;
	string quarter;
	int year(0);
	cout << "\n<<<<<<<<<< Select Courses To Enroll >>>>>>>>>>" << endl;
	possible_enroll_courses(conn, id);
	while(true)
	{
		cout << "==================== ENROLL MENU ====================" << endl;
		cout << "Enter Course Number to Enroll(Enter 0 to STUDENT MENU) : ";
		cin >> course_num;
		if(course_num == "0")
			break;
		cout << "Enter Quarter to Enroll : ";
		cin >> quarter;
		cout << "Enter Year to Enroll : ";
		cin >> year;

		if(check_quarter(conn, course_num, quarter, year)
			&& check_maximum_preferred(conn, course_num, quarter, year)
			&& check_prerequisite(conn, id, course_num)
			&& check_maximum(conn, course_num, quarter, year)
			&& check_prev_course(conn, id, course_num))
		{
			course_add(conn, id, course_num, quarter, year);
			cout << "Course Number " + course_num + " was added!" << endl;
		}
		fflush(stdin);
	}
}

void possible_enroll_courses(MYSQL *conn, int in_id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;
	time_t timer = time(NULL);
	struct tm *t = localtime(&timer);
	int year = t->tm_year + 1900;
	int month = t->tm_mon + 1;
	int day = t->tm_mday;
	string semester, next_semester;
	int next_year;

	if(month >= 9 && month <= 12)
	{
		semester = "Q1";
		next_semester = "Q2";
		next_year = year + 1;
	}
	else if(month >= 1 && month <= 3)
	{
		semester = "Q2";
		next_semester = "Q3";
		next_year = year;
	}
	else if(month >= 4 && month <= 6)
	{
		semester = "Q3";
		next_semester = "Q4";
		next_year = year;
	}
	else
	{
		semester = "Q4";
		next_semester = "Q1";
		next_year = year;
	}
	
	query = "call possible_enroll_courses (" + to_string(in_id)
		+ ", " + to_string(year) + ", \"" + semester
		+ "\", " + to_string(next_year) + ", \"" + next_semester + "\");";
	
	/*
	query = "select * from uosoffering a, lecture b, classroom c where a.uoscode = b.uoscode and a.semester = b.semester and a.year = b.year and b.classroomid = c.classroomid and a.maxenrollment > (select MaximumPreferredEnrollment from student where id = " + to_string(id) + ") and c.type != (select NonPreferredClassroomType from student where id = " + to_string(id) + ") and"
		+ "((a.semester = \"" + semester + "\" and a.year = " + to_string(year) + ") or (a.semester = \"" + next_semester + "\" and a.year = " + to_string(next_year) + "))";
		*/

	if(mysql_query(conn, query.c_str()))
	{
		cout << mysql_error(conn);
		exit(1);
	}

	result = mysql_store_result(conn);
	int numrows = (int)mysql_num_rows(result);
	if(numrows == 0)
	{
		cout << "There's no class now!" << endl;
	}

	for(int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(result);
		if(row != NULL)
		{
			//cout << to_string(i+1) + ". Course Number : " + row[0] + ", Quarter = " + row[1] + ", year = " + row[2] + ", MaxEnrollment = " + row[5] + ", Type = " + row[14] << endl;
			cout << to_string(i+1) + ". Course Number : " + row[0] + ", Quarter = " + row[1] + ", year = " + row[2] + ", MaxEnrollment = " + row[5] << endl;
		}
	}
		
	while(mysql_more_results(conn))
		mysql_next_result(conn);
	mysql_free_result(result);
}

bool check_quarter(MYSQL *conn, string course, string in_semester, int in_year)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;
	time_t timer = time(NULL);
	struct tm *t = localtime(&timer);
	bool satisfied = false;
	int year = t->tm_year + 1900;
	int month = t->tm_mon + 1;
	int day = t->tm_mday;
	string semester, next_semester;
	int next_year;
	
	if(month >= 9 && month <= 12)
	{
		semester = "Q1";
		next_semester = "Q2";
		next_year = year + 1;
	}
	else if(month >= 1 && month <= 3)
	{
		semester = "Q2";
		next_semester = "Q3";
		next_year = year;
	}
	else if(month >= 4 && month <= 6)
	{
		semester = "Q3";
		next_semester = "Q4";
		next_year = year;
	}
	else
	{
		semester = "Q4";
		next_semester = "Q1";
		next_year = year;
	}
	
	query = "call check_quarter (\"" + course
		+ "\", " + to_string(year) + ", \"" + semester
		+ "\", " + to_string(next_year) + ", \"" + next_semester
		+ "\", " + to_string(in_year) + ", \"" + in_semester + "\");";
	
	if(mysql_query(conn, query.c_str()))
	{
		cout << "error!!" << endl;
		cout << mysql_error(conn);
		exit(1);
	}
	
	result = mysql_store_result(conn);
	
	int numrows = (int)mysql_num_rows(result);
	if(numrows != 0)
	{
		cout << "Quarter Checks Satisfied!" << endl;
		satisfied = true;
	}
	else
		cout << "This course is not registered for this quarter or next quarter!" << endl;
	
	while(mysql_more_results(conn))
		mysql_next_result(conn);
	mysql_free_result(result);
	return satisfied;
}

bool check_maximum_preferred(MYSQL *conn, string course, string in_semester, int in_year)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;
	bool satisfied = false;

	query = "call check_maximum_preferred (\"" + course
		+ "\", " + to_string(in_year) + ", \"" + in_semester
		+ "\", " + to_string(maximum) + ", \"" + classtype + "\");";
	
	if(mysql_query(conn, query.c_str()))
	{
		cout << "error!!" << endl;
		cout << mysql_error(conn);
		exit(1);
	}
	
	result = mysql_store_result(conn);
	
	int numrows = (int)mysql_num_rows(result);
	if(numrows != 0)
	{
		cout << "Maximum Preferred and Classtype Non-Preferred Condition Satisfied!" << endl;
		satisfied = true;
	}
	else
		cout << "Maximum Preferred and Classtype Non-Preferred Condition Not Satisfied!" << endl;
		
	while(mysql_more_results(conn))
		mysql_next_result(conn);
	mysql_free_result(result);
	return satisfied;
}

bool check_prerequisite(MYSQL *conn, int id, string course)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;
	bool satisfied = false;

	query = "call check_prerequisite (" + to_string(id) + ", \"" + course + "\");";
	
	if(mysql_query(conn, query.c_str()))
	{
		cout << "error!!" << endl;
		cout << mysql_error(conn);
		exit(1);
	}
	
	result = mysql_store_result(conn);
	
	int numrows = (int)mysql_num_rows(result);
	if(numrows == 0)
	{
		cout << "Prerequisite Satisfied!" << endl;
		satisfied = true;
	}

	for(int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(result);
		if(row != NULL)
		{
			cout << "You are required to take Prerequisite Course : " << row[0] << endl;
		}
	}
	
	while(mysql_more_results(conn))
		mysql_next_result(conn);
	mysql_free_result(result);
	return satisfied;
}

bool check_maximum(MYSQL *conn, string course, string quarter, int year)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;
	bool satisfied = false;

	query = "call check_maximum (\"" + course + "\", "
		+ to_string(year) + ", \"" + quarter + "\");";
	
	if(mysql_query(conn, query.c_str()))
	{
		cout << "error!!" << endl;
		cout << mysql_error(conn);
		exit(1);
	}
	
	result = mysql_store_result(conn);
	
	int numrows = (int)mysql_num_rows(result);
	if(numrows != 0)
	{
		cout << "Maximum Satisfied!" << endl;
		satisfied = true;
	}	
	
	while(mysql_more_results(conn))
		mysql_next_result(conn);
	mysql_free_result(result);
	return satisfied;
}

bool check_prev_course (MYSQL *conn, int id, string course)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;
	bool satisfied = false;

	query = "call check_prev_course (" + to_string(id) + ", \"" + course + "\");";
	
	if(mysql_query(conn, query.c_str()))
	{
		cout << "error!!" << endl;
		cout << mysql_error(conn);
		exit(1);
	}
	
	result = mysql_store_result(conn);
	
	int numrows = (int)mysql_num_rows(result);
	if(numrows == 0)
	{
		cout << "Prev_course Satisfied!" << endl;
		satisfied = true;
	}

	for(int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(result);
		if(row != NULL)
		{
			cout << "You took this course previously : " << row[0] << endl;
		}
	}
	
	while(mysql_more_results(conn))
		mysql_next_result(conn);
	mysql_free_result(result);
	return satisfied;
}

void course_add(MYSQL *conn, int id, string course, string quarter, int year)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;
	bool satisfied = false;

	query = "call course_add (" + to_string(id) + ", \"" + course + "\", \""
		+ quarter + "\", " + to_string(year) + ");";
	
	
	if(mysql_query(conn, query.c_str()))
	{
		cout << "error!!" << endl;
		cout << mysql_error(conn);
		exit(1);
	}
}

void withdraw(MYSQL *conn, int id)
{
	string course_num;
	string quarter;
	int year(0);
	cout << "\n<<<<<<<<<< Select Courses To Withdraw >>>>>>>>>>" << endl;
	possible_withdraw_courses(conn, id);
	while(true)
	{
		cout << "==================== WITHDRAW MENU ====================" << endl;
		cout << "Enter Course Number to withdraw(Enter 0 to STUDENT MENU) : ";
		cin >> course_num;
		if(course_num == "0")
			break;
		cout << "Enter Quarter to withdraw : ";
		cin >> quarter;
		cout << "Enter Year to withdraw : ";
		cin >> year;

		if(check_course_withdraw(conn, id, course_num, quarter, year))
		{
			course_withdraw(conn, id, course_num, quarter, year);
			cout << "Withdrawal Success!" << endl;
		}
		fflush(stdin);		
	}
}

void possible_withdraw_courses(MYSQL *conn, int id)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;
	time_t timer = time(NULL);
	struct tm *t = localtime(&timer);
	int year = t->tm_year + 1900;
	int month = t->tm_mon + 1;
	int day = t->tm_mday;
	string semester, next_semester;
	int next_year;
	
	if(month >= 9 && month <= 12)
	{
		semester = "Q1";
		next_semester = "Q2";
		next_year = year + 1;
	}
	else if(month >= 1 && month <= 3)
	{
		semester = "Q2";
		next_semester = "Q3";
		next_year = year;
	}
	else if(month >= 4 && month <= 6)
	{
		semester = "Q3";
		next_semester = "Q4";
		next_year = year;
	}
	else
	{
		semester = "Q4";
		next_semester = "Q1";
		next_year = year;
	}
	
	query = "call possible_withdraw_courses (" + to_string(id)
		+ ", " + to_string(year) + ", \"" + semester
		+ "\", " + to_string(next_year) + ", \"" + next_semester + "\");";

	if(mysql_query(conn, query.c_str()))
	{
		cout << "error!!" << endl;
		cout << mysql_error(conn);
		exit(1);
	}
	
	result = mysql_store_result(conn);
	
	int numrows = (int)mysql_num_rows(result);
	if(numrows == 0)
	{
		cout << "There's no courses now!" << endl;
	}	
	
	for(int i = 0; i < numrows; i++)
	{
		row = mysql_fetch_row(result);
		if(row != NULL)
		{
			cout << to_string(i+1) + ". Course Number : " + row[1] + ", Quarter = " + row[2] + ", Year = " + row[3] + ", Grade = Not Applicable" << endl;
		}
	}

	while(mysql_more_results(conn))
		mysql_next_result(conn);
	mysql_free_result(result);
}

bool check_course_withdraw(MYSQL *conn, int id, string course, string quarter, int year)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;
	bool satisfied = false;

	query = "call check_course_withdraw (" + to_string(id) + ", \"" + course + "\", \""
		+ quarter + "\", " + to_string(year) + ");";
	
	if(mysql_query(conn, query.c_str()))
	{
		cout << "error!!" << endl;
		cout << mysql_error(conn);
		exit(1);
	}
	
	result = mysql_store_result(conn);
	
	int numrows = (int)mysql_num_rows(result);
	if(numrows != 0)
	{
		cout << "You can withdraw a course!" << endl;
		satisfied = true;
	}	
	
	while(mysql_more_results(conn))
		mysql_next_result(conn);
	mysql_free_result(result);
	return satisfied;
}

void course_withdraw(MYSQL *conn, int id, string course, string quarter, int year)
{
	MYSQL_RES *result;
	MYSQL_ROW row;
	string query;
	bool satisfied = false;

	query = "call course_withdraw (" + to_string(id) + ", \"" + course + "\", \""
		+ quarter + "\", " + to_string(year) + ");";
	
	if(mysql_query(conn, query.c_str()))
	{
		cout << "error!!" << endl;
		cout << mysql_error(conn);
		exit(1);
	}
}

void personal_details(MYSQL *conn, int id)
{
	int max;
	string type;

	while(true)
	{
		cout << "\n========== PERSONAL_DETAILS MENU ==========" << endl;
		cout << "Enter Maximum Preferred Number of Students(Enter -1 to MAIN MENU) : ";
		cin >> max;
		if(max == -1)
			break;
		cout << "Enter Non-preferred classroom Type(Enter -1 to MAIN MENU) : ";
		cin >> type;
		if(type == "-1")
			break;
				
		change_details(conn, id, max, type);
		fflush(stdin);		
	}
}

void change_details(MYSQL *conn, int id, int max, string type)
{
	string query;

	query = "update student set maximumpreferredenrollment = " + to_string(max)
		+ ", NonPreferredClassroomType = \"" + type + "\" where id = " + to_string(id);;
	
	if(mysql_query(conn, query.c_str()))
	{
		cout << mysql_error(conn);
		exit(1);
	}
}