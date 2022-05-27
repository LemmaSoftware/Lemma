# Builds Matlab executable (mex) files
matlab_add_mex(NAME hantenna SRC hantenna_mex.cpp LINK_TO lemmacore fdem1d)

install(TARGETS hantenna
	COMPONENT python
	RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/matlab/"
	LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/matlab/"
	ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/matlab/"
)
