#  ☀️
cmake_minimum_required(VERSION 3.10)

project(lib3d LANGUAGES CXX)

set(SOURCE_FILES src/edge2i.cpp 
	src/face3i.cpp 
	src/frame.cpp 
	src/matrix.cpp 
	src/point2f.cpp 
	src/point3f.cpp 
	src/scene3d.cpp 
	src/simple_mesh.cpp
)

set(HEADER_FILES src/edge2i.h 
	src/face3i.h 
	src/frame.h 
	src/matrix.h 
	src/n_defs.h 
	src/point2f.h 
	src/point3f.h 
	src/scene3d.h 
	src/simple_mesh.h
)


if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
    cmake_policy(SET CMP0072 NEW)
endif()

#if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread -Wall")
#elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -Wall")
#elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
#else()
	set(CMAKE_CXX_STANDARD 17)
#endif()

add_library(3d STATIC ${SOURCE_FILES} ${HEADER_FILES})
