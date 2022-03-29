cmake_minimum_required(VERSION 3.18.4)
project (AdafruitMotor)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE release)

# Ensure library is position independent (no fixed memory location)
set (CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# Bring in the header files
include_directories (source)

# Bring in the source files
file (GLOB_RECURSE SOURCES "./*.cpp")

# Building shared library to AdafruitMotor.so
add_library(AdafruitMotor SHARED ${SOURCES} source/adafruitsteppermotor.h source/adafruitsteppermotor.cpp)

# Configure default installation location (using `sudo make install`)
install (TARGETS AdafruitMotor DESTINATION ./build)