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
GMEXPORT double registerURIScheme(char scheme[]) {

	// Prepare DefaultIcon path
	char schemeDefaultIcon[48] = "";
	strncpy_s(schemeDefaultIcon, scheme, 32);
	strcat_s(schemeDefaultIcon, "\\DefaultIcon");

	// Prepare Shell command path
	char schemeOpenCommand[64] = "";
	strncpy_s(schemeOpenCommand, scheme, 32);
	strcat_s(schemeOpenCommand, "\\shell\\open\\command");

	// Prepare description
	char schemeDescription[52] = "URL:";
	strncat_s(schemeDescription, scheme, 32);
	strcat_s(schemeDescription, " Protocol");


	// Get current filename & path
	char applicationPath[MAX_PATH * 2];
	GetModuleFileNameA(NULL, applicationPath, MAX_PATH * 2);


	// Prepare vars
	bool success = false;
	HKEY hKey = NULL;

	std::ofstream debug;
	debug.open("debug.txt");
	debug << "SCHEME: " << scheme << "\n";
	debug << "DEFAULTICON: " << schemeDefaultIcon << "\n";
	debug << "OPENCOMMAND: " << schemeOpenCommand << "\n";
	debug << "DESCRIPTION: " << schemeDescription << "\n";
	debug << "\nSTART inserting registry items";

	// Check if a URI scheme handler already exists
	// Because if one exists, we shouldn't overwrite it
	long urlProtocol = RegOpenKeyExA(HKEY_CLASSES_ROOT, scheme, 0, KEY_READ, &hKey);
	if (urlProtocol != ERROR_NO_MATCH && urlProtocol != ERROR_FILE_NOT_FOUND)
		debug << "Protocol already exists";
		return 0;

	debug << "Protocol not found\n";

	// Create the base key
	if (RegCreateKeyExA(HKEY_CLASSES_ROOT, scheme, 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS)
		debug << "Error creating registry key";
		return 0;

	debug << "Registry key created";

	// Set default value (description)
	if (RegSetValueExA(hKey, scheme, 0, REG_SZ, (LPBYTE)schemeDescription, strlen(schemeDescription)*sizeof(char)) != ERROR_SUCCESS)
		debug << "Error setting default value";
		return 0;

	debug << "Default value set";

	// Set URL protocol to an empty value, which is required for URI schemes for some reason
	if (RegSetValueExA(hKey, "URL Protocol", 0, REG_SZ, (LPBYTE)"", 0) != ERROR_SUCCESS)
		debug << "Error setting URL protocol";
		return 0;

	debug << "URL protocol set";

	
	debug << "Success!";
	debug.close();

	// If we've made it this far, everything should have worked
	return 1;

}