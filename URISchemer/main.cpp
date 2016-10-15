// Much thanks to the awesome tutorial at http://gmc.yoyogames.com/index.php?showtopic=405812

#pragma comment(lib, "Shlwapi.lib") 

#include "Shlwapi.h"
#include <fstream>
#include <string>
#include <Windows.h>

#define GMEXPORT extern "C" __declspec (dllexport)

using namespace std;
bool debugMode = false;

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
GMEXPORT double registerURIScheme(char schemeChar[]) {

	// Make it a string because char sucks
	string scheme = string(schemeChar);

	// Prepare DefaultIcon path
	string schemeDefaultIcon = string("");
	schemeDefaultIcon += scheme;
	schemeDefaultIcon += "\\DefaultIcon";

	// Prepare shell command path
	string schemeOpenCommand = string("");
	schemeOpenCommand += scheme;
	schemeOpenCommand += "\\shell\\open\\command";

	// Prepare description
	string schemeDescription = string("URL:");
	schemeDescription += scheme;
	schemeDescription += " Protocol";

	// Get current filename & path
	char applicationPath[MAX_PATH * 2];
	GetModuleFileNameA(NULL, applicationPath, MAX_PATH * 2);


	// Prepare vars
	bool success = false;
	HKEY hKey = NULL;

	ofstream debug;
	if (debugMode) {
		debug.open("debug.txt");
		debug << "SCHEME: " << scheme << "\n";
		debug << "DEFAULTICON: " << schemeDefaultIcon << "\n";
		debug << "OPENCOMMAND: " << schemeOpenCommand << "\n";
		debug << "DESCRIPTION: " << schemeDescription << "\n";
		debug << "\nSTART inserting registry items\n";
	}

	// Check if a URI scheme handler already exists
	// Because if one exists, we shouldn't overwrite it
	long urlProtocol = RegOpenKeyExA(HKEY_CLASSES_ROOT, scheme.c_str(), NULL, KEY_READ, &hKey);
	if (urlProtocol != ERROR_NO_MATCH && urlProtocol != ERROR_FILE_NOT_FOUND) {
		if (debugMode)
			debug << "Protocol already exists";
		return -1;
	}
	if (debugMode)
		debug << "Protocol not found\n";



	// Create the base key
	if (RegCreateKeyExA(HKEY_CLASSES_ROOT, scheme.c_str(), 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS) {
		if (debugMode)
			debug << "Error creating registry key";
		return 0;
	}
	if (debugMode)
		debug << "Registry key created\n";

	// Set default value (description)
	if (RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)schemeDescription.c_str(), strlen(schemeDescription.c_str())*sizeof(char)) != ERROR_SUCCESS) {
		if (debugMode)
			debug << "Error setting default value";
		return 0;
	}
	if (debugMode)
		debug << "Default value set\n";

	// Set URL protocol to an empty value, which is required for URI schemes for some reason
	if (RegSetValueExA(hKey, "URL Protocol", 0, REG_SZ, (LPBYTE)"", 0) != ERROR_SUCCESS) {
		if (debugMode)
			debug << "Error setting URL protocol";
		return 0;
	}
	if (debugMode)
		debug << "URL protocol set\n";



	// Create the DefaultIcon key
	if (RegCreateKeyExA(HKEY_CLASSES_ROOT, schemeDefaultIcon.c_str(), 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS) {
		if (debugMode)
			debug << "Error creating DefaultIcon key";
		return 0;
	}
	if (debugMode)
		debug << "DefaultIcon key created\n";

	// Set default value (description)
	LPSTR exeName = PathFindFileNameA(applicationPath);
	if (RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)exeName, strlen(exeName)*sizeof(char)) != ERROR_SUCCESS) {
		if (debugMode)
			debug << "Error setting DefaultIcon value";
		return 0;
	}
	if (debugMode)
		debug << "DefaultIcon value set\n";



	// Create the shell/open/command key
	if (RegCreateKeyExA(HKEY_CLASSES_ROOT, schemeOpenCommand.c_str(), 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL) != ERROR_SUCCESS) {
		if (debugMode)
			debug << "Error creating open command key";
		return 0;
	}
	if (debugMode)
		debug << "Open command key created\n";

	// Set default value (description)
	string path = "\"" + string(applicationPath) + "\" \"%1\"";
	if (RegSetValueExA(hKey, NULL, 0, REG_SZ, (LPBYTE)path.c_str(), strlen(path.c_str())*sizeof(char)) != ERROR_SUCCESS) {
		if (debugMode)
			debug << "Error setting open command value";
		return 0;
	}
	if (debugMode)
		debug << "Open command value set\n";



	// If we've made it this far, everything should have worked
	if (debugMode) {
		debug << "Success!";
		debug.close();
	}

	return 1;

}