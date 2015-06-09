cmake_minimum_required (VERSION 2.8)
set (PROJECT bmp)

set(SOURCES
                bmp.cpp)

set(HEADERS
		def.h
                bmp.h)

add_compile_options(-std=c++11)

add_library(${PROJECT} SHARED ${SOURCES} ${HEADERS})
