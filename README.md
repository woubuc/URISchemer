# To whom it may concern
This is a DLL I am working on for use with GameMaker Studio. When (if) it is finished, it should allow a game developer to register an URI scheme for his game. However, it does not work yet.

## The problem
The DLL itself seems to work. I have added it in a GameMaker test project using this script:

    // Define external method
    registerURIScheme = external_define("URISchemer.dll", "registerURIScheme", dll_cdecl, ty_real, 1, ty_string);
    // Call external method
    return external_call(registerURIScheme, "mytest");

When I run the test project, the script returns 1, but no registry entries are created. I suspect this is because I am using the registry functions wrong. So I am hoping someone with a little bit more experience will look at this code and figure out where I'm screwing up.

## The solution
_(feel free to fill this space if you know how to fix my code)_

## More info
[What I need the URI schemes for](https://www.reddit.com/r/gamemaker/comments/56r6yi/creating_a_uri_scheme_for_my_game/?ref=share&ref_source=link) 