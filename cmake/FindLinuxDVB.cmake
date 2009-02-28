#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# 
FIND_PATH( LinuxDVB_INCLUDE_PATH linuxS2API/dvb/frontend.h
	/usr/include
	/usr/local/include
	/sw/include
	/opt/local/include
	DOC "The directory wherelibdvben50221/en50221_stdcam.h resides")

IF (LinuxDVB_INCLUDE_PATH)
	SET( LinuxDVB_FOUND 1 CACHE STRING "Set to 1 if LinuxDVB is found, 0 otherwise")
ELSE (LinuxDVB_INCLUDE_PATH)
	SET( LinuxDVB_FOUND 0 CACHE STRING "Set to 1 if LinuxDVB is found, 0 otherwise")
ENDIF (LinuxDVB_INCLUDE_PATH)

MARK_AS_ADVANCED( DVBEN50221_FOUND )
