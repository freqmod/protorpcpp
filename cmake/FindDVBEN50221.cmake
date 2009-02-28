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
FIND_PATH( DVBEN50221_INCLUDE_PATH libdvben50221/en50221_stdcam.h
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	DOC "The directory wherelibdvben50221/en50221_stdcam.h resides")
FIND_LIBRARY( DVBEN50221_LIBRARY
	NAMES dvben50221
	PATHS
	/usr/lib64
	/usr/lib
	/usr/local/lib64
	/usr/local/lib
	/sw/lib
	/opt/local/lib
	DOC "The DVB50221 library")

IF (DVBEN50221_INCLUDE_PATH)
	SET( DVBEN50221_FOUND 1 CACHE STRING "Set to 1 if DVBEN50221 is found, 0 otherwise")
ELSE (DVBEN50221_INCLUDE_PATH)
	SET( DVBEN50221_FOUND 0 CACHE STRING "Set to 1 if DVBEN50221 is found, 0 otherwise")
ENDIF (DVBEN50221_INCLUDE_PATH)

MARK_AS_ADVANCED( DVBEN50221_FOUND )
