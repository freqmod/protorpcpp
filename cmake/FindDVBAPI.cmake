#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY
# 

#IF (WIN32)
# 	FIND_PATH( GLEW_INCLUDE_PATH GL/glew.h
# 		$ENV{PROGRAMFILES}/GLEW/include
# 		${PROJECT_SOURCE_DIR}/src/nvgl/glew/include
# 		DOC "The directory where GL/glew.h resides")
# 	FIND_LIBRARY( GLEW_LIBRARY
# 		NAMES glew GLEW glew32 glew32s
# 		PATHS
# 		$ENV{PROGRAMFILES}/GLEW/lib
# 		${PROJECT_SOURCE_DIR}/src/nvgl/glew/bin
# 		${PROJECT_SOURCE_DIR}/src/nvgl/glew/lib
# 		DOC "The GLEW library")
FIND_PATH( DVBAPI_INCLUDE_PATH libdvbapi/dvbca.h
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	DOC "The directory where libdvbapi/dvbca.h resides")

FIND_LIBRARY( DVBAPI_LIBRARY
	NAMES dvbapi
	PATHS
	/usr/lib64
	/usr/lib
	/usr/local/lib64
	/usr/local/lib
	/sw/lib
	/opt/local/lib
	DOC "The DVBCA library")

IF (DVBAPI_INCLUDE_PATH)
	SET( DVBAPI_FOUND 1 CACHE STRING "Set to 1 if DVBAPI is found, 0 otherwise")
ELSE (DVBAPI_INCLUDE_PATH)
	SET( DVBAPI_FOUND 0 CACHE STRING "Set to 1 if DVBAPI is found, 0 otherwise")
ENDIF (DVBAPI_INCLUDE_PATH)

MARK_AS_ADVANCED( DVBAPI_FOUND )
