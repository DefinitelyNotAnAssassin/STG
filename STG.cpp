#include <iostream>
#include "sqlite/sqlite3.h"
#include <string>
#include <windows.h>



using namespace std;

struct User{
    int student_number;
    string firstname;
    string lastname;
    
};


User current_user;



sqlite3_stmt* stmt;
sqlite3* db;
int rc;
char zErrMsg;
int attempt = 1;




void exit_message(){
    cout << "Thank you for using Dominitest"  << endl;
}


int english_score = 0;
int filipino_score = 0;
int math_score = 0;
int science_score = 0;


// string sql = "SELECT * FROM users";
// sqlite3_prepare_v2(..., ..., ... ,, );
// int rc = sqlite3_step(stmt);

void dominitest();
void create_user();
bool login();
void select_question();
void create_question();

void save_grade();
void read_grade();

int main()
{
     

    sqlite3_open("quiz.db", &db);
   
    dominitest();
    
    int opt;
    cout << "\n\n\n1. Login\n2. Sign-in \nChoose from above: ";
    cin >> opt;

    switch (opt)
    {
    case 1: 
        login();
        break;

    case 2: 
        create_user();
        login();
        break;
    default:
        break;
    }
    if(attempt > 2)
    {
        cout << "\n\t\t\t        REMAINING ATTEMPTS HAS BEEN USED, PLEASE RERUN THE PROGRAM AGAIN.";        
    }
    else
    {
        select_question();
    }


    return 0;
    
}

void gotoxy(int x, int y)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hStdOut, coord);
}

void dominitest()
{
		for (int x=0; x<=25; x++)
		{
			gotoxy(x,0);
			cout << " ||||      ||||    |||   |||  ||||||||  |||   ||  ||||||||  ||||||||  ||||||||    ||||||||   |||||||| ";
			gotoxy(x,1);
			cout << " ||  ||   ||  ||   |||| ||||     ||     ||||  ||     ||        ||     ||          ||            ||    ";
			gotoxy(x,2);
			cout << " ||  ||  ||    ||  || ||| ||     ||     || || ||     ||        ||     ||||||      ||||||||      ||    ";
			gotoxy(x,3);
			cout << " || ||    ||  ||   ||     ||     ||     ||  ||||     ||        ||     ||                ||      ||    ";
			gotoxy(x,4);
			cout << " ||||      ||||    ||     ||  ||||||||  ||   |||  ||||||||     ||     ||||||||    ||||||||      ||    ";
			Sleep(50);
		}
	
		for (int x = 25; x>=0; x--)
		{		
			gotoxy(x,0);
			cout << " ||||      ||||    |||   |||  ||||||||  |||   ||  ||||||||  ||||||||  ||||||||    ||||||||   |||||||| ";
			gotoxy(x,1);
			cout << " ||  ||   ||  ||   |||| ||||     ||     ||||  ||     ||        ||     ||          ||            ||    ";
			gotoxy(x,2);
			cout << " ||  ||  ||    ||  || ||| ||     ||     || || ||     ||        ||     ||||||      ||||||||      ||    ";
			gotoxy(x,3);
			cout << " || ||    ||  ||   ||     ||     ||     ||  ||||     ||        ||     ||                ||      ||    ";
			gotoxy(x,4);
			cout << " ||||      ||||    ||     ||  ||||||||  ||   |||  ||||||||     ||     ||||||||    ||||||||      ||    ";
			Sleep(50);
		}	
}

void create_user()
{
    string sql = "INSERT INTO users (first_name, last_name, student_number, password) VALUES ( ?, ?, ?, ?) ";
    string Fname, Lname, pass;
    int SN;

    system("cls");
    cout << "\n\t\t\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | " << endl;
    cout << "\n\t\t\t\t                    STUDENT INFORMATION: ";
    cout << "\n\n\t\t\t        FIRST NAME: ";
   cin.ignore();
    getline(cin, Fname);
    cout << "\n\t\t\t        LAST NAME: ";
    getline(cin, Lname);
    cout << "\n\t\t\t        STUDENT NUMBER : ";
    cin >> SN;
    cout << "\n\t\t\t        PASSWORD: ";
    cin >> pass;
    cout << "\n\t\t\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | " << endl;

    // prepare, bind, step, finalize

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, Fname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, Lname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, SN);
    sqlite3_bind_text(stmt, 4, pass.c_str(), -1, SQLITE_STATIC);
   
    sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    
}

bool login() 
{
    string sql = "SELECT * FROM users WHERE student_number = ? AND password = ?";
    string pass;
    int SN;
    
    cout << "\n\t\t\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | " << endl;
    cout << "\n\t\t\t\t                    STUDENT INFORMATION: ";
    cout << "\n\n\t\t\t        STUDENT NUMBER : ";
    cin >> SN;
    cout << "\n\t\t\t        PASSWORD: ";
    cin >> pass;
    cout << "\n\t\t\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | " << endl;

    int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, SN);
    sqlite3_bind_text(stmt, 2, pass.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) 
    {
        cout << "Login successful!" << endl;
        current_user.firstname = (char*) sqlite3_column_text(stmt, 0);
        current_user.lastname = (char*) sqlite3_column_text(stmt, 1);
        current_user.student_number = sqlite3_column_int(stmt, 2);
        select_question();
        return true;
    } else if (rc != SQLITE_ROW && attempt <=3)
    {
        cout << "\n\t\t\t       Incorrect Student Number/Password"<< endl;
        attempt++;
        login();
        
        return false;
    }
    else{
        cout << "\n\t\t\t       All of the remaining attempts has been reached. Please try again later" << endl;
        return false;
    }
    
}

void create_question()
{
    system("cls");
    string sql = "INSERT INTO questions (question, choice1, choice2, choice3, choice4, ans, category) VALUES (?, ?, ?, ?, ?, ?, ?) ";
    string q, c1, c2, c3, c4, cat;
    int ans;

    cout << "Question : ";
    getline(cin, q);

    cout << "Choice 1 : ";
    getline(cin, c1);

    cout << "Choice 2 : ";
    getline(cin, c2);

    cout << "Choice 3 : ";
    getline(cin, c3);

    cout << "Choice 4 : ";
    getline(cin, c4);

    cout << "Answer : ";
    cin >> ans;
 
    cat = "Science";
    // prepare, bind, step, finalize
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, q.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, c1.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, c2.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, c3.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 5, c4.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 6, ans);
    sqlite3_bind_text(stmt, 7, cat.c_str(), -1, SQLITE_STATIC);
   
    sqlite3_step(stmt);

    if (rc != SQLITE_OK){
        cout << "Insert Error : " << zErrMsg << endl;
    }
    else{
        cout << "Insert Successfully" << endl;
    }

    sqlite3_finalize(stmt);
}

void select_question()
{
    string sql = "SELECT * FROM questions WHERE category = ? ORDER BY RANDOM()";
    int choice;
    string category;
    system("cls");
    dominitest();


    cout << "\n\n\n\t\t\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | " << endl;
    cout << "\n\t\t\t\t\t                 CATEGORIES";
    cout << "\n\n\t\t\t\t1. English\n\t\t\t\t2. Filipino\n\t\t\t\t3. Math\n\t\t\t\t4. Science";  
    cout << "\n\n\n\t\t\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | " << endl;
    cout <<"\n\n\t\t\t\tEnter Category:";
    cin >> choice;

    switch(choice)
    {
        case 1:
            category = "English";   
            break;
        case 2:
            category = "Filipino";
            break;
        case 3:
            category = "Math";
            break;
        case 4:
            category = "Science";
            break;

        default:
            cout << "Invalid Input";
    }
    if(category == "English")
    {
        if(english_score != 0)
        {
            english_score = 0;
        }
    }
    if(category == "Filipino")
    {
        if(filipino_score != 0)
        {
            filipino_score = 0;
        }
    }
    if(category == "Math")
    {
        if(math_score != 0)
        {
            math_score = 0;
        }
    }
    if(category == "Science")
    {
        if(science_score != 0)
        {
            science_score = 0;
        }
    }
    system("cls");

    // prepare, bind, step, finalize

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, category.c_str(),-1, SQLITE_STATIC);

    sqlite3_step(stmt);

    if (rc != SQLITE_OK)
    {
        cout << "Select Error : " << zErrMsg << endl;
    }
    else
    {
        cout << "" << endl;

        while (sqlite3_step(stmt) == SQLITE_ROW){
            cout << "\n\n\t\t\t\t\t      Question : " << sqlite3_column_text(stmt, 0) << endl;
            cout << "\n\t\t\t\t\t        Choice 1 : " << sqlite3_column_text(stmt, 1) << endl;
            cout << "\n\t\t\t\t\t        Choice 2 : " << sqlite3_column_text(stmt, 2) << endl;
            cout << "\n\t\t\t\t\t        Choice 3 : " << sqlite3_column_text(stmt, 3) << endl;
            cout << "\n\t\t\t\t\t        Choice 4 : " << sqlite3_column_text(stmt, 4) << endl;
            cout << "\n\t\t\t\t\t        Category : " << sqlite3_column_text(stmt, 6) << endl;
           
            int ans;
            
            string category = (const char*)sqlite3_column_text(stmt, 6);
            
            int real_ans = sqlite3_column_int(stmt, 5);
            cout << "\n\t\t\t\t\t        Answer : ";
            cin >> ans;

            if (ans == real_ans){

                if(category== "English")
                {
                    english_score++;
                }
                else if (category== "Filipino")
                {
                    filipino_score++;
                }
                else if (category== "Math")
                {
                    math_score++;
                }
                else if(category == "Science")
                {
                    science_score++;
                }
                else{
                    cout << "501 - Problem Occurred" << endl;
                }
                
                cout << "" << endl;
            }
            else{
                cout << "" << endl;
            }


            system("cls");
        }
    }
    sqlite3_finalize(stmt);

    cout << "\n\t\t\t\t\t        English score : " << english_score << "/10" << endl;
    cout << "\n\t\t\t\t\t        Filipino score : " << filipino_score << "/10" << endl;
    cout << "\n\t\t\t\t\t        Math score : " << math_score << "/10" << endl;
    cout << "\n\t\t\t\t\t        Science score : " << science_score << "/10" << endl;


    int opt;

    cout << "\n\t\t\t\t\t        1. Try Again\n\t\t\t\t\t        2. Save Grades\n\t\t\t\t\t        3. Exit Program\n\t\t\t\t\t        Select Action: ";
    cin >> opt;
    switch(opt){
        case 1:
            select_question();
            break;
        case 2:
            save_grade();
            filipino_score, english_score, math_score, science_score = 0;
            break;

        case 3:
            exit_message();
            exit(0);
            break;


        default:
            cout << "\n\t\t\t\t\t        Invalid Input. Try again \n";
            
    }
    

  
}


void save_grade(){
  
    string sql = "INSERT INTO grades (user_id, filipino_grade, english_grade, math_grade, science_grade, date) VALUES (?, ?, ?, ?< ?, ?)";
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    auto date = time(nullptr);


    sqlite3_bind_int(stmt, 1, current_user.student_number);
    sqlite3_bind_int(stmt, 2, filipino_score);
    sqlite3_bind_int(stmt, 3, english_score);
    sqlite3_bind_int(stmt, 4, math_score);
    sqlite3_bind_int(stmt, 5, science_score);
    sqlite3_bind_int64(stmt, 6, date);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE && rc != SQLITE_OK){
        cout << "Error : " << sqlite3_errmsg(db);
    }
    else{
        cout << "Grade Inserted" << endl;
    }


}


