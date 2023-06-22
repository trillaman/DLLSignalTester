#include <iostream>
#include <windows.h>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>


#define MAX_LENGTH 300

typedef void(__stdcall* f_DisplayHelloWorld)();

int invokeProcess(char* pathToExe) {
	WCHAR* execmd;
	execmd = new WCHAR[MAX_LENGTH];
	MultiByteToWideChar(CP_ACP, 0, pathToExe, -1, execmd, MAX_LENGTH);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(LPCWSTR(execmd), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		printf("CreateProcess failed (%d).\n", GetLastError());
		return 1;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;

}

int invokeProcessWithDummyParameters(char* pathToExe, std::vector<std::string> list) {
	for (int i = 0; i < list.size(); i++) {
		std::cout << list[i];
	}
	WCHAR* execmd;
	execmd = new WCHAR[MAX_LENGTH];
	MultiByteToWideChar(CP_ACP, 0, pathToExe, -1, execmd, MAX_LENGTH);
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcess(LPCWSTR(execmd), NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
		printf("CreateProcess failed (%d).\n", GetLastError());
		return 1;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}

char* initArrayAndReturnIt(char* array, int max_length) {
	for (int i = 0; i < max_length; i++) {
		array[i] = '\0';
	}
	return array;
}

int loadLibrary(char* argvDllPath) {
	HINSTANCE hGetProcIDLL = NULL;

	argvDllPath[MAX_LENGTH - 1] = '\0';
	hGetProcIDLL = LoadLibraryA(LPCSTR(argvDllPath));

	if (hGetProcIDLL != NULL) {
		f_DisplayHelloWorld DisplayHelloWorld = (f_DisplayHelloWorld)GetProcAddress(hGetProcIDLL, "DLL_DisplayHelloWorld");
		if (DisplayHelloWorld != NULL) {
			DisplayHelloWorld();
		}
		else {
			printf("Error: %d", GetLastError());
			return 1;
		}
	}
	else {
		printf("Error: %d", GetLastError());
		return 1;
	}

	return 0;
}

void showUsage() {
	printf("Usage:\nLoading DLL: program.exe -load <path to dll>\nInvoking process: program.exe -fork <path to exe>\n");
}

int main(int argc, char** argv)
{
	char argvDllPath[MAX_LENGTH];
	char processName[MAX_LENGTH];
	HINSTANCE hGetProcIDLL = NULL;
	setlocale(LC_ALL, "");

	if (argc == 1) {
		showUsage();
		return EXIT_FAILURE;
	}

	if (std::string(argv[1]) == "-fork") {
		if (argc == 3) {
			strcpy_s(processName, argv[2]);
			printf("Forking process: %s\n", processName);
			invokeProcess(processName);
		}
		else if (argc > 3) {
			std::vector<std::string> list;
			for (int i = 3; i < argc; i++) {
				std::string tmp(argv[i]);
				list.push_back(tmp); // add all arguments to the vector
			}

			strcpy_s(processName, argv[2]);
			printf("Forking process: %s\n", processName);
			invokeProcessWithDummyParameters(processName, list);
		}
		else {
			printf("Not enough arguments\n");
			return 1;
		}
	}
	else if (std::string(argv[1]) == "-load") {
		if (argc == 3) {
			if (strlen(argv[1]) > MAX_LENGTH) {
				printf("Path argument is too long. Max length is %d", MAX_LENGTH);
				return EXIT_FAILURE;
			}
			else {
				printf("\nPath you entered for dll is %s\n\n", argv[2]);
				strcpy_s(argvDllPath, sizeof argvDllPath, argv[2]);
				loadLibrary(argvDllPath);
			}
		}
		else if (argc == 2) {
			printf("Please enter the path for dll: ");
			if (scanf_s("%s", argvDllPath, MAX_LENGTH) != 1) {
				printf("Path should not be longer than %d", MAX_LENGTH);
				return EXIT_FAILURE;
			}
			else {
				loadLibrary(argvDllPath);

			}

		}

	}
	return EXIT_SUCCESS;
}