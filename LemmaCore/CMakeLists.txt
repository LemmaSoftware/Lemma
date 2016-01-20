include_directories(${CMAKE_INSTALL_PREFIX}/include)

add_compile_options(-std=c++11) 

# TODO fill all the files

add_library( lemmacore  "lemma.cpp" )

add_dependencies(lemmacore YAML-CPP)
add_dependencies(lemmacore TINYXML2)
add_dependencies(lemmacore MATIO)

