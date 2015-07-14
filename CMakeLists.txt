cmake_minimum_required(VERSION 2.8)

project(JuPedSim)

set(CMAKE_COLOR_MAKEFILE ON)
set(DESIRED_QT_VERSION "4")
add_subdirectory("jpscore")
add_subdirectory("jpsreport")
add_subdirectory("jpsvis")
