PROJECT(avalon_simulation)
cmake_minimum_required(VERSION 2.6)

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/.orogen/config")
INCLUDE(avalon_simulationBase)

pkg_check_modules(MARS_GRAPHICS REQUIRED "mars_graphics")
include_directories(${MARS_GRAPHICS_INCLUDE_DIRS})
link_directories(${MARS_GRAPHICS_LIBRARY_DIRS})


INSTALL(FILES ${CMAKE_SOURCE_DIR}/configuration/mars_default.yaml DESTINATION configuration/mars_default/)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/configuration/mars_saveOnClose.yaml DESTINATION configuration/mars_default/)

# FIND_PACKAGE(KDL)
# FIND_PACKAGE(OCL)

