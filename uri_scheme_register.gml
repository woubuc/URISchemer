// Define external method
registerURIScheme = external_define("URISchemer.dll", "registerURIScheme", dll_cdecl, ty_string, 1, ty_string);

// Call external method
return external_call(registerURIScheme, argument0);
