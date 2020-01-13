set (SOURCE 
	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/ProgressBar.cpp                # external to Lemma
	${CMAKE_CURRENT_SOURCE_DIR}/helper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LemmaObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ASCIIParser.cpp                # key 
	${CMAKE_CURRENT_SOURCE_DIR}/CubicSplineInterpolator.cpp    # key 
	${CMAKE_CURRENT_SOURCE_DIR}/Grid.cpp                       # ABC, no key 
	${CMAKE_CURRENT_SOURCE_DIR}/RectilinearGrid.cpp            # key
	${CMAKE_CURRENT_SOURCE_DIR}/RectilinearGridReader.cpp      # key
	${CMAKE_CURRENT_SOURCE_DIR}/RectilinearGridVTKExporter.cpp # key
	${CMAKE_CURRENT_SOURCE_DIR}/RectilinearGridVTKImporter.cpp # key
	${CMAKE_CURRENT_SOURCE_DIR}/Filter.cpp                     # ABC, no key
	${CMAKE_CURRENT_SOURCE_DIR}/WindowFilter.cpp			   # key
	${CMAKE_CURRENT_SOURCE_DIR}/EarthModel.cpp                 # ABC, no key
	${CMAKE_CURRENT_SOURCE_DIR}/LayeredEarth.cpp			   # ABC, no key
	${CMAKE_CURRENT_SOURCE_DIR}/Data.cpp                       # ABC, no key
	${CMAKE_CURRENT_SOURCE_DIR}/DataReader.cpp                 # ABC, no key
	PARENT_SCOPE
)
