
#pybind11_add_module(pyFDEM1D   MODULE pyFDEM1D.cpp)
add_library(pyFDEM1D MODULE pyFDEM1D.cpp)
target_link_libraries(pyFDEM1D PRIVATE pybind11::module lemmacore fdem1d)
set_target_properties(pyFDEM1D PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
	                       SUFFIX "${PYTHON_MODULE_EXTENSION}"
                               OUTPUT_NAME "FDEM1D"
)

install(TARGETS pyFDEM1D
	COMPONENT python
	RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/pyLemma/pyLemma/"
	LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/pyLemma/pyLemma/"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/pyLemma/pyLemma/"
)

#install (FILES __init__.py DESTINATION "${CMAKE_INSTALL_PREFIX}/pyLemma/pyLemma/pyFDEM1D")
