' this help to start the application in hidden mode

Set WshShell = CreateObject("WScript.Shell")
WshShell.Run "startApplication.bat", 0, True
