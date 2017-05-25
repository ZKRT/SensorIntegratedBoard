@echo off
md ..\..\..\User\Release

echo turn executable files from axf to bin by tool names fromelf in filefordler of D:\Keil_v5\ARM\ARMCC\bin\ .
D:\Keil_v5\ARM\ARMCC\bin\fromelf.exe --bin -o .\Objects\SensorInterged.bin .\Objects\SensorInterged.axf

echo copy release files to release folder.
copy .\Objects\SensorInterged.bin ..\..\..\User\Release\SensorInterged-V01-00-170331_test.bin
copy .\Objects\SensorInterged.hex ..\..\..\User\Release\SensorInterged-V01-00-170331_test.hex
