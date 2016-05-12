#include <json.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <exception>
#include <iostream>
#include <clocale>

int main(int argc, char* argv[])
{
  std::setlocale(LC_ALL, "");

  try {
    json::value config;
    config["test"];// = 3;
  }
  catch (const std::exception& e) {
    std::cerr << "error: " << e.what() << std::endl;
  }

  std::cout << "done" << std::endl;

#ifdef _WIN32
  if (GetConsoleWindow() && IsDebuggerPresent()) {
    std::cerr << "Press ENTER to close this window." << std::endl;
    std::cin.get();
  }
#endif
  return 0;
}