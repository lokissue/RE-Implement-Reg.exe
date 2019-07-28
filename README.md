# RE-Implement-Reg.exe

This code is for Windows reg.exe re-implementation.
Includes funtionalities of query(recursively), add and delete (recursively) registry key and add value.
Running with identical command line areguments to reg.exe 
Supported value data types: string(REG_SZ), binary(REG_BINARY), dword(REG_DWORD).

SAMPLE commend line input: 
Query: reg-sim query HKCU\Software\Microsoft
Add (key): reg-sim add HKCU\Software\Microsoft\mylog
Add (value):reg-sim add HKCU\Software\Microsoft\mylog  /v data /t REG_DWORD /d 00000000
Delete: reg-sim delete HKCU\Software\Microsoft\mylog
