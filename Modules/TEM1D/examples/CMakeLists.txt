add_executable( TEMSurvey TEMSurvey.cpp  )
target_link_libraries( TEMSurvey  "lemmacore" "fdem1d" "tem1d")
set_property(TARGET TEMSurvey PROPERTY CXX_STANDARD 14)

INSTALL_TARGETS( "/share/TEM1D/"
	TEMSurvey
)

#install (DIRECTORY inp  DESTINATION "${CMAKE_INSTALL_PREFIX}/share/TEM1D/" )
