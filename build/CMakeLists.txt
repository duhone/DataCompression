cmake_minimum_required (VERSION 2.6)
project (DataCompression)

add_subdirectory("${PROJECT_SOURCE_DIR}/../../platform/build" "${PROJECT_SOURCE_DIR}/../../platform_build")
add_subdirectory("${PROJECT_SOURCE_DIR}/../../math/build" "${PROJECT_SOURCE_DIR}/../../math_build")
include_directories("${PROJECT_SOURCE_DIR}/../../3rdParty/catch")
include_directories("${PROJECT_SOURCE_DIR}/../../3rdParty/lz4/lib")
include_directories("${PROJECT_SOURCE_DIR}/../../3rdParty/zlib")
include_directories("${PROJECT_SOURCE_DIR}/../../3rdParty/lzma/c")
include_directories("${PROJECT_SOURCE_DIR}/../../3rdParty/zstd/lib")
include_directories("${PROJECT_SOURCE_DIR}/../../core/inc")
include_directories("${PROJECT_SOURCE_DIR}/../../platform/inc")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")

###############################################
#options
###############################################

option(IncludeTests "Include Unit Tests" ON)

include_directories("${PROJECT_SOURCE_DIR}/../inc/DataCompression")

###############################################
#library
###############################################
file(GLOB_RECURSE dc_inc "${PROJECT_SOURCE_DIR}/../inc/*.h")
file(GLOB_RECURSE dc_src1 "${PROJECT_SOURCE_DIR}/../src/*.h")
file(GLOB_RECURSE dc_src2 "${PROJECT_SOURCE_DIR}/../src/*.cpp")
list(APPEND dc_src ${dc_inc})
list(APPEND dc_src ${dc_src1})
list(APPEND dc_src ${dc_src2})

add_library(DataCompression OBJECT ${dc_src})
					
set_property(TARGET DataCompression APPEND PROPERTY COMPILE_FLAGS "-WX -W4")
				
if(IncludeTests)  
	###############################################
	#unit tests
	###############################################
	set(dc_test_src
		${PROJECT_SOURCE_DIR}/../tests/BasicCompression.cpp
		${PROJECT_SOURCE_DIR}/../tests/main.cpp
	)
	
	add_executable(DataCompression_tests $<TARGET_OBJECTS:DataCompression> $<TARGET_OBJECTS:platform> 
		$<TARGET_OBJECTS:core> $<TARGET_OBJECTS:lz4> $<TARGET_OBJECTS:zlib> $<TARGET_OBJECTS:lzma> $<TARGET_OBJECTS:zstd> ${dc_test_src})
						
	set_property(TARGET DataCompression_tests APPEND PROPERTY FOLDER tests)
	set_property(TARGET DataCompression_tests APPEND PROPERTY COMPILE_FLAGS "-WX -W4")
endif()
