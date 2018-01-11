include_directories(${CXXTEST_INCLUDE_DIR})

CXXTEST_ADD_TEST(unittest_FEM1D_GetNameCheck GetNameCheck.cc ${CMAKE_CURRENT_SOURCE_DIR}/GetNameCheck.h)
target_link_libraries(unittest_FEM1D_GetNameCheck "lemmacore" "fdem1d" "yaml-cpp")

CXXTEST_ADD_TEST(unittest_FEM1D_SerializeCheck SerializeCheck.cc ${CMAKE_CURRENT_SOURCE_DIR}/SerializeCheck.h)
target_link_libraries(unittest_FEM1D_SerializeCheck "lemmacore" "fdem1d" "yaml-cpp")
