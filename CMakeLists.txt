#	  ☀️
cmake_minimum_required (VERSION 3.10)

project (maze2 LANGUAGES CXX)

set (SOURCE_FILES
				src/about_form.cpp
				src/game.cpp
				src/lmain.cpp
				src/my_form1.cpp
				src/my_interface.cpp
				src/opt_form.cpp
)
set (HEADER_FILES
				src/about_form.h
				src/game.h
				src/my_form1.h
				src/my_interface.h
				src/opt_form.h
)

if (UNIX AND NOT APPLE)
    set (LINUX TRUE)
    cmake_policy (SET CMP0072 NEW)
endif()

#if (WIN32)
#    set(ICON firework.rc)
#elseif (APPLE)
#    set(ICON firework.icns)
#endif()

#set(FW_ICON rc/${ICON})
#set(FW_RESOURCES rc/input.txt)

#set(FW_TEXTURES  rc/textures/cursor.png
#                 rc/textures/dot.png
#                 rc/textures/fon.png
#)

set (LIBSUFFIX "a")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -Wall")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread -Wall")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
    set (LIBSUFFIX "lib")
else ()
	set (CMAKE_CXX_STANDARD 17)
endif ()

set (CMAKE_PREFIX_PATH "./libs")

#g++ -std=c++0x -I $LIBS/libpng -I $LIBS/zlib -pthread -Wall -g $SRC -lpthread -lGL -lGLU -lX11 $LIBS/libpng/libpng16.a $LIBS/zlib/libz.a -o 1;
find_package (OpenGL REQUIRED)
find_package (SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package (SDL2 REQUIRED CONFIG COMPONENTS SDL2main)
find_package (OpenAL REQUIRED)

if (MINGW OR MSVC)
    add_compile_definitions(WINDOWS_PLATFORM)
    set (ZLIB_NAME libzlibstatic)
else ()
    set (ZLIB_NAME libz)
endif ()


set(LIBS "./libs/lib")
set (LIBRARIES 
            -lm
            lib2d.${LIBSUFFIX}
            lib3d.${LIBSUFFIX}
            libutils.${LIBSUFFIX}
            ${OPENGL_LIBRARIES}
)
set (LIBS_HEADERS "./libs/include")

if (LINUX)
    set (LIBRARIES  ${LIBRARIES} -lrt -ldl -lX11)
endif (LINUX)

if (APPLE)
    set (CMAKE_EXE_LINKER_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit")
endif (APPLE)

set (LIBRARIES  ${LIBRARIES} -lpthread)

#add_compile_definitions (FILES_PREFIX="${CMAKE_CURRENT_SOURCE_DIR}")


include_directories (${LIBS_HEADERS})
link_directories (${LIBS})
#add_executable(maze WIN32 MACOSX_BUNDLE ${HEADER_FILES} ${SOURCE_FILES} ${FW_ICON} ${FW_RESOURCES} ${FW_TEXTURES})
add_executable (maze2 WIN32 MACOSX_BUNDLE ${HEADER_FILES} ${SOURCE_FILES})

if(TARGET SDL2::SDL2main)
    set (LIBRARIES  ${LIBRARIES} SDL2::SDL2main)
    #target_link_libraries (maze2 PRIVATE SDL2::SDL2main)
endif()
set (LIBRARIES  ${LIBRARIES} SDL2::SDL2)
if(TARGET OpenAL::OpenAL)
	set (LIBRARIES  ${LIBRARIES} OpenAL::OpenAL)
else()
	set (LIBRARIES  ${LIBRARIES} -lopenal)
endif()
message (STATUS "LIBRARIES ${LIBRARIES}")

target_link_libraries (maze2 ${LIBRARIES})

#if (APPLE)
#    set_target_properties(mazw PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Maze")
#
#    set_source_files_properties(${FW_ICON}
#                                PROPERTIES
#				MACOSX_PACKAGE_LOCATION "Resources")
#    set_source_files_properties(${FW_RESOURCES}
#                                PROPERTIES
#				MACOSX_PACKAGE_LOCATION "Resources/rc")
#    set_source_files_properties(${FW_TEXTURES}
#                                PROPERTIES
#				MACOSX_PACKAGE_LOCATION "Resources/rc/textures")
#    set_target_properties(firework PROPERTIES
#                          MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
#                          MACOSX_BUNDLE_LONG_VERSION_STRING "1.0.0"
#			  MACOSX_BUNDLE_ICON_FILE ${ICON} 
#			  MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/src/MacOSXBundleInfo.plist.in")
#endif()
