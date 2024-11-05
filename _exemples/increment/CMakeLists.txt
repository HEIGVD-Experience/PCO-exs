# Version minimale de CMake requise
cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(increment_project)

# Définition du standard C++
set(CMAKE_CXX_STANDARD 11)

# Ajout du fichier source
add_executable(increment_exec main.cpp)
