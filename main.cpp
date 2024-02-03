#include <iostream>
#include <string>
#include "multiuser.h"

using namespace std;

int main() {
	cout << "Are you running the server or the client?" << endl << endl;
	cout << "\t1. Server" << endl
		<< "\t2. Client" << endl << endl;

	string charServ{};
	do {
		cout << "Enter your selection: ";
		getline(cin, charServ);
		if (charServ != "1" && charServ != "2") {
			cout << "\nEnter a valid selection!" << endl << endl;
		}
		else {
			break;
		}
	} while (true);

	system("cls");

	Multiuser multiuse;

	if (charServ == "1") {
		multiuse.startServer();
	}
	else {
		multiuse.startClient();
	}

	return -1;
}