add_library(pyFDEM1D MODULE pyFDEM1D.cpp)
target_link_libraries(pyFDEM1D PRIVATE pybind11::module lemmacore fdem1d )
set_target_properties(pyFDEM1D PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                       SUFFIX "${PYTHON_MODULE_EXTENSION}")

