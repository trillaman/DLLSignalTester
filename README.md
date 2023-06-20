# DLLSignalTester
 This is for testing alerts based on dll loading rules or invoking processes with specific name of executable/dll.

# Purpose
 If you have monitoring software that detects exe processes and dll loaded by them BUT you don't have acccess to software you want to test - you can imitiate that with this one.

# Usage  


```ConsoleApp.exe -fork <path to exe> ``` - imitate process creating of exe you 

```ConsoleApp.exe -load <path to dll in project> ``` - this will imitate side loading of dll that in this case returns Hello World

