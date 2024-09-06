cmake_minimum_required(VERSION 3.10)

project(LoginApp)

set(CMAKE_CXX_STANDARD 11)

add_executable(LoginApp main.cpp)

find_path(MYSQL_INCLUDE_DIR NAMES mysql.h PATH_SUFFIXES mysql)
find_library(MYSQL_LIBRARY NAMES libmysql libmyslclient PATH_SUFFIXES mysql)

include_directories(${MYSQL_INCLUDE_DIR})
target_link_libraries(LoginApp ${MYSQL_LIBRARY})

if(WIN32)
   target_link_libraries(LoginApp ws2_32)
endif()

if(NOT MYSQL_INCLUDE_DIR OR NOT MYSQL_LIBRARY)
    message(FATAL_ERROR "Could not find MySQL development files.")
endif()