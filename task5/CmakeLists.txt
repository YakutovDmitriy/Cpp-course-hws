cmake_minimum_required(VERSION 3.6)

add_library(lazy_string lazy_string.cpp lazy_string.h)
install (TARGETS lazy_string DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install (FILES lazy_string.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
install (FILES lazy_string.cpp DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
