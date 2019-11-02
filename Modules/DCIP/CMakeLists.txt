option ( LEMMA_MODULE_DCIP TRUE )

if (LEMMA_MODULE_DCIP)

# Configure DCIP 
set(DCIP_VERSION_MAJOR "0")
set(DCIP_VERSION_MINOR "0")
set(DCIP_VERSION_PATCH "1")
set(DCIP_VERSION "\"${DCIP_VERSION_MAJOR}.${DCIP_VERSION_MINOR}.${DCIP_VERSION_PATCH}\"")
set(DCIP_VERSION_NOQUOTES "${DCIP_VERSION_MAJOR}.${DCIP_VERSION_MINOR}.${DCIP_VERSION_PATCH}")



configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/config/DCIPConfig.h.in"
	"${PROJECT_BINARY_DIR}/include/DCIPConfig.h"
	)
install ( FILES  ${PROJECT_BINARY_DIR}/include/DCIPConfig.h   DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma/ )

	add_subdirectory("src")
	add_library( dcip ${DCIPSOURCE} )  
	target_include_directories( dcip PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

	set_target_properties(dcip PROPERTIES 
		VERSION  "${DCIP_VERSION_NOQUOTES}"
		SOVERSION "${DCIP_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}"
		PROJECT_LABEL "DCIP ${LABEL_SUFFIX}"
    	CXX_STANDARD 14
    	CXX_STANDARD_REQUIRED ON
	)

	# Linking
	target_link_libraries(dcip "lemmacore" "fdem1d" )

	# Linking
	target_link_libraries(dcip ${VTK_LIBRARIES})

	# Testing
	#if (LEMMA_ENABLE_TESTING)
	#	add_subdirectory(testing)
	#endif()

	# Install
	install ( TARGETS dcip DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
	install ( FILES include/DCIP  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma ) 
	install ( DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma  FILES_MATCHING PATTERN "*.h")

	#install ( DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma/  FILES_MATCHING PATTERN "FDEM1D")
	#install ( DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma/FDEM1D  FILES_MATCHING PATTERN "*.h")

	# Examples
	#if (LEMMA_BUILD_EXAMPLES)
	#	add_subdirectory(examples)
	#endif()

endif()
