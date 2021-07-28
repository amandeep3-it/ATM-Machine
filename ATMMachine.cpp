#include <iostream>

#include <conio.h>                  // Library to access Getch
#include <fstream>                  // File Reader/Writer
#include <math.h>
#include <windows.h>                // Window Console controller

using namespace std;

struct User {
    bool loggedin = false;
    string username = "", password = "";
    float balance = 0;
};

void MainMenu();
void ATMMenu();
void Login();
User getAccount(string user, string password);
bool CreateAccount();
void DepositMoney();
void WithdrawMoney();
void UpdateBalance();
void RequestBalance();

User user;

/*
cls && g++ ATMMachine.cpp -o ATMMachine && ATMMachine
*/

int main() {
    cout << "Hi! Welcome to the ATM Machine. " << endl << endl;
    Sleep(1000);

	MainMenu();

    cout << endl << "---------- THANKS FOR VISITING US. KEEP IN TOUCH ----------" << endl << endl;
    Sleep(3000);

    return 0;
}

void MainMenu() {
    bool outer_loop_run = true;
    while (outer_loop_run) {
        outer_loop_run = false;
        system("cls");
        cout << "Please select an option from the menu below." << endl << endl;
        cout << "---------- Intro Menu ----------" << endl << endl;
        Sleep(500);
        cout << "   l -> Login" << endl;
        cout << "   c -> Create a New Account" << endl;
        cout << "   q -> Quit" << endl << endl;

        while (true) {
            cout << "Enter your option: ";
            const char option = getch();
            cout << option << endl;

            switch (option) {
                case 'l':
                    Login();
                    outer_loop_run = true;
                break;

                case 'c':
                    if (CreateAccount()) Login();
                    outer_loop_run = true;
                break;

                case 'q': return;
                break;

                default:
                    cout << "Please insert a valid input" << endl;
                    continue;
                break;
            }
            break;
        }
    }
}

void ATMMenu() {
    bool outer_loop_run = true;
    while (outer_loop_run) {
        outer_loop_run = false;
        system("cls");
        cout << "---------- Main Menu ----------" << endl << endl;
        Sleep(500);
        cout << "   d -> Deposit Money" << endl;
        cout << "   w -> Withdraw Money" << endl;
        cout << "   r -> Request Balance" << endl;
        cout << "   l -> Log out" << endl;

        while (true) {
            cout << "Enter your option: ";
            const char option = getch();
            cout << option << endl;

            switch (option) {
                case 'd':
                    DepositMoney();
                    outer_loop_run = true;
                break;

                case 'w':
                    WithdrawMoney();
                    outer_loop_run = true;
                break;

                case 'r':
                    RequestBalance();
                    outer_loop_run = true;
                break;

                case 'l':
                    user.loggedin = false;
                    user.username = "";
                    user.password = "";
                    user.balance = 0;
                break;

                default:
                    cout << "Please insert a valid input" << endl;
                    continue;
                break;
            }
            break;
        }
    }
}

void Login() {
    while (true) {
        string username, password;
        system("cls");
        cout << "---------- LOGIN ----------" << endl << endl;
        cout << "Please enter your id: ";
        cin >> username;
        cout << "Please enter your password: ";
        cin >> password;

        User _user = getAccount(username, password);
        if (_user.loggedin) {
            user = _user;
            cout << endl << "---------- SUCCESSFULLY LOGGED IN ----------" << endl << endl;
            Sleep(1000);
            ATMMenu();
            return;
        } else {
            cout << endl << "---------- LOGIN FAILED : Try Again ----------" << endl << endl;
            Sleep(1000);
        }
    }
}

User getAccount(string username, string password) {
    User _user;
    fstream database("database.txt", fstream::in);
	if (database.is_open()) {
        string line;
        while (getline(database, line)) {
            if (line == "") continue;
            string p;
            getline(database, p);
            if ((line == username) && (p == password)) {
                string b;
                getline(database, b);
                _user.balance = stod(b);
                _user.loggedin = true;
                _user.username = line;
                _user.password = p;
            }
        }
        database.close();
    }
    return _user;
}

bool CreateAccount() {
    while (true) {
        string username, password;
        system("cls");
        cout << "---------- CREATE YOUR ACCOUNT! ----------" << endl << endl;
        cout << "Please create your user id: ";
        cin >> username;
        cout << "Please create your password: ";
        cin >> password;

        User _user = getAccount(username, password);
        if (_user.loggedin) {
            cout << endl << "---------- Creating New Account Failed ----------" << endl;
            cout << "Try creating account again? (y/n)" << endl;

            while (true) {
                cout << endl << "Enter your option: ";
                const char option = getch();
                cout << option << endl;

                switch (option) {
                    case 'y': break;
                    break;

                    case 'n': return false;
                    break;

                    default:
                        cout << "Please insert a valid input" << endl;
                        continue;
                    break;
                }
                break;
            }
            continue;
        } else {
            fstream database("database.txt", (fstream::in | fstream::out | fstream::app));
            if (database.is_open()) {
                database << username << "\n" << password << "\n100\n\n";
                database.close();

                cout << endl << "---------- CONGRATULATION! YOUR ACCOUNT HAS BEEN CREATED ----------" << endl;
                Sleep(1000);
                return true;
            }
        }
    }
}

void DepositMoney() {
    system("cls");
	cout << "Amount of deposit: $";
    float amount_deposit;
	cin >> amount_deposit;
    amount_deposit = roundf(amount_deposit * 100) / 100;

    user.balance += amount_deposit;
    user.balance = roundf(user.balance * 100) / 100;
    UpdateBalance();

	cout << endl << "---------- You have successfully deposited $" << amount_deposit << " ----------" << endl << endl;
    system("pause");
}

void WithdrawMoney() {
    system("cls");
    cout << "Amount of withdrawal: $";
    float amount_withdrawal;
    cin >> amount_withdrawal;
    amount_withdrawal = roundf(amount_withdrawal * 100) / 100;

    if (amount_withdrawal <= user.balance) {
        user.balance -= amount_withdrawal;
        user.balance = roundf(user.balance * 100) / 100;
        UpdateBalance();
        cout << endl << "---------- You have successfully withdrawed $"<< amount_withdrawal << " ----------" << endl << endl;
        system("pause");
    } else {
        cout << endl << "---------- The amount exceeds the balance ----------" << endl << endl;
        system("pause");
    }
}

void UpdateBalance() {
    // Get Records and Update Balance
    fstream database("database.txt", (fstream::in | fstream::out | fstream::app));
	string file = "";
    if (database.is_open()) {
        string line;
        while (getline(database, line)) {
            if (line == "") continue;
            file += line + "\n";
            string p;
            getline(database, p);
            file += p + "\n";
            if ((line == user.username) && (p == user.password)) {
                file += to_string(user.balance).substr(0, to_string(user.balance).find(".") + 2 + 1) + "\n\n";
                getline(database, line);
            }
        }
        database.close();
    } else return;

    file = file.substr(0, (file.length() - 2));

    // Clear Records
    ofstream clear("database.txt", (ofstream::out | ofstream::trunc));
    clear.close();
    
    // Upload new Records
    database.open("database.txt", (fstream::in | fstream::out | fstream::app));
    if (database.is_open()) {
        database << file;
        database.close();
    }
}

void RequestBalance() {
    system("cls");
    cout << "---------- YOUR BALANCE IS $" << user.balance << " ----------" << endl << endl;
    system("pause");
}