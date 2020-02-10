#  ☀️
cmake_minimum_required(VERSION 3.10)

project(libutils LANGUAGES CXX)

set(SOURCE_FILES
	src/bmpreader.cpp
	src/options.cpp
	src/wavereader.cpp
)

set(HEADER_FILES
	src/bmpreader.h
	src/options.h
	src/wavereader.h
)


if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
    cmake_policy(SET CMP0072 NEW)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -Wall")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -Wall")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
else()
	set(CMAKE_CXX_STANDARD 17)
endif()

add_library(utils STATIC ${SOURCE_FILES} ${HEADER_FILES})
