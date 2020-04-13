set(root "${CMAKE_CURRENT_LIST_DIR}/..")

###############################################
#library
###############################################
set(PUBLIC_HDRS
    ${root}/inc/DataCompression/LosslessCompression.h
)

set(SRCS
    ${root}/src/LosslessCompression.cpp
)

set(BUILD
    ${root}/build/build.cmake
)

add_library(datacompression OBJECT  
	${PUBLIC_HDRS} 
	${SRCS} 
	${BUILD}
)
		
settingsCR(datacompression)	
			
target_precompile_headers(datacompression PRIVATE 
	<3rdParty/zstd.h>
)
			
target_include_directories(datacompression PUBLIC
	"${root}/inc"
)
	
target_link_libraries(datacompression PUBLIC
	zstd
	core
	platform
)
	
###############################################
#unit tests
###############################################
set(PUBLIC_HDRS
)

set(SRCS
    ${root}/tests/BasicCompression.cpp
    ${root}/tests/Main.cpp
)
	
set(BUILD
)

add_executable(datacompression_tests ${SRCS})
				
settingsCR(datacompression_tests)
					
set_property(TARGET datacompression_tests APPEND PROPERTY FOLDER tests)

target_link_libraries(datacompression_tests 
	doctest
	fmt
	zstd
	core
	platform
	datacompression
)

set(TEST_DATA 
	"${root}/tests/data/alice29.txt"
	"${root}/tests/data/bumble_tales.tga" 
	"${root}/tests/data/cp.html" 
	"${root}/tests/data/CS-3C1.tga"
	"${root}/tests/data/data.xml" 
	"${root}/tests/data/Game.cpp"
	"${root}/tests/data/kodim23.tga"
	"${root}/tests/data/lena3.tga"
	"${root}/tests/data/TitleThemeRemix.wav"
)
  
add_custom_command(TARGET datacompression_tests POST_BUILD        
COMMAND ${CMAKE_COMMAND} -E copy_if_different  
	${TEST_DATA}
	$<TARGET_FILE_DIR:datacompression_tests>)
	
