add_executable( LayeredEarthEM LayeredEarthEM.cpp  )
target_link_libraries(  LayeredEarthEM  "lemmacore" "fdem1d")

add_executable( FieldPoints FieldPoints.cpp  )
target_link_libraries(  FieldPoints  "lemmacore" "fdem1d")

add_executable( PolygonalWireAntenna PolygonalWireAntenna.cpp  )
target_link_libraries(  PolygonalWireAntenna  "lemmacore" "fdem1d")

add_executable( CircularLoop CircularLoop.cpp  )
target_link_libraries(  CircularLoop  "lemmacore" "fdem1d")

add_executable( EMDipEarth1D EMDipEarth1D.cpp  )
target_link_libraries(  EMDipEarth1D  "lemmacore" "fdem1d")

INSTALL_TARGETS( "/share/FEM1D/"
	LayeredEarthEM 
	FieldPoints
	PolygonalWireAntenna
	EMDipEarth1D
    CircularLoop
)
