#include <iostream>
#include <fstream>
using namespace std;

bool isUserRegistered(const string& id, const string& name) { // Verify User
    ifstream file("users.txt");
    string line;
    while (getline(file, line)) {
        if (line == id + "," + name) {  
            return true;
        }
    }
    return false;
}

bool isBookAvailable(const string& isbn) {  // If Book is available
    ifstream file("book.txt");
    string line;

    while (getline(file, line)) {
        string fileISBN = line.substr(0, line.find(',')); 
        if (fileISBN == isbn) return true;
    }
    return false;
}

class Book {  // Book Class
	private: 
	string title, isbn, author;
	bool available;
	
	public:
		Book (){}
		Book(string t, string a, string i, bool avail = true ){
			title = t;
			isbn = i;
			author = a;
			available = avail;
		}
		
	string getISBN() { return isbn; }
    string getTitle() { return title; }
    bool isAvailable() { return available; }
    
void saveBooks(){      // Save Books
    ofstream bookOut ("book.txt", ios:: app);
    bookOut << isbn << "," << title << "," << author << "," << " Available: "<< available << endl;
    bookOut.close();
	}
		
void displayBooks() {    // Display Books
    ifstream bookIn("book.txt");
    string line;
    cout << "\n--- Book List ---\n";
    while (getline(bookIn, line)) {
    cout << line << endl; 
        }
    bookIn.close();
    }
				
};

class LibraryUser {  // Library User Class
	private :
		string userId, name, title;
	public:
		
	LibraryUser(){}
	LibraryUser(string id, string n){
		userId = id;
		name = n;
	}

	string getUserID(){return userId;}
	string getName() { return name; }

	
void saveUser() {   // Save USer
    ofstream saveUser("users.txt", ios::app);
    saveUser << userId << "," << name << endl;
    saveUser.close();
    }
    
void borrowBook(const string& isbn) {  // Borrow a book
if (!isBookAvailable(isbn)) {
    cout << "Book with ISBN " << isbn << " does not exist in library." << endl;
    return;
    }

    ofstream borrowFile("borrowed.txt", ios::app);
    borrowFile << userId << " " << isbn << endl;   
    cout << "Book " << isbn << " borrowed successfully." << endl;

}
	
void returnBook(string isbn) {  // To return a book
    
    ifstream borrowIn("borrowed.txt");
    ofstream tempFile("temp.txt");
    string line;
    bool found = false;

    while (getline(borrowIn, line)) {
        if (line == userId + " " + isbn && !found) {
            found = true; 
            continue;
        }
        tempFile << line << endl;
    }

    borrowIn.close();
    tempFile.close();

    remove("borrowed.txt");
    rename("temp.txt", "borrowed.txt");

    if (found)
        cout << name << " returned book with ISBN: " << isbn << endl;
    else
        cout << "Book not found in borrowed list." << endl;
}
};


class LibraryStaff {  // Library Staff Class
	public: 
	void addNewBooks(){  // Add a new books
		string t, i , a;
		cin.ignore();
		cout << "Enter Title: ";
		getline (cin, t);
	
		cout << ("Enter Author: ");
		getline (cin, a);
	
		cout << "Enter ISBN: ";
		getline(cin, i);
		Book b(t, a, i);
        b.saveBooks();
        cout << "Book added.\n";
	}

    void registerUser() { // Register a User
        string id, n;
        cin.ignore();
        cout << "Enter UserID: ";
        getline(cin, id);
        cout << "Enter Username: ";
        getline(cin, n);

        LibraryUser u(id, n);
        u.saveUser();
        cout << "User registered." << endl;
    }

   void removeBook() {   // Remove existing Book
    string isbnToRemove, line;
    cin.ignore();
    cout << "Enter ISBN of book to remove: ";
    getline(cin, isbnToRemove);

    ifstream bookIn("book.txt");
    ofstream temp("temp.txt");
    bool found = false;

    while (getline(bookIn, line)) {
        string fileISBN = line.substr(0, line.find(','));
        if (fileISBN != isbnToRemove) temp << line << endl;
        else found = true;
    }

    bookIn.close();
    temp.close();

    remove("book.txt");
    rename("temp.txt", "book.txt");

    if (found) cout << "Book removed successfully!\n";
    else cout << "Book not found.\n";
}

 void removeUser() {  // Remove existing User
    string userIdToRemove, line;
    cin.ignore(); 
    cout << "Enter User ID to remove: ";
    getline(cin, userIdToRemove);

    ifstream userIn("users.txt");
    ofstream temp("temp.txt");
    bool found = false;

    while (getline(userIn, line)) {
        if (line.rfind(userIdToRemove + ",", 0) != 0) {
                temp << line << endl;
        } else {
            found = true;
        }
    }

    userIn.close();
    temp.close();

    remove("users.txt");
    rename("temp.txt", "users.txt");

    if (found)
        cout << "User with ID " << userIdToRemove << " removed successfully!\n";
    else
        cout << "User not found.\n";
    }	
	
};
 
int main(){
	
	LibraryStaff lib;
	LibraryUser libUser;
	Book b;

	int choice, choiceMethodLibStaff, choiceMethodLibUser;
	char verifyContinue;
	
	cout << "Welcome to Library System" << endl;
	// Menu
	do{
	cout << "[ 1 ] LibraryUser" << endl;
	cout << "[ 2 ] Library Staff" << endl;
	cout << "[ 3 ] Log Out" << endl;
	cout << "Choose: ";
	cin >> choice;
	
		
	if (choice == 1) {  // Library User login
    string id, name;
    cin.ignore();
    cout << "Enter your User ID: ";
    getline(cin, id);
    cout << "Enter your Name: ";
    getline(cin, name);

    if (!isUserRegistered(id, name)) { // if the user not registered
        cout << "User not registered. Please contact library staff." << endl;
    } else {
        libUser = LibraryUser(id, name); // if the user is registered
        cout << "Welcome " << name << "! You can now borrow/return books.\n";

do {
    cout << "\n--- Library User Menu ---" << endl; // menu Library User
    cout << "[ 1 ] Borrow Book" << endl;
    cout << "[ 2 ] Return Book" << endl;
    cout << "[ 3 ] Exit" << endl;
    cout << "Select Method: ";
    cin >> choiceMethodLibUser;
    
if (choiceMethodLibUser == 1) {
    string isbn;
    b.displayBooks();
    cout << "Enter ISBN of book to borrow: ";
    cin >> isbn;
    libUser.borrowBook(isbn);
} else if (choiceMethodLibUser == 2) {
    string isbn;
    cout << "Enter ISBN of book to return: ";
    cin >> isbn;
    libUser.returnBook(isbn);
  }
  else if (choiceMethodLibUser < 1 || choiceMethodLibUser > 3){
    cout <<"[ Please Try Again ]" << endl;
  }

 } while (choiceMethodLibUser != 3);
}
    }
    
else if(choice == 2){  // Library Staff Section
	    cin.ignore();
		do{
    cout << endl;
    cout << "Library Staff Menu" << endl;
    cout << "[ 1 ] Add new Books" << endl;
    cout << "[ 2 ] Remove Books" << endl;
    cout << "[ 3 ] Display All Books" << endl;
    cout << "[ 4 ] Register User" << endl;
    cout << "[ 5 ] Remove User" << endl;
    cout << "[ 6 ] Exit" << endl;	
    cout << "Select : ";
    cin >> choiceMethodLibStaff;	
	
		
    if (choiceMethodLibStaff == 1 ){
	    lib.addNewBooks();
    }else if (choiceMethodLibStaff == 2){
	    lib.removeBook();
		
    }else if (choiceMethodLibStaff == 3){
	    b.displayBooks();	
			
    }else if (choiceMethodLibStaff == 4){
		    lib.registerUser();
			 	
    }else if (choiceMethodLibStaff == 5){
	    lib.removeUser();	

    }else if(choiceMethodLibStaff < 1 || choiceMethodLibStaff > 6 ){
    cout <<"[ Please Try Again ]" << endl;
    }
	    }while(choiceMethodLibStaff != 6);	
        
    }else if (choice < 1 || choice > 3){
        cout << "[ Please Try Again ]" <<endl;
    }
				
	}while(choice != 3);

    cout << "Thank You Come Again! " << endl;
	
	return 0;
}
