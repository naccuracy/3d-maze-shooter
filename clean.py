#!/usr/bin/env python3
#  ☀️

import sys
import os
import shutil

def rm_rf_if_exists(dir):
    if (os.path.exists(dir)):
        shutil.rmtree(dir)

root_dir = os.path.realpath(os.path.dirname(__file__))
#---------------------------------------------------
LIBS=os.path.join(root_dir, "libs")
DEST=os.path.join(root_dir, "build")
LIBSBUILD=os.path.join(DEST, "libs")
LIBSDL=os.path.join(LIBSBUILD, "SDL2-2.28.1")
LIBOPENAL=os.path.join(LIBSBUILD, "openal-soft-1.23.1")
LIB2D=os.path.join(LIBSBUILD, "2D")
LIB3D=os.path.join(LIBSBUILD, "3D")
LIBUTILS=os.path.join(LIBSBUILD, "utils")

rm_rf_if_exists(LIBS)
rm_rf_if_exists(os.path.join(DEST, "project"))
rm_rf_if_exists(LIBSDL)
rm_rf_if_exists(LIBSDL + "_build")
rm_rf_if_exists(LIBOPENAL)
rm_rf_if_exists(LIBOPENAL + "_build")
rm_rf_if_exists(LIB2D + "_build")
rm_rf_if_exists(LIB3D + "_build")
rm_rf_if_exists(LIBUTILS + "_build")

print("done")
