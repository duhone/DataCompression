include_directories("${CMAKE_CURRENT_LIST_DIR}/../inc")

###############################################
#library
###############################################
set(DC_PUBLIC_HDRS
    ${CMAKE_CURRENT_LIST_DIR}/../inc/DataCompression/LosslessCompression.h
)

set(DC_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/../src/LosslessCompression.cpp
)

set(DC_BUILD
    ${CMAKE_CURRENT_LIST_DIR}/../build/build.cmake
)

add_library(datacompression OBJECT  ${DC_PUBLIC_HDRS} ${DC_SRCS} ${DC_BUILD})
						
source_group("Public Headers" FILES ${DC_PUBLIC_HDRS})
source_group("Source" FILES ${DC_SRCS})
source_group("Build" FILES ${DC_BUILD})
				
if(IncludeTests)
	###############################################
	#unit tests
	###############################################
set(DC_TEST_SRCS
    ${CMAKE_CURRENT_LIST_DIR}/../tests/BasicCompression.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../tests/Main.cpp
)
	
	add_executable(datacompression_tests $<TARGET_OBJECTS:core>
						$<TARGET_OBJECTS:platform> 
						$<TARGET_OBJECTS:datacompression>
						$<TARGET_OBJECTS:zstd> 
						$<TARGET_OBJECTS:fmt> 
						${DC_TEST_SRCS})
						
	source_group("Source" FILES ${DC_TEST_SRCS})
						
	set_property(TARGET datacompression_tests APPEND PROPERTY FOLDER tests)
	
	add_custom_command(TARGET datacompression_tests POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        "${CMAKE_CURRENT_LIST_DIR}/../tests/data/alice29.txt"
        $<TARGET_FILE_DIR:datacompression_tests>)
		
	add_custom_command(TARGET datacompression_tests POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        "${CMAKE_CURRENT_LIST_DIR}/../tests/data/bumble_tales.tga"
        $<TARGET_FILE_DIR:datacompression_tests>)
		
	add_custom_command(TARGET datacompression_tests POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        "${CMAKE_CURRENT_LIST_DIR}/../tests/data/cp.html"
        $<TARGET_FILE_DIR:datacompression_tests>)
		
	add_custom_command(TARGET datacompression_tests POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        "${CMAKE_CURRENT_LIST_DIR}/../tests/data/CS-3C1.tga"
        $<TARGET_FILE_DIR:datacompression_tests>)
		
	add_custom_command(TARGET datacompression_tests POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        "${CMAKE_CURRENT_LIST_DIR}/../tests/data/data.xml"
        $<TARGET_FILE_DIR:datacompression_tests>)
		
	add_custom_command(TARGET datacompression_tests POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        "${CMAKE_CURRENT_LIST_DIR}/../tests/data/Game.cpp"
        $<TARGET_FILE_DIR:datacompression_tests>)
		
	add_custom_command(TARGET datacompression_tests POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        "${CMAKE_CURRENT_LIST_DIR}/../tests/data/kodim23.tga"
        $<TARGET_FILE_DIR:datacompression_tests>)
		
	add_custom_command(TARGET datacompression_tests POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        "${CMAKE_CURRENT_LIST_DIR}/../tests/data/lena3.tga"
        $<TARGET_FILE_DIR:datacompression_tests>)
		
	add_custom_command(TARGET datacompression_tests POST_BUILD        
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  
        "${CMAKE_CURRENT_LIST_DIR}/../tests/data/TitleThemeRemix.wav"
        $<TARGET_FILE_DIR:datacompression_tests>)
endif()
