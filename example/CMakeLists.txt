cmake_minimum_required(VERSION 3.18.4)
project (Stepper)

# Reference the AdafruitMotor shared library
set ( PROJECT_LINK_LIBS libAdafruitMotor.so )
link_directories(../) 

include_directories (../source)
add_executable (Stepper src/stepper.cpp)
target_link_libraries(Stepper ${PROJECT_LINK_LIBS})
