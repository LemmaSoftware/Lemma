set (SOURCE 
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/helper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LemmaObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ASCIIParser.cpp                # fixed, key 
	${CMAKE_CURRENT_SOURCE_DIR}/CubicSplineInterpolator.cpp    # fixed, key 
	${CMAKE_CURRENT_SOURCE_DIR}/Grid.cpp                       # ABC, no key 
	${CMAKE_CURRENT_SOURCE_DIR}/RectilinearGrid.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RectilinearGridReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RectilinearGridVTKExporter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Filter.cpp                     # ABC, no key
	${CMAKE_CURRENT_SOURCE_DIR}/WindowFilter.cpp			   # fixed, key
	${CMAKE_CURRENT_SOURCE_DIR}/EarthModel.cpp                 # ABC, no key
	${CMAKE_CURRENT_SOURCE_DIR}/LayeredEarth.cpp			   # ABC, no key
	${CMAKE_CURRENT_SOURCE_DIR}/Data.cpp                       # ABC, no key
	${CMAKE_CURRENT_SOURCE_DIR}/DataReader.cpp                 # ABC, no key
	${CMAKE_CURRENT_SOURCE_DIR}/Instrument.cpp                 # ABC, no key
	PARENT_SCOPE
)
