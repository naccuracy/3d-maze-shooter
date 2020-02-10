#!/usr/bin/env python3
#  ☀️

import sys
import os
import shutil
import platform
import subprocess
import json

#set(PROJECT_NAME awesome_project_name) from CMakeList
program_name = "maze2"

#--------------------------------------------------utilities
def get_cpu_cores_count():
    return os.cpu_count()

def check_if_app_exists(app):
    ver_result = subprocess.run([app, '--version'])
    if ver_result.returncode:
        print("ERROR: ", app, " doesn't installed!")
        sys.exit(1)

def download_and_extract(url_str, dir_path, file_name, ar_ext):
    out_path = os.path.join(dir_path, file_name)
    ar_path = out_path + ar_ext
    if (not os.path.exists(ar_path)):
        print("DOWNLOAD: " + ar_path)
        subprocess.run(["curl", '-L', url_str, '--output', ar_path])

    if (not os.path.exists(out_path)):
        print("EXTRACT: " + ar_path)
        shutil.unpack_archive(filename=ar_path, extract_dir=dir_path, format="gztar")

def get_cmake_target():
    target = 'install'
    #if (platform.system() == "Windows"):
    #    target = 'INSTALL'
    return target

#--------------------------------------------------prepare
#check_if_app_exists("perl")
check_if_app_exists("curl")
check_if_app_exists("cmake")

ROOT_DIR = os.path.realpath(os.path.dirname(__file__))
CMAKE_GENERATOR="Unix Makefiles"
LIBSDL = "SDL2-2.28.1"
LIBOPENAL = "openal-soft-1.23.1"
LIB2D = "2D"
LIB3D = "3D"
LIBUTILS = "utils"
#---------------------------------------------------
LIBS = os.path.join(ROOT_DIR, "libs")
DEST = os.path.join(ROOT_DIR, "build")
LIBSBUILD=os.path.join(DEST, "libs")

if (not os.path.exists(LIBS)):
    os.mkdir(LIBS)
    os.mkdir(os.path.join(LIBS, "include"))
    os.mkdir(os.path.join(LIBS, "lib"))

if (not os.path.exists(DEST)):
    os.mkdir(DEST)

SDL_LIB_FILE = os.path.join(LIBS, "lib", "libSDL2")
LIB2D_LIB_FILE = os.path.join(LIBS, "lib", "lib2d")
LIB3D_LIB_FILE = os.path.join(LIBS, "lib", "lib3d")
UTILS_LIB_FILE = os.path.join(LIBS, "lib", "libutils")
OPENAL_LIB_FILE = os.path.join(LIBS, "lib", "libopenal-soft")

SOSUFFIX = "so"
ASUFFIX = "a"
if (platform.system() == "Windows"):
#    CMAKE_GENERATOR="Visual Studio 16 2019"
    CMAKE_GENERATOR="MinGW Makefiles"
#    SOSUFFIX="dll"
#    ASUFFIX="lib"

if (platform.system() == "Darwin"):
    SOSUFFIX="dylib"

SDL_LIB_FILE += "." + ASUFFIX
LIB2D_LIB_FILE += "." + ASUFFIX
LIB3D_LIB_FILE += "." + ASUFFIX
UTILS_LIB_FILE += "." + ASUFFIX
OPENAL_LIB_FILE += "." + ASUFFIX

if (not os.path.exists(LIBSBUILD)): 
    os.mkdir(LIBSBUILD)

TGZ=".tar.gz"
DEPS_PATH = "./dependecies"
#--------------------------------------------------download libs
download_and_extract('https://github.com/libsdl-org/SDL/releases/download/release-2.28.1/' + LIBSDL + TGZ, LIBSBUILD, LIBSDL, TGZ)
download_and_extract('https://www.openal-soft.org/openal-releases/' + LIBOPENAL + '.tar.bz2', LIBSBUILD, LIBOPENAL, '.tar.bz2')

#--------------------------------------------------build libs
#--------------------------------------------------SDL2
sources_dir = os.path.join(LIBSBUILD, LIBSDL)
if (os.path.exists(sources_dir)):
    print("BUILD: " + SDL_LIB_FILE)
    build_dir = sources_dir + "_build"
    if (not os.path.exists(build_dir)):
        os.mkdir(build_dir)

    subprocess.run(['cmake',
        '-G', CMAKE_GENERATOR,
        '-S', sources_dir,
        '-B', build_dir,
        '-DCMAKE_INSTALL_PREFIX=' + LIBS])
    subprocess.run(['cmake',
        '--build', build_dir,
        '--config', 'Release',
        '--target', get_cmake_target(),
        '-j{}'.format(get_cpu_cores_count())])

#--------------------------------------------------OPENAL-SOFT
sources_dir = os.path.join(LIBSBUILD, LIBOPENAL)
if (os.path.exists(sources_dir)):
    print("BUILD: " + OPENAL_LIB_FILE)
    build_dir = sources_dir + "_build"
    if (not os.path.exists(build_dir)):
        os.mkdir(build_dir)

    subprocess.run(['cmake',
        '-G', CMAKE_GENERATOR,
        '-S', sources_dir,
        '-B', build_dir,
        '-DCMAKE_INSTALL_PREFIX=' + LIBS])
    subprocess.run(['cmake',
        '--build', build_dir,
        '--config', 'Release',
        '--target', get_cmake_target(),
        '-j{}'.format(get_cpu_cores_count())])

#--------------------------------------------------build dependency 2D #TODO use ImGUI
sources_dir = os.path.join(ROOT_DIR, 'dependencies', LIB2D)
if (os.path.exists(sources_dir)):
    lib_file = LIB2D_LIB_FILE
    print("BUILD: " + LIB2D_LIB_FILE)
    build_dir = os.path.join(LIBSBUILD, LIB2D) + "_build"
    if (not os.path.exists(build_dir)):
        os.mkdir(build_dir)
#TODO implement install target in cmake
    subprocess.run(['cmake',
        '-G', CMAKE_GENERATOR,
        '-S', sources_dir,
        '-B', build_dir])
    subprocess.run(['cmake',
        '--build', build_dir,
        '--config', 'Release',
        '-j{}'.format(get_cpu_cores_count())])
    #install
    include_path = os.path.join(LIBS, "include", LIB2D)
    if (not os.path.exists(include_path)):
        os.mkdir(include_path)
    for i in ['interface.h' , 'vector_font.h']:
        shutil.copy2(os.path.join(sources_dir, 'src', i), include_path)
    shutil.copy2(os.path.join(build_dir, 'lib2d.' + ASUFFIX), lib_file)

#--------------------------------------------------build dependency 3D
sources_dir = os.path.join(ROOT_DIR, 'dependencies', LIB3D)
if (os.path.exists(sources_dir)):
    lib_file = LIB3D_LIB_FILE
    print("BUILD: " + LIB3D_LIB_FILE)
    build_dir = os.path.join(LIBSBUILD, LIB3D) + "_build"
    if (not os.path.exists(build_dir)):
        os.mkdir(build_dir)
#TODO implement install target in cmake
    subprocess.run(['cmake',
        '-G', CMAKE_GENERATOR,
        '-S', sources_dir,
        '-B', build_dir])
    subprocess.run(['cmake',
        '--build', build_dir,
        '--config', 'Release',
        '-j{}'.format(get_cpu_cores_count())])
    #install
    include_path = os.path.join(LIBS, "include", LIB3D)
    if (not os.path.exists(include_path)):
        os.mkdir(include_path)
    for i in ['edge2i.h', 'face3i.h', 'frame.h', 'matrix.h', 'n_defs.h', 'point2f.h', 'point3f.h', 'scene3d.h', 'simple_mesh.h']:
        shutil.copy2(os.path.join(sources_dir, 'src', i), include_path)
    shutil.copy2(os.path.join(build_dir, 'lib3d.' + ASUFFIX), lib_file)

#--------------------------------------------------build dependency utils #TODE use json for options
sources_dir = os.path.join(ROOT_DIR, 'dependencies', LIBUTILS)
if (os.path.exists(sources_dir)):
    lib_file = UTILS_LIB_FILE
    print("BUILD: " + UTILS_LIB_FILE)
    build_dir = os.path.join(LIBSBUILD, LIBUTILS) + "_build"
    if (not os.path.exists(build_dir)):
        os.mkdir(build_dir)
#TODO implement install target in cmake
    subprocess.run(['cmake',
        '-G', CMAKE_GENERATOR,
        '-S', sources_dir,
        '-B', build_dir])
    subprocess.run(['cmake',
        '--build', build_dir,
        '--config', 'Release',
        '-j{}'.format(get_cpu_cores_count())])
    #install
    include_path = os.path.join(LIBS, "include", LIBUTILS)
    if (not os.path.exists(include_path)):
        os.mkdir(include_path)
    for i in ['bmpreader.h', 'options.h', 'wavereader.h']:
        shutil.copy2(os.path.join(sources_dir, 'src', i), include_path)
    shutil.copy2(os.path.join(build_dir, 'libutils.' + ASUFFIX), lib_file)

#--------------------------------------------------build project

proj_build=os.path.join(DEST, 'project')
if (not os.path.exists(proj_build)):
    os.mkdir(proj_build)

print("BUILD: generate project for " + CMAKE_GENERATOR)
subprocess.run(['cmake',
    '-G', CMAKE_GENERATOR,
    '-S', ROOT_DIR,
    '-B', proj_build,
    '-DCMAKE_BUILD_TYPE=release'])
print("BUILD: build executable with cmake...")
subprocess.run(['cmake',
    '--build', proj_build,
    '--config','Release',
    '-j{}'.format(get_cpu_cores_count())])

print('path to project: ' + proj_build)

if (platform.system() == "Windows"):
    program_name = "maze2.exe"
executable_path = os.path.join(proj_build, program_name)
if (os.path.exists(executable_path)):
    shutil.move(os.path.join(proj_build, program_name), os.path.join(ROOT_DIR, program_name))
    print('done')
    sys.exit(0)
else:
    print('ERROR: '+ executable_path + ' is not exists!')
    sys.exit(1)

