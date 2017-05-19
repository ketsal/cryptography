#include <windows.h>
#include <conio.h>
#include <tlhelp32.h>
#include <iostream>
#include <psapi.h> 
#include <ctime>
#include <map>
#include <algorithm>
#include <iterator>
class TaskDisplay
{
private:
	HANDLE CONST _hStdOut;
	std::map <DWORD,std::string> _Snapshot;
	std::map  <DWORD, std::string> _OldSnapshot;
	VOID PrintUpdatedProcess(std::map <DWORD, std::string> diff)
	{
		DWORD dwtemp;
		TCHAR Buffer[1024];
		for (auto itMap = diff.begin(); itMap != diff.end(); itMap++)
		{
			if (_Snapshot.count(itMap->first) > 0)
			{
				wsprintf(Buffer, "Opened: %08X %s ", itMap->first, itMap->second.c_str());
				WriteConsole(_hStdOut, &Buffer, lstrlen(Buffer), &dwtemp, NULL);
				ProcessPath(itMap->first);
			}
			if (_OldSnapshot.count(itMap->first) > 0)
			{
				wsprintf(Buffer, "Closed: %08X %s ", itMap->first, itMap->second.c_str());
				WriteConsole(_hStdOut, &Buffer, lstrlen(Buffer), &dwtemp, NULL);
				ProcessPath(itMap->first);
			}
		}
	}
	VOID ProcessPath(DWORD ProcessId)
	{	
		DWORD dwtemp;
		TCHAR Buffer[MAX_PATH];
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, ProcessId);
		if (GetModuleFileNameEx(hProcess, NULL, Buffer, MAX_PATH) == 0)
		{
			wsprintf(Buffer, "Couldn't find directory \n");
		}
		else
		{
			wsprintf(Buffer, "%s \n",Buffer);
		}
		WriteConsole(_hStdOut, &Buffer, lstrlen(Buffer), &dwtemp, NULL);
		CloseHandle(hProcess);
	}
public:
	TaskDisplay(HANDLE CONST hStdOut) :_hStdOut(hStdOut)
	{

	}
	VOID GetProcessList() 
	{
		PROCESSENTRY32 peProcessEntry;
		HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapshot)
			return;
		peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
		Process32First(hSnapshot, &peProcessEntry);
		do
		{
			_Snapshot.insert(std::pair<DWORD, std::string>(peProcessEntry.th32ProcessID, peProcessEntry.szExeFile));
		} while (Process32Next(hSnapshot, &peProcessEntry));
		CloseHandle(hSnapshot);
	}
	VOID Refresh()
	{
		_OldSnapshot=_Snapshot;
		GetProcessList();
		//_SnapShot.clear();
		std::map  <DWORD, std::string> difference;
		std::set_symmetric_difference(_OldSnapshot.begin(), _OldSnapshot.end()
			, _Snapshot.begin(), _Snapshot.end()
			, inserter(difference, difference.end()));
		PrintUpdatedProcess(difference);
	}
};

int main(int argc, char* argv[])
{
	HANDLE CONST hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	TaskDisplay MyTaskDisplay(hStdOut);
    MyTaskDisplay.GetProcessList();
	bool cont=true;
	while (cont)
	{
		Sleep(100);
        MyTaskDisplay.Refresh();
	}
}
