add_executable( LayeredEarthEM LayeredEarthEM.cpp  )
target_link_libraries(  LayeredEarthEM  "lemmacore" "fdem1d")
set_property(TARGET LayeredEarthEM PROPERTY CXX_STANDARD 14)

add_executable( FieldPoints FieldPoints.cpp  )
target_link_libraries(  FieldPoints  "lemmacore" "fdem1d")
set_property(TARGET FieldPoints PROPERTY CXX_STANDARD 14)

add_executable( PolygonalWireAntenna PolygonalWireAntenna.cpp  )
target_link_libraries(  PolygonalWireAntenna  "lemmacore" "fdem1d")
set_property(TARGET PolygonalWireAntenna PROPERTY CXX_STANDARD 14)

add_executable( CircularLoop CircularLoop.cpp  )
target_link_libraries(  CircularLoop  "lemmacore" "fdem1d")
set_property(TARGET CircularLoop PROPERTY CXX_STANDARD 14)

add_executable( EMDipEarth1D EMDipEarth1D.cpp  )
target_link_libraries(  EMDipEarth1D  "lemmacore" "fdem1d")
set_property(TARGET EMDipEarth1D PROPERTY CXX_STANDARD 14)

INSTALL_TARGETS( "/share/FEM1D/"
	LayeredEarthEM 
	FieldPoints
	PolygonalWireAntenna
	EMDipEarth1D
    CircularLoop
)
