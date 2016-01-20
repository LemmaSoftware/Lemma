include_directories(${CMAKE_INSTALL_PREFIX}/include)

add_compile_options(-std=c++11 -DHAVE_YAMLCPP) 

#file(GLOB LemmaCore_SRC
#    "${CMAKE_CURRENT_DIR}/src/*.cpp"
#)
add_subdirectory("src")

# TODO fill all the files
add_library( lemmacore ${SOURCE} )  
#		"src/lemma.cpp" 
#		"src/dipolesource.cpp" 
#		"src/GroundedElectricDipole.cpp" 
#		"src/UngroundedElectricDipole.cpp"
#)


add_dependencies(lemmacore YAML-CPP)
add_dependencies(lemmacore TINYXML2)
if (MATIO_SUPPORT)
	add_dependencies(lemmacore MATIO)
endif()
target_link_libraries(lemmacore ${VTK_LIBRARIES})
target_link_libraries(lemmacore "yaml-cpp")
