/*

https://github.com/NTDLS/NSWFL/blob/master/NSWFL_Registry.Cpp
 */
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <conio.h>


#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

/* Sample call "sim query HKEY_LOCAL_MACHINE\hardware\description\system"*/

void query(HKEY Hive, wchar_t * OriginalCopy, DWORD Level );
void QueryKey(HKEY regHive, HKEY hKey, const wchar_t * proba, DWORD Level );


int counter = 0;

void __cdecl _tmain(int argc, char* argv[])
{

    // HKEY regHive = HKEY_LOCAL_MACHINE;
    // query(regHive, L"hardware\\description\\system", 0);


    printf("%s\n", argv[2]);
    // char pathCopy[MAX_VALUE_NAME];
    char pathCopy[MAX_PATH];
    char subpath[MAX_PATH];
    strcpy(pathCopy, argv[2]);
    char* hive = strtok(argv[2], "\\");
    sprintf(subpath, "%s", pathCopy + strlen(hive) + 1);


    printf("%s,%s \n", hive, subpath);

    printf("%s | %s %d\n", hive, subpath, strcmp(strupr(hive), "HKEY_LOCAL_MACHINE"));
    HKEY regHive;
    if(!strcmp(strupr(hive), "HKEY_LOCAL_MACHINE")){
      regHive = HKEY_LOCAL_MACHINE;
      wchar_t wszSubPath[MAX_PATH];
      mbstowcs(wszSubPath, subpath, sizeof(subpath));
      query(regHive, wszSubPath, 0);

      //query(regHive, L"hardware\\description\\system", 0);
    }else{
      regHive = HKEY_CURRENT_USER;
      query(regHive, L"software\\microsoft\\wisp", 0);
    }

    return;

      // HKEY regHive = HKEY_CURRENT_USER;
      // query(regHive, L"software\\microsoft\\wisp", 0);
}

void query(HKEY regHive, wchar_t * OriginalCopy, DWORD Level)
{
    HKEY hqueryKey;

    if ( RegOpenKeyExW( regHive, (LPCWSTR)OriginalCopy, 0, KEY_READ, &hqueryKey ) == ERROR_SUCCESS )
    {
        printf("\n%ls\n",OriginalCopy);
        QueryKey(regHive, hqueryKey, OriginalCopy, Level + 1 );
    }
    else printf("\nquery Failed");
    RegCloseKey(hqueryKey);

}

void QueryKey(HKEY regHive, HKEY hKey, const wchar_t * OriginalCopy, DWORD Level )
{
    //printf("\n1. OriginalCopy: %ls Level %d", OriginalCopy, Level );

    TCHAR    achKey[MAX_KEY_LENGTH];      // buffer for subkey name
    DWORD    cbName;                          // size of name string
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name
    DWORD    cchClassName = MAX_PATH;  // size of class string
    DWORD    cSubKeys=0;                      // number of subkeys
    DWORD    cbMaxSubKey;                 // longest subkey size
    DWORD    cchMaxClass;                 // longest class string
    DWORD    cValues;                    // number of values for key
    DWORD    cchMaxValue;            // longest value name
    DWORD    cbMaxValueData;         // longest value data
    DWORD    cbSecurityDescriptor; // size of security descriptor
    FILETIME ftLastWriteTime;        // last write time

    DWORD i, retCode;
    TCHAR  achValue[MAX_VALUE_NAME];

    DWORD cchValue = MAX_VALUE_NAME;

    // Get the class name and the value count.
    retCode = RegQueryInfoKey(
                                     hKey,                        // key handle
                                     achClass,                    // buffer for class name
                                     &cchClassName,           // size of class string
                                     NULL,                        // reserved
                                     &cSubKeys,                   // number of subkeys
                                     &cbMaxSubKey,                // longest subkey size
                                     &cchMaxClass,                // longest class string
                                     &cValues,                    // number of values for this key
                                     &cchMaxValue,                // longest value name
                                     &cbMaxValueData,             // longest value data
                                     &cbSecurityDescriptor,   // security descriptor
                                     &ftLastWriteTime);       // last write time

    // Enumerate the subkeys, until RegEnumKeyEx fails.

    if (cSubKeys)
    {
        //printf( "\nNumber of subkeys: %d\n", cSubKeys);

        for (i=0; i<cSubKeys; i++)
        {
            cbName = MAX_KEY_LENGTH;
            retCode = RegEnumKeyExW(hKey,
                                            i,
                                            (LPWSTR)achKey,
                                            &cbName,
                                            NULL,
                                            NULL,
                                            NULL,
                                            &ftLastWriteTime
                                          );

            if (retCode == ERROR_SUCCESS)
            {
                //_tprintf(TEXT("(%d) %s\n"), i+1, achKey);
                //-------------------------------------------------------------------
                HKEY subkey;

                wchar_t NewCopy[MAX_PATH];
                wcscpy( NewCopy, OriginalCopy );
                wcscat( NewCopy, L"\\" );
                wcscat( NewCopy, (const wchar_t *)achKey);
                //printf("\nNew subkey found \"%ls\" Number of subkeys: %d\n",achKey, cSubKeys);
                //printf("\nNew OriginalCopy \"%ls\"Level: %d\n", NewCopy, Level);

                if ( RegOpenKeyExW( regHive, OriginalCopy, 0, KEY_READ, &subkey ) == ERROR_SUCCESS )
                {
                    counter++;
                    query(regHive, NewCopy, Level + 1 );
                    RegCloseKey( subkey );
                }
                else printf("\n-----Querykey Failed for %ls\n",OriginalCopy );


                //-------------------------------------------------------------------
            }
        }
    }

    // Enumerate the key values.

    /*if (cValues)
    {
         printf( "\nNumber of values: %d\n", cValues);

         for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++)
         {
              cchValue = MAX_VALUE_NAME;
              achValue[0] = '\0';
              retCode = RegEnumValue( hKey,
                                              i,
                                              achValue,
                                              &cchValue,
                                              NULL,
                                              NULL,
                                              NULL,
                                              NULL
                                            );

              if (retCode == ERROR_SUCCESS )
              {
                    _tprintf(TEXT("(%d) %s\n"), i+1, achValue);
              }
         }
    } */
}

