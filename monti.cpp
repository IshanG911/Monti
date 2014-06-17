#include<cstring>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include<fstream>
#include<conio.h>

#define ScreeningFrequency 3

using namespace std;
char EMPTYSEAT=176, BOOKEDSEAT=219;


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
void bookShow();
/*User class containing details of Users*/
class User{
	public:
		char Uname[15], Pass[15];	//Uname is primary Key
		char Email[15], Name[20], Address[30], Phone[11];
		void showUserDetail();
};

void User::showUserDetail(){
	cout<<"\n\tUsername: "<<this->Uname<<"\tFull Name: "<<this->Name<<"\n\tEmail: "<<this->Email<<"\tContact: "<<this->Phone<<"\n\tAddress:"<<this->Address;
	cout<<"\n-------------------------------------------------------------------------------";
}
/* Global Class Variables to Access objects*/
User Global_User;

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
		cout<<"\n\n\tUserName: ";
		gets(NUser.Uname);
		fflush(stdin);	//fflush is used to clear the keyboard input buffer
		cout<<"\tPassword: ";
		strcpy(NUser.Pass,hidePassword());
		fflush(stdin);
		cout<<"\n\tEmail ID: ";
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
			LoginMenu();
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
	User tempUser;
	ifstream inFile("User.dat");
	if(inFile){
		while(!inFile.eof()){
			inFile.read((char*)&tempUser, sizeof(tempUser));
			if(!strcmpi(tempUser.Uname,Name)){
				inFile.close();
				return 0;
			}
		}
		inFile.close();
	}
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
	cout<<"\n Choose from following options:\n\t1. Book A Show\n\t2. Cancel Ticket\n\t3. View Booking History\n\t4. Logout\n\t-->";
	cin>>choice;
	switch(choice){
		case 1:	bookShow();
				break;
	}
	getchar();
}

/*Movie class containing details of Movies*/
class Movie{
	public:		//Movie is primary Key
		short int SEAT[ScreeningFrequency][120];
		char MovieTitle[30], screenDate[ScreeningFrequency][12], screeningTime[10], Detail[40];	//Movie Screened for 5 Days
		int TotalSeats[ScreeningFrequency], SeatsBooked[ScreeningFrequency], SeatsLeft[ScreeningFrequency], Fare;
		Movie(){
			short int i=0,j=0;
			for(i=0;i<ScreeningFrequency;i++){
				TotalSeats[i] = 120;	// Every Movie Screening has 120 seats
				SeatsBooked[i] = 0;
				SeatsLeft[i] = 120;
				for(j=0; j<120; j++)
				{
						SEAT[i][j]=0;
				}
			}
		}
		void addMovie();
		void getMovieDetail(int);
		void getFullMovieDetail();
};

/*Ticket class containing details of Transactions*/
class Ticket{
	public:
		int TID;//random unique Ticket ID
		char Uname[15], MovieTitle[20], Date[12], Time[12];	//Uname is primary Key
		int TotalSeats, TotalCost;	//Seat No.s are limited to 6 per User
		char seatNo[10][3];
		void MakeNewTicket(Movie Mov, char seats[][3], short int nos, short int chooseDate);
		void PrintTicket();
};

void Ticket::MakeNewTicket(Movie Mov, char seats[10][3], short int nos, short int chooseDate)
{
	short int i=0;
	char choice='n';
	srand(time(NULL));
	this->TID = 99999 + rand()%999999;
	strcpy(this->Uname, Global_User.Uname);
	strcpy(this->MovieTitle, Mov.MovieTitle);
	strcpy(this->Date, Mov.screenDate[chooseDate]);
	strcpy(this->Time, Mov.screeningTime);
	this->TotalSeats = nos;
	this->TotalCost = nos*Mov.Fare;
//Insert all seat no.s in the array	
	for(i=0;i<nos;i++)
		strcpy(this->seatNo[i], seats[i]);
//Make remaining seats NULL
	for(;i<10;i++)
		strcpy(this->seatNo[i],"\0");
	PrintTicket();	
	cout<<"\n Confirm Ticket? (Y/N): ";
	cin>>choice;
	if(choice=='y' || choice=='Y')
		addToFile("Ticket.dat",this);
	User_MainScreen();
}

void Ticket::PrintTicket(){
	short int i=0;
	cout<<"\n\n---------------------------: Ticket Details :-----------------------------------";
	cout<<"\n TID: "<<TID<<"\tUSER: "<<Uname<<"\t Movie: "<<MovieTitle<<"\n Screen: Prometheus (Floor 1)";
	cout<<"\n DATE: "<<Date<<"\t TIME: "<<Time<<"\n TotalSeats: "<<TotalSeats;
	cout<<"\t SEAT: ";
	for( i=0;i<TotalSeats-1;i++)
		cout<<seatNo[i]<<", ";
	cout<<seatNo[i];
	cout<<"\t TOTAL FARE: "<<TotalCost;
	cout<<"\n---------------------------------: ~ :-----------------------------------------";
}

void Movie::getMovieDetail(int index){
	cout<<"\n"<<index<<". Title: "<<this->MovieTitle<<" | Release Date: "<<this->screenDate[0]<<" | Detail: "<<this->Detail;
	cout<<"\n-----------------------------------------------------------------";
}

void Movie::getFullMovieDetail(){
	cout<<"\n********************************************************************************\n\t\t\t\t User Menu: Movie Details";
	cout<<"\n********************************************************************************\n";
	cout<<"\n\tMovie Title: "<<MovieTitle;
	cout<<"\n\tMovie Details: "<<Detail;
	cout<<"\n\tScreening Time (Daily): "<<screeningTime;
	cout<<"\n\tScreening Dates: ";
	for(short int i=0;i<ScreeningFrequency;i++)
		cout<<screenDate[i]<<',';
	cout<<"\n------------------------------------------------------------------";
}

void Movie::addMovie(){
	short int i=0;
	system("cls");
	cout<<"\n********************************************************************************\n\t\t\t\t Admin Menu: Adding Movie";
	cout<<"\n********************************************************************************\n";
	cout<<"\n Enter the following Movie Details:";
	fflush(stdin);	
	cout<<"\n\tMovie Title: ";
	gets(MovieTitle);
	fflush(stdin);	//fflush is used to clear the keyboard input buffer
	for(i=0;i<ScreeningFrequency;i++){
		cout<<"\tScreening Date "<<i+1<<" : ";
		gets(screenDate[i]);
		fflush(stdin);	
	}
	cout<<"\tScreening Time (Daily):";
	gets(screeningTime);
	fflush(stdin);	
	cout<<"\tMovie Details: ";
	gets(Detail);
	fflush(stdin);	
	cout<<"\tBase Fare: ";
	cin>>Fare;
	fflush(stdin);
}

void postNewMovie(){
	Movie Mo;
	Mo.addMovie();
	addToFile("Movie.dat",Mo);
	cout<<"\n Movie added successfully!!";
}

void drawSeats(Movie selectedMovie, int chooseDate){
	short int i=0,j=0;
	char ch='A';
	system("cls");
	cout<<"\n   0\t1\t2\t3\t4\t5\t6\t7\t8\t9\n";
	for(i=0;i<120;i++){
		if(i%10 == 0)
			cout<<'\n'<<" "<<ch++<<" ";
		if(selectedMovie.SEAT[chooseDate][i] == 0)	//when seat is empty output emptyseat
			cout<<EMPTYSEAT<<'\t';
		else
			cout<<BOOKEDSEAT<<'\t';
	}
	cout<<"\n\tEmpty Seats:"<<EMPTYSEAT<<"\t\tBooked Seats:"<<BOOKEDSEAT;
}

void bookShow(){
	Ticket T;
	short int i=0, choice=0, nos=0, seat_no=-1,chooseDate=0;
	char seat_id[3], seat_array[10][3];
	Movie tempMovie[5], selectedMovie;
	ifstream inFile("Movie.dat");
	system("cls");
	if(inFile){
		while(!inFile.eof()){
			inFile.read((char*)&tempMovie[i], sizeof(tempMovie[i]));
			tempMovie[i].getMovieDetail(i+1);
			++i;
		}
		cout<<"\n Choose the show: ";
		cin>>choice;
		i=0;
		inFile.close();
		inFile.open("Movie.dat",ios::in);
//		inFile.seekg(0, ios::beg);
		do{
			inFile.read((char*)&selectedMovie, sizeof(selectedMovie));
			++i;
		}while(!inFile.eof() && i<choice);
		
		selectedMovie.getFullMovieDetail();
		cout<<"\n Choose Date of Booking the Show from following:";
	//Choose the show from a particular date	
		for(i=0;i<ScreeningFrequency;i++)
			cout<<"\n\t"<<i+1<<".\t"<<selectedMovie.screenDate[i];
		cout<<"\n\t\t-->";
		cin>>chooseDate;
		chooseDate-=1;
	//Check if the selected date is valid
		if(chooseDate>=0 && chooseDate<=ScreeningFrequency){
			drawSeats(selectedMovie, chooseDate);
			cout<<"\n\n Number of Seats to be Booked:";
			cin>>nos;
			if(nos<=selectedMovie.SeatsLeft[chooseDate]){
					for(i=0;i<nos;i++){
						seat_no = -1;
						cout<<"\n Seat "<<i+1<<": ";
						cin>>seat_id;
			/* Seat ID is now parsed to find the exact seat no. and weather it is available*/
						if(seat_id[0]>='a' && seat_id[0]<='j')
							if(seat_id[1]>='0' && seat_id[1]<='9'){
								seat_no = seat_id[0]-'a';
								seat_no *= 10;
								seat_no += (seat_id[1] - '0');					
							}
						else if(seat_id[0]>='A' && seat_id[0]<='J')
							if(seat_id[1]>='0' && seat_id[1]<='9'){
								seat_no = seat_id[0]-'A';
								seat_no *= 10;
								seat_no += (seat_id[1] - '0');					
							}
						else cout<<"\n Invalid Seat No. entered";
												
						if(selectedMovie.SEAT[chooseDate][seat_no]==1)
							cout<<"\n Sorry! Selected seat is already booked!!";
						else if(selectedMovie.SEAT[chooseDate][seat_no]==0 && seat_no!=-1){
							selectedMovie.SEAT[chooseDate][seat_no] = 1;	// Book the seat
							selectedMovie.SeatsBooked[chooseDate]++;
							selectedMovie.SeatsLeft[chooseDate]--;
							strcpy(seat_array[i],seat_id);
							
							cout<<" Seat "<<seat_id<<" booked. Charge added to the Total!\n";
						}
						else;
					}
			}
			cout<<"\n All seats booked... generating reciept for your transaction!";
			T.MakeNewTicket(selectedMovie, seat_array, nos, chooseDate);
		}
	}
}

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
	if(!strcmpi(U,this->Uname) && !strcmpi(P,this->Pass))	return 1;
	return 0;
}

/*Screen with all Admin Options*/
void Admin_MainScreen(){
	int choice=0;
	do{
		system("cls");
		cout<<"\n********************************************************************************\n\t\t\t\t Admin Menu: Main Screen";
		cout<<"\n********************************************************************************\n";
		cout<<"\n\t1. Post New Movie\n\t2. Remove Movie\n\t3. View Movie Stats\n\t4. View User Stats\n\t5. Logout\n\t-->";
		cin>>choice;
		switch(choice){
			case 1:	postNewMovie();
					break;
		}
	}while(choice!=5);
	LoginMenu();
}

/* Function for Administrator to Login to the System*/
void AdminLogin(){
	Admin MainAdmin;
	char U[15], P[15];
	system("cls");
	cout<<"\n********************************************************************************\n\t\t\t\t Admin Menu: Login Screen";
	cout<<"\n********************************************************************************\n";
	fflush(stdin);
	cout<<"\n\tEnter Administrator's ID: ";
	gets(U);
	fflush(stdin);
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

void LoginMenu(){
		int choice=0;
			system("cls");
			cout<<"\n********************************************************************************\n\t\t\t\tWELCOME To GALAXY Cinema: Login/SignUp";
			cout<<"\n********************************************************************************\n";
			cout<<"\n\n\t\t1. Login\n\n\t\t2. Sign Up\n\n\t\t3. Administrator Login (Not for Users)\n\n\t\t4. Exit\n\n\t\t-->";
			cin>>choice;
			switch(choice){
				case 1: existingUser();
						break;
				case 2: newUser();
						break;
				case 3: AdminLogin();
						break;
				case 4: system("exit");
						break;
				default:
						cout<<"Wrong Input!!";
			}
}	


int main(){
	LoginMenu();
	return 0;
}
