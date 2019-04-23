# include directories
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/python)

# create the lib
#add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR} )
#pybind11_add_module(pyLemmaCore pyLemmaCore.cpp)

add_library(pyLemmaCore MODULE pyLemmaCore.cpp)
target_link_libraries(pyLemmaCore PRIVATE pybind11::module lemmacore )
set_target_properties(pyLemmaCore PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                         SUFFIX "${PYTHON_MODULE_EXTENSION}")

#target_include_directories( lemmacore PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../include" )

#add_library(pyLemmaCore SHARED ${CMAKE_CURRENT_SOURCE_DIR}/pyLemmaCore.cpp)

# link
#target_link_libraries(pyLemmaCore lemmacore)

# Copy the __init__.py file
#configure_file(__init__.py ${CMAKE_CURRENT_BINARY_DIR}/__init__.py COPYONLY)

# Suppress prefix "lib" because Python does not allow this prefix
#set_target_properties(pyLemmaCore PROPERTIES PREFIX "")

#install(TARGETS pyLemmaCore __init__.py DESTINATION "${PYTHON_INSTALL_PATH}")
