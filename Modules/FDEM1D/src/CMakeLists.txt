set (FDEM1DSOURCE
	${FDEM1DSOURCE}

	# model space
	${CMAKE_CURRENT_SOURCE_DIR}/LayeredEarthEM.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LayeredEarthEMReader.cpp
	
	# Calculation points
	${CMAKE_CURRENT_SOURCE_DIR}/FieldPoints.cpp

	# Sources	
	${CMAKE_CURRENT_SOURCE_DIR}/CompactSupportEMSource.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/CircularLoop.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/DipoleSource.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/WireAntenna.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PolygonalWireAntenna.cpp

	# Kernel management	
	${CMAKE_CURRENT_SOURCE_DIR}/KernelEM1DManager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/KernelEM1DSpec.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/KernelEM1DReflSpec.cpp

	#####################
	# Hankel transforms #
	#####################

	# FHT
	${CMAKE_CURRENT_SOURCE_DIR}/HankelTransform.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FHTAnderson801.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FHTKey201.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FHTKey101.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FHTKey51.cpp

	# Templated FHT
	${CMAKE_CURRENT_SOURCE_DIR}/FHT.cpp

	# Gaussian Quadrature
	${CMAKE_CURRENT_SOURCE_DIR}/GQChave.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/QWEKey.cpp
	
	######################
	# Sin/Cos transforms #
	######################
	
	${CMAKE_CURRENT_SOURCE_DIR}/IntegrationKernel.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/DigitalFilterIntegratorAnderson.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FastSinTransformAnderson.cpp
		
	# Calculation
	${CMAKE_CURRENT_SOURCE_DIR}/EMEarth1D.cpp

	# AEM 
	${CMAKE_CURRENT_SOURCE_DIR}/AEMSurvey.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AEMSurveyReader.cpp
	
	#${CMAKE_CURRENT_SOURCE_DIR}/UngroundedElectricDipole.cpp
	PARENT_SCOPE
)
