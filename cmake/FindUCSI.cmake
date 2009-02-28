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
FIND_PATH( UCSI_INCLUDE_PATH libucsi/mpeg/iso_639_language_descriptor.h
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	DOC "The directory where libucsi/mpeg/iso_639_language_descriptor.h resides")

FIND_LIBRARY( UCSI_LIBRARY
	NAMES ucsi
	PATHS
	/usr/lib64
	/usr/lib
	/usr/local/lib64
	/usr/local/lib
	/sw/lib
	/opt/local/lib
	DOC "The UCSI library")

IF (UCSI_INCLUDE_PATH)
	SET( UCSI_FOUND 1 CACHE STRING "Set to 1 if UCSI is found, 0 otherwise")
ELSE (UCSI_INCLUDE_PATH)
	SET( UCSI_FOUND 0 CACHE STRING "Set to 1 if UCSI is found, 0 otherwise")
ENDIF (UCSI_INCLUDE_PATH)

MARK_AS_ADVANCED( UCSI_FOUND )
