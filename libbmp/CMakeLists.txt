cmake_minimum_required (VERSION 2.8)
set (PROJECT bmp)

set(SOURCES
                bmp.cpp
                bmperror.cpp)

set(HEADERS
		def.h
                bmp.h
                bmperror.h)
add_library(${PROJECT} SHARED ${SOURCES} ${HEADERS})
