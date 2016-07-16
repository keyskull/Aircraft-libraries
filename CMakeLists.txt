cmake_minimum_required(VERSION 3.5)
project(Aircraft_libraries)

include_directories(
        c_environment/hardware/arduino/cores/arduino
        c_environment/hardware/arduino/variants/sunxi
        Hardware_Driver/Includes
        Includes
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES  libAircraft.c)


add_executable(Aircraft_libraries ${SOURCE_FILES})