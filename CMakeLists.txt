cmake_minimum_required (VERSION 2.8)
set (PROJECT bmp)

set(SOURCES
                bmp.cpp
                bmperror.cpp)

set(HEADERS
		def.h
                bmp.h
                bmperror.h)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

add_library(${PROJECT} SHARED ${SOURCES} ${HEADERS})
