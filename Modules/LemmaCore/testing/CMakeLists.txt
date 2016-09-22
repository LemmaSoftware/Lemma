#add_executable(testOne testOne.cpp)
#add_executable(testTwo testTwo.cpp)
#add_test( testLemmaCoreOne testOne )
#add_test( testLemmaCoreTwo testTwo )
    	
include_directories(${CXXTEST_INCLUDE_DIR})
CXXTEST_ADD_TEST(unittest_GetNameCheck GetNameCheck.cc ${CMAKE_CURRENT_SOURCE_DIR}/GetNameCheck.h)
target_link_libraries(unittest_GetNameCheck "lemmacore")

CXXTEST_ADD_TEST(unittest_SerializeCheck SerializeCheck.cc ${CMAKE_CURRENT_SOURCE_DIR}/SerializeCheck.h)
target_link_libraries(unittest_SerializeCheck "lemmacore")

CXXTEST_ADD_TEST(unittest_CopyDisableCheck CopyDisableCheck.cc ${CMAKE_CURRENT_SOURCE_DIR}/CopyDisableCheck.h)
target_link_libraries(unittest_CopyDisableCheck "lemmacore")
 
