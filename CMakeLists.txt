CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )


SET( PROJ_PATH      ${CMAKE_SOURCE_DIR} )
SET( PROJ_OUT_PATH  ${CMAKE_BINARY_DIR} )


# Section for project specific settings

SET(               PROJ_NAME          "hand" )
SET(               PROJ_LIBRARIES     "Ws2_32" "${PROJ_PATH}/lib/emotiv/edk.dll" )
SET(               PROJ_INCLUDES      "src" "lib" "lib/com" )
FILE( GLOB_RECURSE PROJ_HEADERS        src/*.h lib/*.h lib/com/*.h )
FILE( GLOB_RECURSE PROJ_SOURCES        main.cpp src/*.cpp lib/*.cpp lib/com/*.c )

# End of special section


SET( CMAKE_CXX_FLAGS "-std=c++11" )
IF ( UNIX )
    SET( CMAKE_CXX_FLAGS "-pthread ${CMAKE_CXX_FLAGS}" )
ENDIF ( UNIX )





PROJECT( ${PROJ_NAME} )
INCLUDE_DIRECTORIES( ${PROJ_INCLUDES} )
ADD_EXECUTABLE( ${PROJ_NAME} ${PROJ_SOURCES} )
TARGET_LINK_LIBRARIES( ${PROJ_NAME} ${PROJ_LIBRARIES} )

