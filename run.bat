cd Debug
start ServerPrototype.exe ../Source-Code-Publisher localhost 8080

cd ..\ClientGUI\bin\x86\Debug\
start ClientGUI.exe ../../Source-Code-Publisher localhost 8080 8081
