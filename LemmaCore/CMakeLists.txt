include_directories(${CMAKE_INSTALL_PREFIX}/include)

add_compile_options(-std=c++11) # CMake 2.8.12 or newer

# TODO fill all the files

add_library(lemmacore  "lemma.cpp" )
add_dependencies(lemmacore YAML_CPP)

