if not exist "build" mkdir "build"
cd "build"

set "BUILD_TYPE=Release"
cmake .. -G "Visual Studio 17 2022" -A x64 -DCMAKE_BUILD_TYPE=%BUILD_TYPE%