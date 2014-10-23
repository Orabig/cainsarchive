cd ..\src
echo Meta-Object Compile..
if not exist %CD%\moc md "moc"

for /D %%d in ("*") do ( 
if not "%%~nd"=="moc" (
cd %%~nd
for %%f in (Qt*.h) do (
	moc.exe -o %CD%\moc\moc_%%~nf.cpp %%~nf.h
	echo %CD%\%%~nf.h generated %CD%\moc\moc_%%~nf.cpp
)
cd ..
)
)

for %%f in (Qt*.h) do (
	moc.exe -o %CD%\moc\moc_%%~nf.cpp %%~nf.h
	echo %CD%\%%~nf.h generated %CD%\moc\moc_%%~nf.cpp
)
