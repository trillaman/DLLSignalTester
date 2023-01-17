#include <iostream>
#include <windows.h>
#include <string>
#include <stdio.h>

#define MAX_LENGTH 300

typedef void(__stdcall* f_DisplayHelloWorld)();

int main(int argc, char **argv)
{
    char argvDllPath[MAX_LENGTH];
	HINSTANCE hGetProcIDLL = NULL;
    setlocale(LC_ALL, "");

    if (argc == 2) {
		if (strlen(argv[1]) > MAX_LENGTH){
			printf("Path argument is too long. Max length is %d", MAX_LENGTH);
			return EXIT_FAILURE;
		}
		else {
			printf("\nPath you entered for dll is %s\n\n", argv[1]);
			strcpy_s(argvDllPath, sizeof argvDllPath, argv[1]);
		}
	}
	else if (argc == 1) {
		printf("Please enter the path for dll: ");
		if (scanf_s("%s", argvDllPath, MAX_LENGTH) != 1) {
			printf("Path should not be longer than %d", MAX_LENGTH);
			return EXIT_FAILURE;
		}
	}
	else {
		printf("Not recognized number of parameters\nUsage: program.exe <path to DLL>");
		return EXIT_FAILURE;
	}
	
	argvDllPath[MAX_LENGTH - 1] = '\0';
	
	hGetProcIDLL = LoadLibraryA(argvDllPath);

	if (hGetProcIDLL != NULL) {
		f_DisplayHelloWorld DisplayHelloWorld = (f_DisplayHelloWorld)GetProcAddress(hGetProcIDLL, "DLL_DisplayHelloWorld");
		if (DisplayHelloWorld != NULL) {
			DisplayHelloWorld();
		}
		else {
			printf("Error: %d", GetLastError());
		}
	}
	else {
		printf("Error: %d", GetLastError());
	}

    return EXIT_SUCCESS;
}