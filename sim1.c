/*

https://github.com/NTDLS/NSWFL/blob/master/NSWFL_Registry.Cpp
https://www.tenouk.com/cpluscodesnippet/createregistrysubkeysnamesvalues.html
 */
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <stdbool.h>


#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

/* Sample call "sim query HKEY_LOCAL_MACHINE\hardware\description\system"*/

void query(HKEY Hive, wchar_t * OriginalCopy, DWORD Level );
void QueryKey(HKEY hKeyRoot, HKEY hKey, const wchar_t * proba, DWORD Level );
bool CreateRegistryKey(HKEY hKeyRoot, LPCTSTR pszSubKey);
bool Set_BinaryRegistryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, PVOID pData, DWORD dwSize);
bool Set_DWORDRegistryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, unsigned long ulValue);
bool Set_StringRegistryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszString);
bool DeleteRegistryKey(HKEY hKeyRoot, LPCTSTR pszSubKey);

int counter = 0;

void __cdecl _tmain(int argc, char* argv[])
{

  if(argc < 2){
    printf("Invaild Input\n");
    return;
  }

  printf("%s\n", argv[2]);

  char pathCopy[MAX_PATH];
  char subpath[MAX_PATH];
  strcpy(pathCopy, argv[2]);
  char* root = strtok(argv[2], "\\");
  sprintf(subpath, "%s", pathCopy + strlen(root) + 1);


  printf("%s,%s \n", root, subpath);

  printf("%s | %s %d\n", root, subpath, strcmp(strupr(root), "HKEY_LOCAL_MACHINE"));

  HKEY hKeyRoot;
  if(!strcmp(strupr(root), "HKEY_LOCAL_MACHINE")||!strcmp(strupr(root), "HKLM")){
    hKeyRoot = HKEY_LOCAL_MACHINE;
  }else if(!strcmp(strupr(root), "HKEY_CURRENT_USER")||!strcmp(strupr(root), "HKCU"))
  {
    hKeyRoot = HKEY_CURRENT_USER;
  }else if(!strcmp(strupr(root), "HKEY_CLASSES_ROOT")||!strcmp(strupr(root), "HKCR")){
    hKeyRoot = HKEY_CLASSES_ROOT;
  }else if(!strcmp(strupr(root), "HKEY_CURRENT_CONFIG")||!strcmp(strupr(root), "HKCC")){
    hKeyRoot = HKEY_CURRENT_CONFIG;
  }else if(!strcmp(strupr(root), "HKEY_USERS")||!strcmp(strupr(root), "HKU")){
    hKeyRoot = HKEY_USERS;
  }else{
    printf("Invaild Root Key\n");
    return;
  }

  wchar_t wszSubPath[MAX_PATH];
  mbstowcs(wszSubPath, subpath, sizeof(subpath));

  if(!strcmp(strupr(argv[1]), "ADD")){
    printf("run add \n");
    if(argc == 2){
      CreateRegistryKey(hKeyRoot, (LPCTSTR) wszSubPath);
    }
    return;
  }else if (!strcmp(strupr(argv[1]), "QUERY")){
    printf("run query \n");
    query(hKeyRoot, wszSubPath, 0);
  }else if (!strcmp(strupr(argv[1]), "DELETE")) {
    printf("run delete \n");
    DeleteRegistryKey(hKeyRoot, (LPCTSTR) wszSubPath);
  }else{
    printf("Invaild Commnand\n");
  }
  return;
}

void query(HKEY hKeyRoot, wchar_t * OriginalCopy, DWORD Level)
{
    HKEY hQueryKey;

    if ( RegOpenKeyExW( hKeyRoot, (LPCWSTR)OriginalCopy, 0, KEY_READ, &hQueryKey ) == ERROR_SUCCESS )
    {
        printf("\n%ls\n",OriginalCopy);
        QueryKey(hKeyRoot, hQueryKey, OriginalCopy, Level + 1 );
    }
    else printf("\nquery Failed");
    RegCloseKey(hQueryKey);

}

void QueryKey(HKEY hKeyRoot, HKEY hKey, const wchar_t * OriginalCopy, DWORD Level )
{

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
    //TCHAR  achValue[MAX_VALUE_NAME];

    //DWORD cchValue = MAX_VALUE_NAME;

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

                //-------------------------------------------------------------------
                HKEY subkey;

                wchar_t NewCopy[MAX_PATH];
                wcscpy( NewCopy, OriginalCopy );
                wcscat( NewCopy, L"\\" );
                wcscat( NewCopy, (const wchar_t *)achKey);

                if ( RegOpenKeyExW( hKeyRoot, OriginalCopy, 0, KEY_READ, &subkey ) == ERROR_SUCCESS )
                {
                    counter++;
                    query(hKeyRoot, NewCopy, Level + 1 );
                    RegCloseKey( subkey );
                }
                else printf("\n-----Querykey Failed for %ls\n",OriginalCopy );


                //-------------------------------------------------------------------
            }
        }
    }
}

bool CreateRegistryKey(HKEY hKeyRoot, LPCTSTR pszSubKey)
{
  HKEY hKey;
  DWORD dwFunc;
  LONG  lRet;

  //------------------------------------------------------------------------------

  SECURITY_DESCRIPTOR SD;
  SECURITY_ATTRIBUTES SA;

  if (!InitializeSecurityDescriptor(&SD, SECURITY_DESCRIPTOR_REVISION))
    return false;

  if (!SetSecurityDescriptorDacl(&SD, true, 0, false))
    return false;

  SA.nLength = sizeof(SA);
  SA.lpSecurityDescriptor = &SD;
  SA.bInheritHandle = false;

  //------------------------------------------------------------------------------

  lRet = RegCreateKeyEx(
    hKeyRoot,
    pszSubKey,
    0,
    (LPTSTR)NULL,
    REG_OPTION_NON_VOLATILE,
    KEY_WRITE,
    &SA,
    &hKey,
    &dwFunc
    );

  if (lRet == ERROR_SUCCESS)
  {
    RegCloseKey(hKey);
    hKey = (HKEY)NULL;
    return true;
  }

  SetLastError((DWORD)lRet);
  return false;
}

bool Set_BinaryRegistryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, PVOID pData, DWORD dwSize)
{
	HKEY hKey;
	LONG lRes = 0;

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_WRITE, &hKey);

	if (lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return false;
	}

	lRes = RegSetValueEx(hKey, pszValue, 0, REG_BINARY, (unsigned char*)pData, dwSize);

	RegCloseKey(hKey);

	if (lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return false;
	}

	return true;
}

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Writes a DWORD value to the registry
bool Set_DWORDRegistryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, unsigned long ulValue)
{
	HKEY hKey;
	LONG lRes;

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_WRITE, &hKey);

	if (lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return false;
	}

	lRes = RegSetValueEx(hKey, pszValue, 0, REG_DWORD, (unsigned char*)&ulValue, sizeof(ulValue));

	RegCloseKey(hKey);

	if (lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return false;
	}

	return true;
}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Writes a string to the registry.
bool Set_StringRegistryValue(HKEY hKeyRoot, LPCTSTR pszSubKey, LPCTSTR pszValue, LPCTSTR pszString)
{
	HKEY  hKey;
	LONG  lRes;
	DWORD dwSize = lstrlen(pszString) * sizeof(TCHAR);

	lRes = RegOpenKeyEx(hKeyRoot, pszSubKey, 0, KEY_WRITE, &hKey);

	if (lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return false;
	}

	lRes = RegSetValueEx(hKey, pszValue, 0, REG_SZ, (unsigned char*)pszString, dwSize);

	RegCloseKey(hKey);

	if (lRes != ERROR_SUCCESS)
	{
		SetLastError(lRes);
		return false;
	}

	return true;
}

bool DeleteRegistryKey(HKEY hKeyRoot, LPCTSTR pszSubKey)
{
  DWORD dwRet = ERROR_SUCCESS;

  if (RegDeleteKey(hKeyRoot, pszSubKey) != ERROR_SUCCESS)
  {
    HINSTANCE hLibInst = LoadLibrary(_T("shlwapi.dll"));

    if (!hLibInst)
    {
      //throw ERROR_NO_SHLWAPI_DLL;
    }

    typedef DWORD(__stdcall* SHDELKEYPROC)(HKEY, LPCTSTR);

#if defined(UNICODE) || defined(_UNICODE)
    SHDELKEYPROC DeleteKeyRecursive = (SHDELKEYPROC)GetProcAddress(hLibInst, "SHDeleteKeyW");
#else
    SHDELKEYPROC DeleteKeyRecursive = (SHDELKEYPROC)GetProcAddress(hLibInst, "SHDeleteKeyA");
#endif

    if (!DeleteKeyRecursive)
    {
      FreeLibrary(hLibInst);
      return false;
    }

    dwRet = DeleteKeyRecursive(hKeyRoot, pszSubKey);

    FreeLibrary(hLibInst);
  }

  if (dwRet == ERROR_SUCCESS)
    return true;

  SetLastError(dwRet);
  return false;
}
