add_subdirectory("src")
add_library( lemmacore ${SOURCE} ) 
target_include_directories( lemmacore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

set_target_properties(lemmacore PROPERTIES 
	VERSION  "${LEMMA_VERSION_NOQUOTES}"
	SOVERSION "${LEMMA_VERSION_MAJOR}.${LEMMA_VERSION_MINOR}"
	PROJECT_LABEL "LemmaCore ${LABEL_SUFFIX}"
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

#generate_export_header(lemmacore)

#add_dependencies(lemmacore YAML_CPP)

#if (LEMMA_TINYXML_SUPPORT)
#	add_dependencies(lemmacore TINYXML2)
#endif()
#if (LEMMA_MATIO_SUPPORT)
#	add_dependencies(lemmacore MATIO)
#endif()

target_link_libraries (lemmacore Eigen3::Eigen)
#target_link_libraries (lemmacore CxxTest)
target_link_libraries (lemmacore ${YAML_CPP_LIBRARIES}) 
target_link_libraries (lemmacore ${YAML_CPP_LIBRARY}) 

# Linking
if ( LEMMA_VTK6_SUPPORT OR LEMMA_VTK7_SUPPORT OR LEMMA_VTK8_SUPPORT ) 
	target_link_libraries(lemmacore ${VTK_LIBRARIES})
#	target_link_libraries(lemmacore "yaml-cpp")
#	target_link_libraries(lemmacore "matplot")
endif()

# find_package(yaml-cpp) does not seem to properly define library names...
# a better solution than this is welcome 

#if (WIN32)
#	target_link_libraries(lemmacore "libyaml-cppmdd") 
#else()
#	target_link_libraries(lemmacore "yaml-cpp") 
#endif()

# Testing
if (LEMMA_ENABLE_TESTING)
	add_subdirectory(testing)
endif()

# Python Bindings
if (LEMMA_PYTHON3_BINDINGS)
	add_subdirectory(python)
endif()

# Install
install ( TARGETS lemmacore        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
install ( FILES include/LemmaCore  DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma ) 
install ( DIRECTORY include/       DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Lemma  FILES_MATCHING PATTERN "*.h" )

# Examples
if (LEMMA_BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
