add_executable( LayeredEarthEM LayeredEarthEM.cpp  )
target_link_libraries(  LayeredEarthEM  "lemmacore" "fdem1d")

INSTALL_TARGETS( "/share/FEM1D/"
	LayeredEarthEM 
)
