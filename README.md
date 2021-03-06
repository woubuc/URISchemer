# URISchemer for GM:S
URISchemer is a DLL that allows you to create a URI scheme in Windows right from your Gamemaker: Studio game.

## What is a URI scheme
A URI scheme is a type of link to easily open a program and pass it some data. An example of a program that does this is [Steam](https://developer.valvesoftware.com/wiki/Steam_browser_protocol).

## Where to get
Grab the DLL from the [releases](https://github.com/woubuc/URISchemer/releases) page.

## How to use
Take a look at the [example project](https://github.com/woubuc/URISchemer-example).

## DLL reference
The DLL exposes two functions:

#### registerURIScheme

    double registerURIScheme(char scheme[])

Creates the required registry entries for a URI scheme. It will not override existing URI schemes.

| Argument | Type   | Description |
| -------- | ------ | ----------- |
| scheme   | char[] | Name of the URI scheme you want (e.g. `"example"` to get the scheme `example://`) |

Returns one of three numerical values:

| Return value | Description |
| ------------ | ----------- |
| -1           | URI scheme already exists |
| 0            | An error occured while creating the URI scheme |
| 1            | URI scheme was successfully created |

#### requestAdminRights

    double requestAdminRights()

Restarts the application in admin mode, popping up a UAC request if appropriate.

**Note**: it does not exit the current process.

Takes no arguments and always returns 1.

## Disclaimer
This is my first attempt at writing C++. As such, it may contain bugs and it almost definitely isn't following any kind of best practices. If you see room for improvement, don't hesitate to let me know or submit a PR.

This DLL is also largely untested at this point. I've only had the chance to test it on my on Windows 7 computer. If you find it doesn't work on your system, be sure to let me know.
