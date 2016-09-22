set (SOURCE 
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/helper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LemmaObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ASCIIParser.cpp                # fixed, key 
	${CMAKE_CURRENT_SOURCE_DIR}/CubicSplineInterpolator.cpp    # fixed, key 
	${CMAKE_CURRENT_SOURCE_DIR}/Data.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DataReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EarthModel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Instrument.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LayeredEarth.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Grid.cpp       
	${CMAKE_CURRENT_SOURCE_DIR}/RectilinearGrid.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RectilinearGridReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RectilinearGridVTKExporter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/WindowFilter.cpp
	PARENT_SCOPE
)
