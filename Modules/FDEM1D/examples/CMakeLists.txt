add_executable( LayeredEarthEM LayeredEarthEM.cpp  )
target_link_libraries(  LayeredEarthEM  "lemmacore" "fdem1d")

add_executable( FieldPoints FieldPoints.cpp  )
target_link_libraries(  FieldPoints  "lemmacore" "fdem1d")

INSTALL_TARGETS( "/share/FEM1D/"
	LayeredEarthEM 
	FieldPoints
)
