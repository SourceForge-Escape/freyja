#!/bin/bash
# Update Lua and Python API bindings via metaprogramming.
# - Terry 'Mongoose' Hendrix II

python scripts/lua_api.py > freyja/LuaABI.cpp
python scripts/python_api.py > freyja/PythonABI.cpp

