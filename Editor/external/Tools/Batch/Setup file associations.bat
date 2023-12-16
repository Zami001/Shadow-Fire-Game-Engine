@echo off

REG ADD "HKCR\SOFTWARE\.SFPROJECT" /ve /t REG_DWORD /d "ShadowFire.ProjectFile"
REG ADD "HKCR\SOFTWARE\ShadowFire.ProjectFile" /ve /t REG_DWORD /d "Shadow fire project file"
REG ADD "HKCR\SOFTWARE\ShadowFire.ProjectFile\shell\open" /ve /t REG_DWORD /d "C:\Shadow Fire Game Engine\out\build\x64-Debug\bin\Debug\Shadow Fire Editor.exe %1"