cmake_minimum_required(VERSION 3.7)

project(hello_crow)

set(CMAKE_CXX_STANDARD 11)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads REQUIRED)
find_package(PkgConfig REQUIRED)

# Find the libmongocxx package
pkg_check_modules(libmongocxx REQUIRED libmongocxx)

add_executable(hello_crow main.cpp)

# Include directories for Boost and libmongocxx
target_include_directories(hello_crow PRIVATE ${Boost_INCLUDE_DIRS} ${libmongocxx_INCLUDE_DIRS})

# Link the required libraries
target_link_libraries(hello_crow ${Boost_LIBRARIES} Threads::Threads ${libmongocxx_LIBRARIES})
