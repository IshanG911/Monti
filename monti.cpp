#include<cstring>
#include<iostream>
#include<fstream>
#include<conio.h>
using namespace std;

/* Generic functions to Add & Delete Objects from file */

/*Generic function to Add an Object to the passed Filename*/
template <class T>
int addToFile(const char* fileName, T& Object){	//Return true if added successfully else false
	ofstream outFile;
	outFile.open(fileName, ios::out | ios::app);
	outFile.write((char*) &Object, sizeof(Object));
	outFile.close();
}

//Other Function Declarations
int verify_login(char*, char*);
void existingUser();
void newUser();
int verify_uname(char*);
void User_MainScreen();
void LoginMenu();
char* hidePassword();
/*User class containing details of Users*/
class User{
	public:
		char Uname[15], Pass[15];	//Uname is primary Key
		char Email[15], Name[20], Address[30], Phone[11];
		void showUserDetail();
};

void User::showUserDetail(){
	cout<<"\n\n Username: "<<this.Uname<<" | Full Name: "<<this.Name<<" | Email: "<<this.Email<<"\n Address:"<<this.Address<<" | Contact: "<<this.Phone;
}
/* Global Class Variables to Access objects*/
User Global_User;

void LoginMenu(){
		int choice=0;
			system("cls");
			cout<<"\n********************************************************************************\n\t\t\t\tWELCOME To GALAXY Cinema: Login/SignUp";
			cout<<"\n********************************************************************************\n";
			cout<<"\n\n\t\t1. Login\n\n\t\t2. Sign Up\n\n\t\t3. Exit\n\n\t\t-->";
			cin>>choice;
			switch(choice){
				case 1: existingUser();
						break;
				case 2: newUser();
						break;
				case 3: system("exit");
						break;
				default:
						cout<<"Wrong Input!!";
			}
}	

/* Function to Add new User to the File*/
void newUser(){
	User NUser;
	char confirm='n', login='n';
	int uniqueFlag = 0;
	
	while(confirm=='n' || confirm=='N'){
		system("cls");
		fflush(stdin);	//fflush is used to clear the keyboard input buffer
		cout<<"\n********************************************************************************\n\t\t\t\tNEW USER : WELCOME To GALAXY Cinema";
		cout<<"\n********************************************************************************\n";
		cout<<"\n Please Enter following details:";
		cout<<"\n\tUserName: ";
		gets(NUser.Uname);
		fflush(stdin);	//fflush is used to clear the keyboard input buffer
		cout<<"\tPassword: ";
		strcpy(NUser.Pass,hidePassword());
		fflush(stdin);
		cout<<"\tEmail ID: ";
		gets(NUser.Email);
		fflush(stdin);
		cout<<"\tFirst & Last name: ";
		gets(NUser.Name);
		fflush(stdin);
		cout<<"\tAddress: ";
		gets(NUser.Address);
		fflush(stdin);
		cout<<"\tContact No: ";
		gets(NUser.Phone);
		fflush(stdin);
	
		cout<<"\n\n Confirm Details (Y/N):";
		cin>>confirm;
		
		if(confirm=='y' || confirm=='Y')
			uniqueFlag = verify_uname(NUser.Uname);	//Function to check if Uname exist already
		
		if(!uniqueFlag){	//If Uname not unique
			cout<<"\n Sorry the User Name already Exist!!\n Do you wish to login (Y/N)?";
			cin>>login;
			
			if(login=='y' || login=='Y')
				existingUser();
			else
				confirm = 'n'; 				
		}
		else{	//if Uname unique
			cout<<"\n Account Added, Congratulations!!";
			addToFile("User.dat", NUser);
			getchar();
		}
	}
}

void existingUser(){
		char U[15], P[15];
		system("cls");
		fflush(stdin);	//fflush is used to clear the keyboard input buffer
		cout<<"\n********************************************************************************\n\t\t\t\tUSER LOGIN : WELCOME To GALAXY Cinema";
		cout<<"\n********************************************************************************\n";
		cout<<"\tEnter UserName: ";
		gets(U);
		fflush(stdin);	//fflush is used to clear the keyboard input buffer
		cout<<"\tEnter Password: ";
		strcpy(P,hidePassword());
		fflush(stdin);
		if(verify_login(U,P))
			User_MainScreen();
		else{
			cout<<"\n User Name or Password doesn't match!!";
			getchar();
			LoginMenu();
		}
}

int verify_uname(char *Name){	//return false if not Unique else true
	ifstream inFile;
	User tempUser;
	inFile.open("User.dat",ios::in);
	while(!inFile.eof()){
		inFile.read((char*)&tempUser, sizeof(tempUser));
		if(!strcmpi(tempUser.Uname,Name)){
			inFile.close();
			return 0;
		}
	}
	inFile.close();
	return 1;
}

int verify_login(char *U, char *P){	//check login details
	ifstream inFile;
	User tempUser;
	inFile.open("User.dat",ios::in);
	while(!inFile.eof()){
		inFile.read((char*)&tempUser, sizeof(tempUser));
		if(!strcmpi(tempUser.Uname,U) && !strcmpi(tempUser.Pass,P)){
			inFile.close();
			Global_User = tempUser;	//Pin Global User so it can be easily accessed after login
			return 1;
		}
	}
	inFile.close();
	return 0;
}

/*Function to hide the input password*/
char* hidePassword(){
	int i=0;
	char ch, string[15];
	while(1){
		ch = getch();
		if(ch == 13){
			string[i]='\0';
			break;
		}
		else if(ch == 8){
			--i;
			cout<<'\b'<<' '<<'\b';
		}
		else{
			string[i++]=ch;
			cout<<'*';
		}
	}
	return string;
}

void User_MainScreen(){
	int choice = 0;
	system("cls");
	cout<<"\n********************************************************************************\n\t\t\t\t User Menu: Main Screen";
	cout<<"\n********************************************************************************\n";
	Global_User.showUserDetail();
	cout<<"\n\n Choose from following options:\n\t1. Book A Show\n\t2. Cancel Ticket\n\t3. View Booking History\n\t4. Logout\n\t-->";
	getchar();
}

/*Ticket class containing details of Transactions*/
class Ticket{
	public:
		char Uname[15], Movie[20], Date[12], Time[12];	//Uname is primary Key
		int TotalSeats, SeatNo[6], TotalCost;	//Seat No.s are limited to 6 per User
};

/*Movie class containing details of Movies*/
class Movie{
	public:		//Movie is primary Key
		char Movie[20], Date[12], Time[12], Detail[100];
		int TotalSeats, SeatsBooked, SeatsLeft;
		Movie(){
			TotalSeats = 120;
			SeatsBooked = 0;
			SeatsLeft = 0;
		}
};

/*Administrator Class*/
class Admin{
		char Uname[15], Pass[15];
	public:
		Admin(){
			strcpy(Uname,"Mayank");
			strcpy(Pass,"Gupta");
		}
		int verifyAdmin(char*,char*);
		void addMovie();
		void removeMovie();
		void viewUsers();
};

int Admin::verifyAdmin(char* U, char* P){
	if(!strcmpi(U,this.Uname) && !strcmpi(P,this.Pass))	return 1;
	return 0;
}

/*Screen with all Admin Options*/
void Admin_MainScreen(){
	int choice=0;
	system("cls");
	cout<<"\n********************************************************************************\n\t\t\t\t Admin Menu: Main Screen";
	cout<<"\n********************************************************************************\n";
	cout<<"\n\t1. Post New Movie\n\t2. Remove Movie\n\t3. View Movie Stats\n\t4. View User Stats\n\t4. Logout\n\t-->";
	cin>>choice;
	
}

/* Function for Administrator to Login to the System*/
void AdminLogin(){
	Admin MainAdmin;
	char U[15], P[15];
	cout<<"\n********************************************************************************\n\t\t\t\t Admin Menu: Login Screen";
	cout<<"\n********************************************************************************\n";
	cout<<"\n\tEnter Administrator's ID: ";
	gets(U);
	cout<<"\n\tEnter Password: ";
	strcpy(P,hidePassword());
	
	if(MainAdmin.verifyAdmin(U,P))
		Admin_MainScreen();
	else{
		cout<<"\nInvalid Username/Password!!";
		getchar();
		LoginMenu();
	}
}

int main(){
	LoginMenu();
	return 0;
}