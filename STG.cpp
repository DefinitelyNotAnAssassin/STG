#include <iostream>
#include "sqlite/sqlite3.h"
#include <string>
#include <windows.h>

#define ITEMS 10



using namespace std;



sqlite3_stmt* stmt;
sqlite3* db;
int rc;
char zErrMsg;
int attempt = 1;

int math_score, english_score, filipino_score, science_score = 0;
bool is_english, is_filipino, is_math, is_science = false; // boolean values that will determine if the category has been answered already.


int count = 0;


/// CATEGORY CODES : 


/*
Original English - 2023123 
Original Filipino - 2023124
Original Math - 2023125
Original Science - 2023126

*/ 


void exit_message(){
    cout << "Thank you for using Dominitest"  << endl;
}

int quiz_score();


// string sql = "SELECT * FROM users";
// sqlite3_prepare_v2(..., ..., ... ,, );
// int rc = sqlite3_step(stmt);

void dominitest();
void create_user();
bool login();
void select_question();
void create_question();

void save_grade(string quiz_id);
void read_grade();

void transmuted_grade();

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
    cat = "English"; // Change the category when running/executing the program.

    

    while(1){
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
    cout << "\n\n\t\t\t\t        1. English\n\t\t\t\t        2. Filipino\n\t\t\t\t        3. Math\n\t\t\t\t        4. Science"; 
    cout << "\n\n\n\t\t\t\t| | | | | | | | | | | | | | | | | | | | | | | | | | | | | " << endl;
    cout <<"\n\n\t\t\t\tEnter Action: ";
    cin >> choice;



    switch(choice)
    {
        case 1:
            category = "English";   
            if (english_score != 0){
                english_score = 0;
            }
            
            break;
        case 2:
            category = "Filipino";
            if (filipino_score != 0){
                filipino_score = 0;
            }
            break;
        case 3:
            category = "Math";
            if (math_score != 0){
            math_score = 0; // resets the score if a score is already existing
             }
            break;
        case 4:
            category = "Science";
            if (science_score != 0){
                science_score = 0;
            }
            break;

        default:
            cout << "Invalid Input";

    }
    system("cls");

    // prepare, bind, step, finalize

    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    
    sqlite3_bind_text(stmt, 1, category.c_str(),-1, SQLITE_STATIC);

    sqlite3_step(stmt);

    if (rc != SQLITE_OK && rc != SQLITE_ROW)
    {
        cout << "Select Error : " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "" << endl;
        string category;
        while (sqlite3_step(stmt) == SQLITE_ROW){
            cout << "\n\n\t\t\t\t\t      Question : " << sqlite3_column_text(stmt, 0) << endl;
            cout << "\n\t\t\t\t\t        Choice 1 : " << sqlite3_column_text(stmt, 1) << endl;
            cout << "\n\t\t\t\t\t        Choice 2 : " << sqlite3_column_text(stmt, 2) << endl;
            cout << "\n\t\t\t\t\t        Choice 3 : " << sqlite3_column_text(stmt, 3) << endl;
            cout << "\n\t\t\t\t\t        Choice 4 : " << sqlite3_column_text(stmt, 4) << endl;
            category = (char*) sqlite3_column_text(stmt, 6);
            int ans;
            
            
            int real_ans = sqlite3_column_int(stmt, 5);
            cout << "\n\t\t\t\t\t        Answer : ";
            cin >> ans;

            // put some score function here

            if (real_ans == ans){
                if (category == "English"){
                    english_score++;
                    is_english = true; //sets is_english to mark as answered
                }
                else if (category == "Filipino"){
                    filipino_score++;
                    is_filipino == true;
                }
                else if (category == "Math"){
                    math_score++;
                    is_math = true;
                }
                else if (category == "Science"){
                    science_score++;
                    is_science = true;
                }

            }
           
            system("cls");
        }
    }
    sqlite3_finalize(stmt);
   transmuted_grade();

    int opt;

    cout << "\n\t\t\t\t\t        1. Try Again\n\t\t\t\t\t        2. Exit\n\t\t\t\t\t       Select Action: ";
    cin >> opt;
    switch(opt){
        case 1:
            select_question();
            
            break;
        case 2:
            exit_message();
            exit(0);
            break;
     
        default:
            cout << "\n\t\t\t\t\t        Invalid Input. Try again \n";
            
    }
    

  
}



void transmuted_grade(){
    int t50, t70, t80;

    
    // Now the boolean values of is_category will play the magic here determining what grades to show 


    if(english_score){
    t50=((english_score/ITEMS)*50)+50;
	t70=((english_score/ITEMS)*50)+40;
	t80=((english_score/ITEMS)*50)+35;
	
    cout <<"\n\t\t\t\t\t        ENGLISH SCORES  " << endl << endl;
    cout << "\n\t\t\t\t\t        Raw Score : " << english_score << endl;
    cout << "\n\t\t\t\t\t        80% Passing : " << t80 << endl;
    cout << "\n\t\t\t\t\t        70% Passing : " << t70 << endl;
    cout << "\n\t\t\t\t\t        50% Passing : " << t50 << endl << endl;
    } 
    // ^^ if english_score is true or if its already 'answered' then show the scores
    
    
    if (filipino_score){
         
    t50=((filipino_score/ITEMS)*50)+50;
	t70=((filipino_score/ITEMS)*50)+40;
	t80=((filipino_score/ITEMS)*50)+35;
	
    cout <<"\n\t\t\t\t\t        FILIPINO SCORES   " << endl << endl;
    cout << "\n\t\t\t\t\t        Raw Score : " << filipino_score << endl;
    cout << "\n\t\t\t\t\t        80% Passing : " << t80 << endl;
    cout << "\n\t\t\t\t\t        70% Passing : " << t70 << endl;
    cout << "\n\t\t\t\t\t        50% Passing : " << t50 << endl << endl;
    }

    // you can notice that I used if - if  not a if - else if, that is a crucial logical choice,
    // if - else if = If the first condition is met, then the rest is ignored
    // if - if = If the first condition is met, then the next condition will also be checked.



    if (math_score){

    t50=((math_score/ITEMS)*50)+50;
	t70=((math_score/ITEMS)*50)+40;
	t80=((math_score/ITEMS)*50)+35;
	
    cout <<"\n\t\t\t\t\t        MATH SCORES  " << endl << endl;
    cout << "\n\t\t\t\t\t        Raw Score : " << math_score << endl;
    cout << "\n\t\t\t\t\t        80% Passing : " << t80 << endl;
    cout << "\n\t\t\t\t\t        70% Passing : " << t70 << endl;
    cout << "\n\t\t\t\t\t        50% Passing : " << t50 << endl << endl;


    }

    if (science_score){

    t50=((science_score/ITEMS)*50)+50;
	t70=((science_score/ITEMS)*50)+40;
	t80=((science_score/ITEMS)*50)+35;
	
    cout <<"\n\t\t\t\t\t        SCIENCE SCORES  " << endl << endl;
    cout << "\n\t\t\t\t\t        Raw Score : " << science_score << endl;
    cout << "\n\t\t\t\t\t        80% Passing : " << t80 << endl;
    cout << "\n\t\t\t\t\t        70% Passing : " << t70 << endl;
    cout << "\n\t\t\t\t\t        50% Passing : " << t50 << endl << endl;


    }
    

   
}

