cmake_minimum_required(VERSION 3.27)
project(angeltest)

set(CMAKE_CXX_STANDARD 17)

add_executable(angeltest main.cpp
        main.cpp)

# vcpkg install angelscript
find_package(Angelscript CONFIG REQUIRED)
target_link_libraries(angeltest PRIVATE Angelscript::angelscript)