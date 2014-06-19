#include<cstring>
#include<stdlib.h>
#include<time.h>
#include<iostream>
#include<fstream>
#include<conio.h>

#define ScreeningFrequency 3

using namespace std;
char EMPTYSEAT=176, BOOKEDSEAT=219;

/*Generic function to Add an Object to the passed Filename*/
template <class T>
int addToFile(const char* fileName, T& Object){	//Return true if added successfully else false
	ofstream outFile;
	outFile.open(fileName, ios::out | ios::app);
	outFile.write((char*) &Object, sizeof(Object));
	outFile.close();
}

/*All general purpose functions*/

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

/* All User Related Functions Declaration*/
int verify_login(char*, char*);
void existingUser();
void newUser();
int verify_uname(char*);
void User_MainScreen();
void LoginMenu();
void viewBookingHistory_USER();
void bookShow();
void cancelBookedTicket();

/*User class containing details of Users*/
class User{
	public:
		char Uname[15], Pass[15];	//Uname is primary Key
		char Email[20], Name[20], Address[30], Phone[11];
		void showUserDetail();
};

void User::showUserDetail(){
	cout<<"\n\tUsername: "<<this->Uname<<"\n\tFull Name: "<<this->Name<<"\n\tEmail: "<<this->Email<<"\n\tContact: "<<this->Phone<<"\n\tAddress:"<<this->Address;
	cout<<"\n-------------------------------------------------------------------------------";
}
//Global Class Variables to Access objects
User Global_User;

// Function to Add new User to the File
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

// Function for existing User
void existingUser(){
		char U[15], P[15];
		system("cls");
		fflush(stdin);	//fflush is used to clear the keyboard input buffer
		cout<<"\n********************************************************************************\n\t\t\t\tUSER LOGIN : WELCOME To GALAXY Cinema";
		cout<<"\n********************************************************************************\n";
		cout<<"\tEnter UserName: ";
		gets(U);
		fflush(stdin);	//fflush is used to clear the keyboard input buffer
		cout<<"\n\tEnter Password: ";
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

//Function to check if new user takes up a new User name and not one already taken by someone else
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

//See if user login successfully
int verify_login(char *U, char *P){	//check login details
	ifstream inFile("User.dat");
	User tempUser;
	if(inFile)
		while(!inFile.eof()){
			inFile.read((char*)&tempUser, sizeof(tempUser));
			if(!strcmpi(tempUser.Uname,U) && !strcmpi(tempUser.Pass,P)){
				inFile.close();
				Global_User = tempUser;	//Pin Global User so it can be easily accessed after login
				return 1;
			}
		}
	else{
		cout<<"\n No such user exist!!";
	}
	inFile.close();
	return 0;
}

/* Function to Display Options for User's Main Screen*/
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
		case 2: cancelBookedTicket();
				system("pause");
				User_MainScreen();
				break;
		case 3: viewBookingHistory_USER();
				system("pause");
				User_MainScreen();
				break;
		case 4: LoginMenu();
				break;
	}
	getchar();
}

/*Movie class containing details of Movies*/
class Movie{
	public:		
		short int SEAT[ScreeningFrequency][120];
		char MovieTitle[30], screenDate[ScreeningFrequency][9], screeningTime[10], Detail[40];	//Movie Screened for 5 Days
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
		void editSeatsInMovie();
};

/*Ticket class containing details of Transactions*/
class Ticket{
	public:
		int TID;//random unique Ticket ID
		char Uname[15], MovieTitle[20], Date[9], Time[10];	//Uname is primary Key
		int TotalSeats, TotalCost;	//Seat No.s are limited to 6 per User
		char seatNo[10][3];
		void MakeNewTicket(Movie Mov, char seats[][3], short int nos, short int chooseDate);
		void PrintTicket();
		void getBasicTicketDetail(int);
};


/*Ticket function to Make a New ticket by User*/
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
}

/*Ticket class function to print Ticket*/
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

void Ticket::getBasicTicketDetail(int index){
	short int i=0;
	cout<<"\n Entry: "<<index<<".\tTID: "<<TID<<"\tUSER: "<<Uname<<"\n MOVIE TITLE: "<<MovieTitle<<"\tDATE: "<<Date<<"\tSEAT: ";
	for(i=0;i<TotalSeats-1;i++)
		cout<<seatNo[i]<<", ";
	cout<<seatNo[i];
	cout<<"\n-------------------------------------------------------------------------------\n";
}

/* View Booking History for User*/
void viewBookingHistory_USER(){
	ifstream inFile("Ticket.dat");
	short int i=1;
	Ticket T;
	system("cls");
	cout<<"\n********************************************************************************\n\t\t\t\t User Menu: Booked Tickets History";
	cout<<"\n********************************************************************************\n";
	if(inFile){
		while(true){
			inFile.read((char*)&T, sizeof(T));
			if(inFile.eof()) break;
			if(!strcmp(T.Uname, Global_User.Uname)){
				T.getBasicTicketDetail(i);
				i++;
			}
		}
		inFile.close();
	}
}

/*Function to convert int to date string*/
char compareDate(char *date, int d,int m, int y){
	short int day=0,month=0,year=0;
	char result='n';
	day=date[0]-48;
	day*=10;
	day+=date[1]-48;
	
	month=date[3]-48;
	month*=10;
	month+=date[4]-48;
	
	year=date[6]-48;
	year*=10;
	year+=date[7]-48;
	
	if(year>y)
		result='y';
	else{
		if(month>m)
			result='y';
		else{
			if(day>d)
				result='y';
			else
				result='n';
		}
	}
	return result;
}

void cancelBookedTicket(){
	int tid, seat_no=-1;
	char date[11], seat_id[3];
	long MovieFilePointer;
	time_t theTime = time(NULL);
	Movie M;
	struct tm *aTime = localtime(&theTime);
	ifstream inFile("Ticket.dat");
	fstream Mfile;
	Ticket T;
	int day = aTime->tm_mday;
	int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
	int year = aTime->tm_year + 1900; // Year is # years since 1900
	char dateCmp='n';
	system("cls");
	cout<<"\n********************************************************************************\n\t\t\t\t User Menu: Cancel Booked Tickets\n";
	cout<<"\n********************************************************************************\n";
	viewBookingHistory_USER();
	cout<<"\n Enter the TID of the Ticket, you wish to Cancel: ";
	cin>>tid;
	
	if(inFile){
		while(true){
			inFile.read((char*)&T, sizeof(T));
			if(inFile.eof()) break;
			
			if(T.TID==tid){
				dateCmp=compareDate(T.Date,day,month,year);
				if(dateCmp!='y')
					cout<<"\n Ticket booked before current date, cannot delete!";
				else
					break;
			}
			else{
				addToFile("Ticket_temp.dat",T);	//Add all tickets with tid not equal to input tid to the temp ticket file
			}
		}
	}
	inFile.close();

	if(dateCmp=='y'){
		remove("Ticket.dat");
		rename("Ticket_temp.dat", "Ticket.dat");
	//Making changes to the Movie.dat file, as seats need to be vacated
		Mfile.open("Movie.dat",ios::in | ios::out);
		while(true){
			MovieFilePointer = Mfile.tellg();
			Mfile.read((char *)&M, sizeof(M));
			if(Mfile.eof()) break;
			if(!strcmp(M.MovieTitle,T.MovieTitle)){			//matching movie title
				for(int i=0;i<ScreeningFrequency;i++){
					if(!strcmp(M.screenDate[i],T.Date)){	//matching screening date
						for(int j=0;j<T.TotalSeats;j++){	//ishan
							seat_no = -1;
							strcpy(seat_id,T.seatNo[j]);
							if(seat_id[0]>='a' && seat_id[0]<='j'){
								if(seat_id[1]>='0' && seat_id[1]<='9'){
									seat_no = seat_id[0]-'a';
									seat_no *= 10;
									seat_no += (seat_id[1] - '0');					
								}
							}
							else if(seat_id[0]>='A' && seat_id[0]<='J'){
								if(seat_id[1]>='0' && seat_id[1]<='9'){
									seat_no = seat_id[0]-'A';
									seat_no *= 10;
									seat_no += (seat_id[1] - '0');					
								}
							}
							else;
							M.SEAT[i][seat_no] = 0;
							--M.SeatsBooked[i];
							++M.SeatsLeft[i];
						}
					}
				}
			break;
			}
		}
		//Now as the changes are made in the Movie object, we overwrite it
		Mfile.seekg(MovieFilePointer);
		Mfile.write((char *)&M, sizeof(M));
		Mfile.close();
		cout<<"\n Ticket Successfully Cancelled. Amount Refunded: "<<T.TotalCost<<'\n';
	}
	else{
		remove("Ticket_temp.dat");
	}
}

/*Movie method to get Basic Detail*/
void Movie::getMovieDetail(int index){
	cout<<"\n"<<index<<". Title: "<<this->MovieTitle<<" | Release Date: "<<this->screenDate[0]<<" | Detail: "<<this->Detail;
	cout<<"\n-----------------------------------------------------------------";
}

/*Movie method to get Full Detail*/
void Movie::getFullMovieDetail(){
	cout<<"\n\tMovie Title: "<<MovieTitle;
	cout<<"\n\tMovie Details: "<<Detail;
	cout<<"\n\tScreening Time (Daily): "<<screeningTime;
	cout<<"\n\tScreening Date\t\tSeats_Available\t\tSeats_Booked";
	for(short int i=0;i<ScreeningFrequency;i++){
		cout<<"\n\t  "<<screenDate[i]<<"\t\t\t"<<SeatsLeft[i]<<"\t\t\t"<<SeatsBooked[i];
	}
	cout<<"\n----------------------------------------------------------------------";
}

/*Movie method to add Movie by Admin*/
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
		cout<<"\tScreening Date (STRICTLY as dd/mm/yy)"<<i+1<<" : ";
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

/*Method actually called by Admin to post a Movie*/
void postNewMovie(){
	Movie Mo;
	Mo.addMovie();
	addToFile("Movie.dat",Mo);
	cout<<"\n Movie added successfully!!";
}

/*Function to Draw Theatre View*/
void drawSeats(Movie selectedMovie, int chooseDate){
	short int i=0,j=0;
	char ch='A';
	system("cls");
	cout<<"\n________________________________________________________________________________";
	cout<<"\n____________________==== S == C == R == E == E == N ====________________________";
	cout<<"\n\n   0\t1\t2\t3\t4\t5\t6\t7\t8\t9\n";
	for(i=0;i<120;i++){
		if(i%10 == 0)
			cout<<'\n'<<" "<<ch++<<" ";
		if(selectedMovie.SEAT[chooseDate][i] == 0)	//when seat is empty output emptyseat
			cout<<EMPTYSEAT<<'\t';
		else
			cout<<BOOKEDSEAT<<'\t';
	}
	cout<<"\n\tEmpty Seats:"<<EMPTYSEAT<<"\t\tBooked Seats:"<<BOOKEDSEAT;
	cout<<"\n________________________________________________________________________________";
}

/*Important Function: for booking ticket by User*/
void bookShow(){
	Ticket T;
	long filePointerPosition;
	short int i=0, choice=0, nos=0, seat_no=-1,chooseDate=0, readCurrentPos;
	char seat_id[3], seat_array[10][3], yorn, successFlag='n';
	Movie tempMovie[10], selectedMovie;
	fstream inFile("Movie.dat", ios::in | ios::out);
	system("cls");
	if(inFile){
		while(true){
			inFile.read((char*)&tempMovie[i], sizeof(Movie));
			if(inFile.eof()) break;
			tempMovie[i].getMovieDetail(i+1);
			++i;
		}
		cout<<"\n Choose the show: ";
		cin>>choice;
		i=0;
		inFile.close();
		inFile.open("Movie.dat",ios::in|ios::out);
		do{
			filePointerPosition = inFile.tellg();
			inFile.read((char*)&selectedMovie, sizeof(selectedMovie));
			++i;
		}while(!inFile.eof() && i<choice);
		
		system("cls");
		cout<<"\n********************************************************************************\n\t\t\t\t User Menu: Movie Details";
		cout<<"\n********************************************************************************\n";

		selectedMovie.getFullMovieDetail();
		cout<<"\n Choose Date of Booking the Show from following:";
	//Choose the show from a particular date	
		for(i=0;i<ScreeningFrequency;i++)
			cout<<"\n\t"<<i+1<<".\t"<<selectedMovie.screenDate[i]<<"\tSeats Avl:"<<selectedMovie.SeatsLeft[i];
		cout<<"\n\t\t-->";
		cin>>chooseDate;
		chooseDate-=1;
	//Check if the selected date is valid
		if(chooseDate>=0 && chooseDate<=ScreeningFrequency){
			drawSeats(selectedMovie, chooseDate);
			cout<<"\n\n Number of Seats to be Booked:";
			cin>>nos;
			if(nos<=selectedMovie.SeatsLeft[chooseDate] && nos>0){
					for(i=0;i<nos;i++){
						seat_no = -1;
						cout<<"\n Seat "<<i+1<<": ";
						cin>>seat_id;
			/* Seat ID is now parsed to find the exact seat no. and weather it is available*/
						if(seat_id[0]>='a' && seat_id[0]<='l'){
							if(seat_id[1]>='0' && seat_id[1]<='9'){
								seat_no = seat_id[0]-'a';
								seat_no *= 10;
								seat_no += (seat_id[1] - '0');	
//								cout<<"\nDEBUG:"<<seat_no<<"\t"<<seat_id;				
							}
						}
						else if(seat_id[0]>='A' && seat_id[0]<='L'){
							if(seat_id[1]>='0' && seat_id[1]<='9'){
								seat_no = seat_id[0]-'A';
								seat_no *= 10;
								seat_no += (seat_id[1] - '0');					
							}
						}
						else{
							cout<<"\n Invalid Seat No. entered";
							--i;
						}
						
						if(selectedMovie.SEAT[chooseDate][seat_no]==1){
							cout<<"\n Sorry! Selected seat is already booked!!";
							--i;
						}
						else if(selectedMovie.SEAT[chooseDate][seat_no]==0 && seat_no!=-1){
							selectedMovie.SEAT[chooseDate][seat_no] = 1;	// Book the seat
							selectedMovie.SeatsBooked[chooseDate]++;
							selectedMovie.SeatsLeft[chooseDate]--;
							strcpy(seat_array[i],seat_id);
							cout<<" Seat "<<seat_id<<" booked. Charge added to the Total!\n";
						}
						else;
					}
			successFlag='y';
			}
			else{
				cout<<"\n\tSorry Seats Unavailable!";
			}
	//Making changes in the Movie.dat to book the selected seats and make them unavailable		
			if(successFlag=='y'){
				cout<<"\n All seats booked... generating reciept for your transaction!";
				T.MakeNewTicket(selectedMovie, seat_array, nos, chooseDate);
				cout<<"\n Confirm Ticket? (Y/N): ";
				cin>>yorn;		//Yes or No
				if(yorn=='y' || yorn=='Y'){
					inFile.seekg(filePointerPosition);
					inFile.write((char *)&selectedMovie, sizeof(selectedMovie));
					addToFile("Ticket.dat",T);
				}
			}
			inFile.close();
			User_MainScreen();
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

/* Give all Movie details to the Admin*/
void viewMovieStats(){
	ifstream inFile("Movie.dat");
	Movie M;
	short int i=1;
	
	system("cls");
	cout<<"\n********************************************************************************\n\t\t\t\tAdmin Menu: Movie Stats\n";
	cout<<"\n********************************************************************************\n";

	while(true){
		inFile.read((char *)&M, sizeof(M));
		if(inFile.eof()) break;
		cout<<"\nMovie::"<<i++;
		M.getFullMovieDetail();
	}
	inFile.close();
}

/*Give All User Booking Details*/
void viewUserStats(){
	ifstream TFile;
	ifstream UFile("User.dat");
	User U;
	Ticket T;
	int i=1, userTransaction=0, costTotal=0;
	char UserName[15], UserEmail[20];
	system("cls");
	cout<<"\n********************************************************************************\n\t\t\t\tAdmin Menu: User Stats\n";
	cout<<"\n********************************************************************************\n";
	while(true){
		UFile.read((char *)&U, sizeof(U));
		if(UFile.eof()) break;
		userTransaction=0;
		costTotal=0;		
		TFile.open("Ticket.dat",ios::in);		
		while(true){
			TFile.read((char *)&T, sizeof(T));
			if(TFile.eof()) break;
			if(!strcmp(U.Uname,T.Uname)){
				++userTransaction;
				costTotal+=T.TotalCost;
			}
		}
		strcpy(UserName,U.Uname);
		strcpy(UserEmail,U.Email);
		TFile.close();
		cout<<"\n User::"<<i++<<"\n\tUser Name: "<<UserName<<"\n\tEmail: "<<UserEmail<<"\n\tNo. of Transactions: "<<userTransaction<<"\n\tTotal Amount:"<<costTotal;
		cout<<"\n----------------------------------------------------------------------";
	}
	UFile.close();
}

/*Function to Delete a Movie*/
void deleteMovie(){
	Movie M;
	char movFound = 'n';	//flag
	ifstream Mfile;
	char movTitle[30];
	system("cls");
	cout<<"\n********************************************************************************\n\t\t\t\t Admin Menu: Delete Movie\n";
	cout<<"\n********************************************************************************\n";
	viewMovieStats();
	cout<<"\n\tEnter Movie Title to Delete it:";
	fflush(stdin);
	gets(movTitle);
	Mfile.open("Movie.dat");
	while(true){
		Mfile.read((char *)&M, sizeof(M));
		if(Mfile.eof()) break;
//		cout<<"\n "<<M.MovieTitle<<"\t"<<movTitle;
		if(!strcmpi(M.MovieTitle, movTitle))
			movFound='y';
		else
			addToFile("Movie_temp.dat",M);
	}
	Mfile.close();
	if(movFound=='y'){
		remove("Movie.dat");
		rename("Movie_temp.dat","Movie.dat");
		cout<<"\n Movie successfully deleted!";
		getchar();
	}
	else{
		remove("Movie_temp.dat");
	}
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
			case 2: deleteMovie();
					break;
			case 3: viewMovieStats();
					break;
			case 4: viewUserStats();
					break;
		}
		fflush(stdin);
		getchar();
	}while(choice!=5);
	getchar();
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

/*Login Menu for All people*/
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
		case 4: break;
		default:
			cout<<"Wrong Input!!";
	}
}	


int main(){
	LoginMenu();
	return 0;
}
