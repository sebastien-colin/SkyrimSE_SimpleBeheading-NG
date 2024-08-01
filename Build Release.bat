@echo off

cmake -S . --preset=ALL --check-stamp-file "build\CMakeFiles\generate.stamp"
if %ERRORLEVEL% NEQ 0 exit 1
cmake --build build --config Release > build.log
if %ERRORLEVEL% NEQ 0 exit 1

xcopy "build\release\*.dll" "dist\SKSE\Plugins\" /I /Y
xcopy "build\release\*.pdb" "dist\SKSE\Plugins\" /I /Y

xcopy "package" "dist" /I /Y /E

pause