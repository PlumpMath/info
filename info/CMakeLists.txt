cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(info VERSION 0.1.0 LANGUAGES CXX)

# Compiler
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configurations
set(CMAKE_CONFIGURATION_TYPES Debug Release)

# Compiler Options
if(MSVC)
  foreach(flag
      #CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
      CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE)
    if(${flag} MATCHES "/MD")
      string(REPLACE "/MD" "/MT" ${flag} "${${flag}}")
    endif()
    set(${flag} "${${flag}} /arch:IA32")
  endforeach()
endif()

# Definitions
if(MSVC)
  add_definitions(/D_UNICODE /DUNICODDE /DWIN32_LEAN_AND_MEAN /DNOMINMAX)
  add_definitions(/D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS)
  add_definitions(/DWINVER=0x0501 /D_WIN32_WINNT=0x0501)
endif()

# Linker Options
if(MSVC)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ignore:4099")
endif()

# Dependencies
find_package(compat REQUIRED PATHS ../third_party/compat)
find_package(json REQUIRED PATHS ../third_party/json)

# Sources
function(assign_source_group)
  foreach(source IN ITEMS ${ARGN})
    file(RELATIVE_PATH source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${source}")
    get_filename_component(source_path "${source_rel}" PATH)
    string(REPLACE "/" "\\" source_path_msvc "${source_path}")
    source_group("${source_path_msvc}" FILES "${source}")
  endforeach()
endfunction(assign_source_group)

file(GLOB_RECURSE sources src/*.h src/*.cc)
assign_source_group(${sources})

file(GLOB_RECURSE cef_headers ../third_party/json/include/*.h)
assign_source_group(${cef_headers})

# Resources
if(WIN32)
  file(GLOB resources res/resource.*)
  source_group("res" FILES ${resources})
  list(APPEND sources ${resources})
endif()

# Executable
add_executable(info ${sources} ${cef_headers})
target_link_libraries(info PRIVATE compat json)

# Include Directories
target_include_directories(info PRIVATE src res)

# Install Target
install(TARGETS info
  DESTINATION bin/$<CONFIG>)
