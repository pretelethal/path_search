cmake_minimum_required(VERSION 3.5.0)
project(globus VERSION 0.1.0 LANGUAGES C CXX)

set(SOURCES main.cpp path.cpp)
add_executable(my_project_executable ${SOURCES}) 
