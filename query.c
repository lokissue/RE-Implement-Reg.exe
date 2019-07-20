#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <conio.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void test( wchar_t * OriginalCopy, DWORD Level );
void QueryKey(HKEY hKey, const wchar_t * proba, DWORD Level );


int counter = 0;

void __cdecl _tmain(void)
{

    test( L"hardware\\description\\system", 0);

}

void test(wchar_t * OriginalCopy, DWORD Level)
{
    HKEY hTestKey;

    if ( RegOpenKeyExW( HKEY_LOCAL_MACHINE, (LPCWSTR)OriginalCopy, 0, KEY_READ, &hTestKey ) == ERROR_SUCCESS )
    {
        printf("\n%ls",OriginalCopy);
        QueryKey(hTestKey, OriginalCopy, Level + 1 );
    }
    else printf("\nTest Failed");
    RegCloseKey(hTestKey);

}

void QueryKey( HKEY hKey, const wchar_t * OriginalCopy, DWORD Level )
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
                printf("\nNew OriginalCopy \"%ls\"Level: %d\n", NewCopy, Level);

                if ( RegOpenKeyExW( HKEY_LOCAL_MACHINE, OriginalCopy, 0, KEY_READ, &subkey ) == ERROR_SUCCESS )
                {
                    counter++;
                    test( NewCopy, Level + 1 );
                    RegCloseKey( subkey );
                }
                else printf("\n-----Querykey Failed for %ls\n",OriginalCopy );


                //-------------------------------------------------------------------
            }
        }
    }

    // Enumerate the key values.

    if (cValues)
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
    }
}

