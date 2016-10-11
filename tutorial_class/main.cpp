#include <iostream>
#include <fstream>
#include <Windows.h>
#define GMEXPORT extern "C" __declspec (dllexport)

GMEXPORT double registerURIScheme(wchar_t scheme[]) {

	char exePathBuffer[MAX_PATH * 2];
	GetModuleFileNameA(NULL, exePathBuffer, MAX_PATH * 2);

	char* cmd = GetCommandLineA();

	std::ofstream myfile;
	myfile.open("debug.txt");
	myfile << exePathBuffer << "\n" << cmd << "\n";
	myfile.close();

	return 1;

}
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