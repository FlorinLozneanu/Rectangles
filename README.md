# Rectangles

The project was implemented using Visual Studio 2017 Community Edition (which can be freely downloaded from Microsoft site).

The name of the solution is Rectangles.sln and consists of two projects:

- Rectangle.vcxproj (the implementation of the given task)
- Tests\Tests.vcxproj (unit tests implemented with Google Test framework)

Third party libraries:

- json\json.h (https://github.com/nlohmann/json) : a simple header only framework used to load the json files and parse them
- Google Test framework : can be found in directory "packages\gtest-vc140-static-32_64.1.1.0\lib\native" (used for the unit tests in project Tests\Tests.vcxproj)

The both third party libraries are included with this repo.

Visual Studio solution builds two executable files (Rectangles.exe and Tests.exe) which can be found in the directory "bin" (for each platform (win32/x64) and configuration (Debug/Release).

Directory "data" contains some json files which I used to test the current implementation.
