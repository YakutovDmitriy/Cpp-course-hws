cmake_minimum_required(VERSION 3.3)

set(HEADERS format.h)
set(SOURCES format.cpp)
add_library(format ${SOURCES} ${HEADERS})
install (TARGETS format DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install (FILES format.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install (FILES format.cpp DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
