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

	STARTUPINFO* si = new STARTUPINFO[numOfSenders];
	PROCESS_INFORMATION* pi = new PROCESS_INFORMATION[numOfSenders];
	HANDLE* sEvent = new HANDLE[numOfSenders];
	HANDLE* senders = new HANDLE[numOfSenders];
	SECURITY_ATTRIBUTES secureA = { sizeof(SECURITY_ATTRIBUTES), 0, TRUE };
	string eve = "SenderStartEvent";
	const char* Eve = eve.c_str();
	wchar_t* wEve = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, Eve, -1, wEve, 4096);
	HANDLE evToStart = CreateEvent(&secureA, FALSE, FALSE, wEve);
	for (int i = 0; i < numOfSenders; i++) {
		string arg = "Sender.exe";
		arg = arg + " " + filename;
		ZeroMemory(&si[i], sizeof(STARTUPINFO));
		si[i].cb = sizeof(STARTUPINFO);
		ZeroMemory(&pi[i], sizeof(PROCESS_INFORMATION));
		string argm = "Event";
		argm = argm + " " + to_string(i);
		const char* Argm = argm.c_str();
		wchar_t* wArgm = new wchar_t[4096];
		MultiByteToWideChar(CP_ACP, 0, Eve, -1, wArgm, 4096);
		sEvent[i] = CreateEvent(&secureA, FALSE, FALSE, wArgm);
		arg = arg + " Event " + to_string(i);
		const char* Arg = arg.c_str();
		wchar_t* wArg = new wchar_t[4096];
		MultiByteToWideChar(CP_ACP, 0, Arg, -1, wArg, 4096);
		if (!CreateProcess(NULL, wArg, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si[i], &pi[i])) {
			cout << GetLastError();
			ExitProcess(0);
		}
		senders[i] = pi[i].hProcess;
	}
	WaitForMultipleObjects(numOfSenders, sEvent, TRUE, INFINITE);
	SetEvent(evToStart);

	while (WaitForMultipleObjects(numOfSenders, senders, TRUE, 0) == WAIT_TIMEOUT) {
		cout << "Input Z for trying to read file: ";
		string text;
		cin >> text;
		if (text.compare("Z") != 0) {
			break;
		}
		ifstream in;
		in.open(filename, fstream::binary);
		string message;
		int i = 0;
		while (!in.eof()) {
			in.read((char*)&message, sizeof(string));
			cout << message;
			i++;
		}
		in.close();
		LeaveCriticalSection(&criticalSection);
	}

	for (int i = 0; i < numOfSenders; i++) {
		CloseHandle(pi[i].hThread);
		CloseHandle(pi[i].hProcess);
		CloseHandle(sEvent[i]);
	}
	CloseHandle(evToStart);
	return 0;
}