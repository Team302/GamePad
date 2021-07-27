@echo off

copy Team302Gamepad.json c:\Users\Public\wpilib\2021\vendordeps
Xcopy /E /I team302 c:\Users\Public\wpilib\2021\maven\com\team302 
echo Done!
pause