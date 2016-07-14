
CXXTEST_ADD_TEST(unittestFEM1D_GetNameCheck GetNameCheck.cc ${CMAKE_CURRENT_SOURCE_DIR}/GetNameCheck.h)
target_link_libraries(unittestFEM1D_GetNameCheck "lemmacore" "fdem1d")
