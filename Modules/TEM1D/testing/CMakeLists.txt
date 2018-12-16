
include_directories( ${CXXTEST_INCLUDE_DIR} )

CXXTEST_ADD_TEST(unittest_TEM1D_GetNameCheck GetNameCheck.cc ${CMAKE_CURRENT_SOURCE_DIR}/GetNameCheck.h)
target_link_libraries(unittest_TEM1D_GetNameCheck "lemmacore" "fdem1d" "tem1d")

CXXTEST_ADD_TEST(unittest_TEM1D_SerializeCheck SerializeCheck.cc ${CMAKE_CURRENT_SOURCE_DIR}/SerializeCheck.h)
target_link_libraries(unittest_TEM1D_SerializeCheck "lemmacore" "fdem1d" "tem1d")

set_target_properties( unittest_TEM1D_GetNameCheck 
	               unittest_TEM1D_SerializeCheck
	PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
)
