# CMakeList.txt : CMake project for zoom_cli, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("zoom_cli")

# Add source to this project's executable.
add_executable (zoom  "src/main.h"  "src/utils.h" "src/utils.cpp" "src/meeting.h" "src/meeting.cpp" "src/main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET zoom PROPERTY CXX_STANDARD 20)
endif()

# Tests
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(zoom_tests
  "tests/test_main.cpp"
  "src/utils.cpp"
  "src/meeting.cpp"
)
target_include_directories(zoom_tests PRIVATE src)
target_link_libraries(zoom_tests PRIVATE Catch2::Catch2WithMain)
set_property(TARGET zoom_tests PROPERTY CXX_STANDARD 20)

include(CTest)
include(Catch)
catch_discover_tests(zoom_tests)