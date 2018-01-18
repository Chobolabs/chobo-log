@echo off
if not exist gen mkdir gen
cd gen
if not exist vs14x64 mkdir vs14x64
cd vs14x64
cmake ../../.. -G "Visual Studio 14 2015 Win64" -DCHOBO_LOG_SHARED=1 -DCHOBO_LOG_BUILD_EXAMPLES=1 -DCHOBO_LOG_BUILD_TESTS=1
cd ..\..