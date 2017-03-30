cmake_minimum_required(VERSION 3.6)

add_library(format format.h)
install (TARGETS lazy_string DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install (FILES lazy_string.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
