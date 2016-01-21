add_library( matplot matplot.cpp )
target_link_libraries(matplot ${VTK_LIBRARIES})
install ( TARGETS matplot DESTINATION ${CMAKE_INSTALL_PREFIX}/lib )
