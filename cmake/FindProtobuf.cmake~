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
FIND_PATH( PROTOBUF_INCLUDE google/protobuf/descriptor.h
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	DOC "The directory where google/protobuf/descriptor.h resides")

FIND_LIBRARY( PROTOBUF_LIBRARY
	NAMES protobuf
	PATHS
	/usr/lib64
	/usr/lib
	/usr/local/lib64
	/usr/local/lib
	/sw/lib
	/opt/local/lib
	DOC "The protobuf library")

IF (PROTOBUF_INCLUDE)
	SET( PROTOBUF_FOUND 1 CACHE STRING "Set to 1 if PROTOBUF is found, 0 otherwise")
ELSE (PROTOBUF_INCLUDE)
	SET( PROTOBUF_FOUND 0 CACHE STRING "Set to 1 if PROTOBUF is found, 0 otherwise")
ENDIF (PROTOBUF_INCLUDE)

MARK_AS_ADVANCED( PROTOBUF_FOUND )
