cmake_minimum_required (VERSION 2.6)

project (Lemma)

###################
# External Projects
###################
include(ExternalProject)
# Yaml-cpp

ExternalProject_Add(YAML_CPP
	GIT_REPOSITORY  "https://github.com/jbeder/yaml-cpp.git"
	GIT_TAG  "master"
	UPDATE_COMMAND ""
	PATCH_COMMAND ""
)

#####################
# Lemma Configuration
#####################
project(LEMMA_CXX)

set(LEMMA_VERSION_MAJOR "0")
set(LEMMA_VERSION_MINOR "0")
set(LEMMA_VERSION_PATCH "0")
#set(LEMMA_VERSION "${LEMMA_CPP_VERSION_MAJOR}_${LEMMA_CPP_VERSION_MINOR}_${LEMMA_CPP_VERSION_PATCH}")

configure_file (
  "${PROJECT_SOURCE_DIR}/include/LemmaConfig.h.in"
  "${PROJECT_BINARY_DIR}/include/LemmaConfig.h"
)
include_directories("${PROJECT_BINARY_DIR}/include")


include_directories ("${PROJECT_SOURCE_DIR}/LemmaCore")
add_subdirectory (LemmaCore) 


add_executable(Hello "${PROJECT_SOURCE_DIR}/src/test.cpp")
target_link_libraries(Hello lemmacore)
#enable_testing()


