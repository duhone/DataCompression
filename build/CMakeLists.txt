cmake_minimum_required (VERSION 2.6)
project (DataCompression)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /WX /W4 /std:c++latest /fp:fast /fp:except-")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /arch:AVX /Oi /Ot /sdl-")

###############################################
#options
###############################################
option(IncludeTests "Include Unit Tests" ON)

include (${CMAKE_CURRENT_SOURCE_DIR}/build.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/build/build_catch.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty/build/build_zstd.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../core/build/build.cmake)
include (${CMAKE_CURRENT_SOURCE_DIR}/../../platform/build/build.cmake)

