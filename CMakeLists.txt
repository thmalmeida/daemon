cmake_minimum_required(VERSION 3.10)

# set the project name and version
# project(Daemon0 VERSION 0.1)
project(Dameon)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# configure a header file to pass some of the CMake settings
# to the source code
# configure_file(TutorialConfig.h.in TutorialConfig.h)

# TODO 2: Use add_subdirectory() to add MathFunctions to this project
# add_subdirectory(components/socket/udp)
# add_subdirectory(components/ntp)

# add the executable
add_executable(Daemon0 main.cpp)

find_package(Threads REQUIRED)
target_link_libraries(Daemon0 PRIVATE Threads::Threads)

# TODO 3: Use target_link_libraries to link the library to our executable
# target_link_libraries(Tutorial PUBLIC MathFunctions)

# TODO 4: Add MathFunctions to Tutorial's target_include_directories()
# Hint: ${PROJECT_SOURCE_DIR} is a path to the project source. AKA This folder!

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Daemon0 PUBLIC
							"${PROJECT_SOURCE_DIR}/components/socket/tcp"
							"${PROJECT_SOURCE_DIR}/components/socket/udp"
							"${PROJECT_SOURCE_DIR}/components/ntp"
							"${PROJECT_BINARY_DIR}"
							)
