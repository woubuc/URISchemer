// Not sure how many of these I still need cause I copied a lot of tutorials and shit but that doesn't really matter at this point
#include <iostream>
#include <fstream>
#include<stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <Windows.h>

// This is needed according to the GM DLL tutorial
#define GMEXPORT extern "C" __declspec (dllexport)

GMEXPORT double registerURIScheme(wchar_t scheme[]) {

	wchar_t exePathBuffer[MAX_PATH * 2];
	// GetModuleFileNameA(NULL, exePathBuffer, MAX_PATH * 2);
	GetModuleFileName(NULL, exePathBuffer, MAX_PATH * 2);

	// Output file for debugging this cause I don't know how else I would do it
	std::ofstream myfile;
	myfile.open("debug.txt");
	myfile << "PATH: " << exePathBuffer << "\n";


	// So this works, but it only opens the exe, it doesn't use the CLI args
	ShellExecute(NULL, L"runas", exePathBuffer, NULL, NULL, SW_SHOWNORMAL);

	// There's this charming little fella who looks like he could help me out here
	// But I haven't managed to get anywhere with this
	char* cmd = GetCommandLineA();

	// Test code - this works fine
	// ShellExecute(NULL, L"runas", L"D:\\Program Files (x86)\\Notepad++\\notepad++.exe", L"C:\\Windows\\System32\\drivers\\etc\\hosts", NULL, SW_SHOWNORMAL);

	// Don't forget to close the file
	myfile.close();


	return 1;

}

// This is my other test code which I separated cause I'm focusing on the top bit currently
GMEXPORT double test(wchar_t scheme[]) {


	// Prepare DefaultIcon path
	wchar_t schemeDefaultIcon[48] = L"";
	wcsncpy_s(schemeDefaultIcon, scheme, 32);
	wcscat_s(schemeDefaultIcon, L"\\DefaultIcon");
	
	// Prepare Shell command path
	wchar_t schemeOpenCommand[64] = L"";
	wcsncpy_s(schemeOpenCommand, scheme, 32);
	wcscat_s(schemeOpenCommand, L"\\shell\\open\\command");

	// Prepare description
	wchar_t schemeDescription[52] = L"URL:";
	wcsncat_s(schemeDescription, scheme, 32);
	wcscat_s(schemeDescription, L" Protocol");

	// Prepare vars
	wchar_t applicationPath[] = L"D:\\Gamemaker\\URISchemerTest\\URISchemerTest.exe"; // Hard-coded for now cause I have to figure this out later
	bool success = false;
	HKEY hKey = NULL;

	// Check if a URI scheme handler already exists
	// Because if one exists, we shouldn't overwrite it
	long urlProtocol = RegOpenKeyEx(HKEY_CLASSES_ROOT, scheme, 0, KEY_READ, &hKey);

	// If it doesn't exists, create it
	if(urlProtocol == ERROR_NO_MATCH || urlProtocol == ERROR_FILE_NOT_FOUND){

		// Create the key
		if (RegCreateKeyEx(HKEY_CLASSES_ROOT, scheme, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) == ERROR_SUCCESS) {

			// Set default value (description)
			long setDescription = RegSetValueEx(hKey, scheme, 0, REG_SZ, (LPBYTE)schemeDescription, wcslen(schemeDescription)*sizeof(char));

			// Set URL protocol empty value which is required for URI schemes for some reason
			long setUrlProtocol = RegSetValueEx(hKey, TEXT("URL Protocol"), 0, REG_SZ, (LPBYTE)"", 0);

			// Did all of that work?
			if (setDescription == ERROR_SUCCESS && setUrlProtocol == ERROR_SUCCESS) {
				
				// So let's try stopping here. If I'm correct this should already create the first required registry entry.
				// Spoiler alert: it doesn't :(

				success = true;

			}
		}
	}

	
	// Return 1 if it was added to the registry successfully, 0 if not
	if (success == true) {
		double output = 1;
		return output;
	}
	else {
		double output = 0;
		return output;
	}
}