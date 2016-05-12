@echo off
rem Enter the script directory.
pushd %~dp0

rem Create and enter the build directory.
if not exist build (
  mkdir build
)
pushd build

rem Generate and open the solution.
cmake -G "Visual Studio 14 2015" -T "v140_xp" -DCMAKE_INSTALL_PREFIX:PATH=.. -DCMAKE_BUILD_TYPE=Release ..
if %errorlevel% == 0 (
  start %~n0.sln
) else (
  pause
)

rem Leave the build directory.
popd
