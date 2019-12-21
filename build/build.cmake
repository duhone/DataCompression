set(datacompression_root "${CMAKE_CURRENT_LIST_DIR}/..")

###############################################
#library
###############################################
set(DC_PUBLIC_HDRS
    ${datacompression_root}/inc/DataCompression/LosslessCompression.h
)

set(DC_SRCS
    ${datacompression_root}/src/LosslessCompression.cpp
)

set(DC_BUILD
    ${datacompression_root}/build/build.cmake
)

add_library(datacompression OBJECT  
	${DC_PUBLIC_HDRS} 
	${DC_SRCS} 
	${DC_BUILD}
)
						
source_group("Public Headers" FILES ${DC_PUBLIC_HDRS})
source_group("Source" FILES ${DC_SRCS})
source_group("Build" FILES ${DC_BUILD})
		
target_include_directories(datacompression PUBLIC
	"${datacompression_root}/inc"
)
	
target_link_libraries(datacompression 
	zstd
	core
	platform
)
	
if(IncludeTests)
	###############################################
	#unit tests
	###############################################
set(DC_TEST_SRCS
    ${datacompression_root}/tests/BasicCompression.cpp
    ${datacompression_root}/tests/Main.cpp
)
	
	add_executable(datacompression_tests ${DC_TEST_SRCS})
						
	source_group("Source" FILES ${DC_TEST_SRCS})
						
	set_property(TARGET datacompression_tests APPEND PROPERTY FOLDER tests)
	
	target_link_libraries(datacompression_tests 
		catch
		fmt
		zstd
		core
		platform
		datacompression
	)
	
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
