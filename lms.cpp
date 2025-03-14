#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

struct Book {
    string id;
    string title;
    string author;
    int quantity;
    int borrowed;

    void display() const {
        cout << left << setw(10) << id 
             << setw(25) << title 
             << setw(20) << author 
             << setw(10) << quantity 
             << setw(10) << borrowed << endl;
    }
};

struct User {
    string userId;
    string name;
    vector<string> borrowedBooks;

    void display() const {
        cout << left << setw(10) << userId 
             << setw(20) << name 
             << "Books Borrowed: ";
        for (size_t i = 0; i < borrowedBooks.size(); i++)
            cout << borrowedBooks[i] << " ";
        cout << endl;
    }
};

vector<Book> library;
map<string, User> users;

void loadFromFile();
void saveToFile();
void addBook();
void displayBooks();
void searchBook();
void deleteBook();
void borrowBook();
void returnBook();
void registerUser();
void displayUsers();
void deleteUser();
void userBorrowHistory();
void generateReport();
void menu();

int main() {
    loadFromFile();
    menu();
    return 0;
}

void loadFromFile() {
    ifstream file("library.txt");
    if (!file) return;

    Book temp;
    while (file >> temp.id >> temp.title >> temp.author >> temp.quantity >> temp.borrowed) {
        library.push_back(temp);
    }
    file.close();

    ifstream userFile("users.txt");
    if (!userFile) return;

    User tempUser;
    string line;
    while (getline(userFile, line)) {
        stringstream ss(line);
        ss >> tempUser.userId >> tempUser.name;
        tempUser.borrowedBooks.clear();
        string bookId;
        while (ss >> bookId)
            tempUser.borrowedBooks.push_back(bookId);
        users[tempUser.userId] = tempUser;
    }
    userFile.close();
}

void saveToFile() {
    ofstream file("library.txt");
    for (size_t i = 0; i < library.size(); i++) {
        file << library[i].id << " " << library[i].title << " " << library[i].author << " "
             << library[i].quantity << " " << library[i].borrowed << endl;
    }
    file.close();

    ofstream userFile("users.txt");
    for (map<string, User>::iterator it = users.begin(); it != users.end(); ++it) {
        userFile << it->second.userId << " " << it->second.name;
        for (size_t j = 0; j < it->second.borrowedBooks.size(); j++)
            userFile << " " << it->second.borrowedBooks[j];
        userFile << endl;
    }
    userFile.close();
}

void addBook() {
    Book temp;
    cout << "Enter Book ID: ";
    cin >> temp.id;
    cout << "Enter Title: ";
    cin.ignore();
    getline(cin, temp.title);
    cout << "Enter Author: ";
    getline(cin, temp.author);
    cout << "Enter Quantity: ";
    cin >> temp.quantity;
    temp.borrowed = 0;
    library.push_back(temp);
    saveToFile();
    cout << "Book added successfully!\n";
}

void displayBooks() {
    if (library.empty()) {
        cout << "No books available.\n";
        return;
    }
    cout << left << setw(10) << "ID" << setw(25) << "Title" << setw(20) << "Author" << setw(10) << "Stock" << setw(10) << "Borrowed" << endl;
    cout << string(75, '-') << endl;
    for (size_t i = 0; i < library.size(); i++) {
        library[i].display();
    }
}

void borrowBook() {
    string userId, bookID;
    cout << "Enter User ID: ";
    cin >> userId;
    if (users.find(userId) == users.end()) {
        cout << "User not registered.\n";
        return;
    }
    cout << "Enter Book ID to borrow: ";
    cin >> bookID;
    for (size_t i = 0; i < library.size(); i++) {
        if (library[i].id == bookID) {
            if (library[i].quantity <= 0) {
                cout << "Book out of stock!\n";
                return;
            }
            library[i].quantity--;
            library[i].borrowed++;
            users[userId].borrowedBooks.push_back(bookID);
            saveToFile();
            cout << "Book borrowed successfully!\n";
            return;
        }
    }
    cout << "Book not found.\n";
}

void deleteBook() {
    string bookID;
    cout << "Enter Book ID to delete: ";
    cin >> bookID;
    auto it = find_if(library.begin(), library.end(), [&bookID](const Book &book) {
        return book.id == bookID;
    });
    if (it != library.end()) {
        library.erase(it, library.end());
        saveToFile();
        cout << "Book deleted successfully!\n";
    } else {
        cout << "Book not found!\n";
    }
}

void returnBook() {
    string userId, bookID;
    cout << "Enter User ID: ";
    cin >> userId;
    cout << "Enter Book ID to return: ";
    cin >> bookID;
    auto &borrowedBooks = users[userId].borrowedBooks;
    auto it = find(borrowedBooks.begin(), borrowedBooks.end(), bookID);
    if (it != borrowedBooks.end()) {
        borrowedBooks.erase(it);
        for (auto &book : library) {
            if (book.id == bookID) {
                book.quantity++;
                book.borrowed--;
                saveToFile();
                cout << "Book returned successfully!\n";
                return;
            }
        }
    }
    cout << "Book return failed.\n";
}


void displayUsers() {
    if (users.empty()) {
        cout << "No registered users.\n";
        return;
    }
    for (const auto &user : users) {
        user.second.display();
    }
}

void deleteUser() {
    string userId;
    cout << "Enter User ID to delete: ";
    cin >> userId;
    if (users.erase(userId)) {
        saveToFile();
        cout << "User deleted successfully!\n";
    } else {
        cout << "User not found.\n";
    }
}

void userBorrowHistory() {
    displayUsers();
}

void generateReport() {
    cout << "Library Report:\n";
    displayBooks();
    displayUsers();
}

void registerUser() {
    User newUser;
    cout << "Enter User ID: ";
    cin >> newUser.userId;
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, newUser.name);
    users[newUser.userId] = newUser;
    saveToFile();
    cout << "User registered successfully!\n";
}

void menu() {
    int choice;
    
    while (true) {
    	cout<<"\n***************************************\n";
    	cout<<" ***** Library Management System *****\n";
    	cout<<"***************************************\n";
        cout << "\n\n1. Register User\n2. Add Book\n3. Display Books\n4. Borrow Book\n5. Return Book\n6. User Borrow History\n7. Generate Report\n8. Delete User\n9. Display User\n10.Delete Book\n11.Exit\n\nEnter choice: ";
        cin >> choice;
        if (choice == 11) return;
        switch (choice) {
            case 1: registerUser(); break;
            case 2: addBook(); break;
            case 3: displayBooks(); break;
            case 4: borrowBook(); break;
            case 5: returnBook(); break;
            case 6: userBorrowHistory(); break;
            case 7: generateReport(); break;
            case 8: deleteUser(); break;
            case 9: displayUsers(); break;
            case 10: deleteBook(); break;
            default: cout << "Invalid choice!\n";
        }
    }
}
