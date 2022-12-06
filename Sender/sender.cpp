#pragma warning(disable: 4996)
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
using std::cin;
using std::cout;
using std::ofstream;
using std::string;
using std::endl;
CRITICAL_SECTION criticalSection;
int main(int argc, char** argv) {
	InitializeCriticalSection(&criticalSection);
	cout << argv[1] << endl;
	wchar_t* wString1 = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, argv[2], -1, wString1, 4096);
	wchar_t* nameEvent = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, "StartSenderEvent", -1, nameEvent, 4096);
	HANDLE ev = OpenEvent(EVENT_MODIFY_STATE, FALSE, wString1);
	HANDLE sEv = OpenEvent(SYNCHRONIZE, FALSE, nameEvent);
	ofstream out;
	out.open(argv[1], ofstream::binary | std::ios_base::app);
	string message;
	SignalObjectAndWait(ev, sEv, INFINITE, FALSE);
	while (true) {
		cout << "Input message or input Z to stop: ";
		cin >> message;
		if (message == "Z") {
			break;
		}
		EnterCriticalSection(&criticalSection);
		message += '\n';
		out.write((const char*)&message, sizeof(string));
		SetEvent(ev);
		LeaveCriticalSection(&criticalSection);
	}
	CloseHandle(sEv);
	out.close();
	CloseHandle(ev);
	return 0;
}