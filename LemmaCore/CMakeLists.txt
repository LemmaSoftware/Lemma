#add_library(lemmacore  "${PROJECT_SOURCE_DIR}/LemmaCore/lemma.cpp" )


#include_directories ("${PROJECT_SOURCE_DIR}/LemmaCore")
add_library(lemmacore  "lemma.cpp" )
#e) 
#include_directories(LemmaCore)
# "${PROJECT_BINARY_DIR}/LemmaCore")
#  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/mylib>
#  $<INSTALL_INTERFACE:include/mylib>  # <prefix>/include/mylib
#)
