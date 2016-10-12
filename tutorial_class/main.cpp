// Much thanks to the awesome tutorial at http://gmc.yoyogames.com/index.php?showtopic=405812

#include <fstream>
#include <Windows.h>

#define GMEXPORT extern "C" __declspec (dllexport)


// Relaunch the application with admin rights
GMEXPORT double requestAdminRights() {

	// Get current filename & path
	wchar_t applicationPath[MAX_PATH * 2];
	GetModuleFileName(NULL, applicationPath, MAX_PATH * 2);

	// Get cli command and arguments
	wchar_t* cmd = GetCommandLine();

	// Extract cli arguments
	wchar_t* cmdParams;
	wchar_t separators[] = L" ";
	wchar_t* cmdCommand = wcstok_s(cmd, separators, &cmdParams);

	// Relaunch the .exe as admin (asks for admin rights if they weren't granted already)
	ShellExecute(NULL, L"runas", applicationPath, cmdParams, NULL, SW_SHOWNORMAL);

	return 1;
}

// Register the URI scheme
GMEXPORT double registerURIScheme(wchar_t scheme[]) {

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


	// Get current filename & path
	wchar_t applicationPath[MAX_PATH * 2];
	GetModuleFileName(NULL, applicationPath, MAX_PATH * 2);


	// Prepare vars
	bool success = false;
	HKEY hKey = NULL;

	std::ofstream debug;
	debug.open("debug.txt");
	debug << "SCHEME: " << scheme << "\n";
	debug << "DEFAULTICON: " << &schemeDefaultIcon << "\n";
	debug << "OPENCOMMAND: " << &schemeOpenCommand << "\n";
	debug << "DESCRIPTION: " << &schemeDescription << "\n";

	// Check if a URI scheme handler already exists
	// Because if one exists, we shouldn't overwrite it
	long urlProtocol = RegOpenKeyEx(HKEY_CLASSES_ROOT, scheme, 0, KEY_READ, &hKey);
	if (urlProtocol != ERROR_NO_MATCH && urlProtocol != ERROR_FILE_NOT_FOUND)
		debug << "Protocol already exists";
		return 0;

	// Create the base key
	if (RegCreateKeyEx(HKEY_CLASSES_ROOT, scheme, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
		debug << "Error creating regex key";
		return 0;

	// Set default value (description)
	if (RegSetValueEx(hKey, scheme, 0, REG_SZ, (LPBYTE)schemeDescription, wcslen(schemeDescription)*sizeof(char)) != ERROR_SUCCESS)
		debug << "Error setting default value";
		return 0;

	// Set URL protocol to an empty value, which is required for URI schemes for some reason
	if (RegSetValueEx(hKey, TEXT("URL Protocol"), 0, REG_SZ, (LPBYTE)"", 0) != ERROR_SUCCESS)
		debug << "Error setting URL protocol";
		return 0;

	
	debug << "Success!";
	debug.close();

	// If we've made it this far, everything should have worked
	return 1;

}