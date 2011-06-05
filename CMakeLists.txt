PROJECT(avalon_simulation)
cmake_minimum_required(VERSION 2.6)

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/.orogen/config")
INCLUDE(avalon_simulationBase)


INSTALL(FILES ${CMAKE_SOURCE_DIR}/configuration/mars_default.yaml DESTINATION configuration/mars-default/)
INSTALL(FILES ${CMAKE_SOURCE_DIR}/configuration/mars_saveOnClose.yaml DESTINATION configuration/mars-default/)

# FIND_PACKAGE(KDL)
# FIND_PACKAGE(OCL)

