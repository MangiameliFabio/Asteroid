file(GLOB_RECURSE Sources "*.h" "*.cpp") # will get all the .cpp and all the .h files in directory and subdirectories
add_executable(SRE_Asteroid ${Sources})

set(PROJ_DIR "${CMAKE_SOURCE_DIR}/project/SRE_Asteroid")

target_link_libraries(SRE_Asteroid ${SGE_LIBRARIES})
target_include_directories(Asteroid PUBLIC ${PROJ_DIR} "${PROJ_DIR}/Engine" "${PROJ_DIR}/Game")

# Copy project data to output path
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


IF (WIN32)
	## Copy dll's to output path
	file(COPY ${SGE_LIBRARIES_SHARED} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ENDIF(WIN32)
