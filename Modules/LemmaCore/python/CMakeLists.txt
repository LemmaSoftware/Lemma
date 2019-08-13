# create the lib
#add_subdirectory( ${CMAKE_CURRENT_SOURCE_DIR} )
#pybind11_add_module(pyLemmaCore pyLemmaCore.cpp)

add_library(pyLemmaCore MODULE pyLemmaCore.cpp)
target_link_libraries(pyLemmaCore PRIVATE pybind11::module lemmacore )
set_target_properties(pyLemmaCore PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                         SUFFIX "${PYTHON_MODULE_EXTENSION}")

