:: This nonsense is from the "Developer PowerShell for VS 2022" shortcut "Target:" property
:: Without all this crap, PowerShell is not properly opened for VS Code
C:\Windows\SysWOW64\WindowsPowerShell\v1.0\powershell.exe -noe -c "&{Import-Module """C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"""; Enter-VsDevShell b802c9a4}; code %cd%"

:: -c is for -Command
:: Passed are the arguments to run. MS set up 1 and 2, I appended 3
:: (1) &{Import-Module """C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\ToolsMicrosoft.VisualStudio.DevShell.dll"""
:: (2) Enter-VsDevShell b802c9a4}
:: (3) code %cd%