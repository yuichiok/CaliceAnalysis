CMAKE_MINIMUM_REQUIRED(VERSION 3.0 FATAL_ERROR)
SET(CMAKE_OSX_ARCHITECTURES "x86_64")

PROJECT(SiWECALAnalysis CXX)

### DEPENDENCIES ############################################################

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
FIND_PACKAGE( ROOT REQUIRED )
SET(CMAKE_CXX_STANDARD 17)
# SET(CMAKE_CXX_STANDARD 14)

FOREACH( pkg ROOT )
    IF( ${pkg}_FOUND )
        INCLUDE_DIRECTORIES( ${${pkg}_INCLUDE_DIRS} )
        LINK_LIBRARIES( ${${pkg}_LIBRARIES} )
        ADD_DEFINITIONS ( ${${pkg}_DEFINITIONS} )
    ENDIF()
ENDFOREACH()

### DOCUMENTATION ###########################################################

OPTION( INSTALL_DOC "Set to OFF to skip build/install Documentation" OFF )

IF( INSTALL_DOC AND EXISTS "${PROJECT_SOURCE_DIR}/doc/CMakeLists.txt" )
    ADD_SUBDIRECTORY( ./doc )
ENDIF()

# definitions to pass to the compiler
ADD_DEFINITIONS( "-c -Wall -fdiagnostics-color -std=gnu++17" )

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR} )

ADD_SUBDIRECTORY( src )
ADD_EXECUTABLE( main.exe main.cc )
TARGET_LINK_LIBRARIES( main.exe Analyzer )

ADD_EXECUTABLE( converter.exe converter.cc )
TARGET_LINK_LIBRARIES( converter.exe Analyzer )