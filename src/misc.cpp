#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <stdio.h>
//#include "regex.h"

int IsInString(char c, char* string)
{
    while(*string)
    {
        if(c==*string) return 1;
        string++;
    }
    return 0;
}

int CreateConsole()
{
    if( !AllocConsole() )
    {
        return 0;
    }
    SetConsoleTitle(TEXT("Klotski Console"));
    freopen(TEXT("CONOUT$"),TEXT("w"), stdout);
    //freopen(TEXT("CONIN$"), TEXT("r"), stdin);
    return 1;
}

/* int test()
{   
    //const char pRegexStr[] = "^[A-Za-z0-9.*]-[A-Za-z0-9.*]=-?[1-9]+$";
    //const char pText[] = ".-f=-1231";
    const char pRegexStr[] = "^[A-Za-z0-9.*]=[A-Za-z0-9.*]$";
    const char pText[] = ".=*";
    regex_t oRegex;
    int nErrCode = 0;
    if ((nErrCode = regcomp(&oRegex, pRegexStr, REG_EXTENDED)) == 0)
    {
        if ((nErrCode = regexec(&oRegex, pText, 0, NULL, 0)) == 0)
        {
            printf("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
            regfree(&oRegex);
            return 0;
        }
    }
} */
