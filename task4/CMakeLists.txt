cmake_minimum_required(VERSION 3.6)

add_library(format format.h)
install (TARGETS format DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install (FILES format.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
