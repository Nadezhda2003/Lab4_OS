#pragma warning(disable : 4996)
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <conio.h>
#include <process.h>
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::fstream;
using std::string;
using std::to_string;
CRITICAL_SECTION criticalSection;
int main() {
	cout << "Input filename:" << endl;
	char filename[100];
	int numOfEnters;
	cin >> filename;
	cin >> numOfEnters;
	int numOfSenders;
	cin >> numOfSenders;
	InitializeCriticalSection(&criticalSection);
	std::ofstream out;
	out.open(filename, fstream::binary);
	out.close();
	return 0;
}