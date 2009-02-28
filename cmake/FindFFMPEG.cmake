# Find the FFmpeg library
# The file was found in a repostitory as part of: 
# (it is not edited, except adding this notice), btw. the file is last edited in 06-jul-2006.
# Copyright (c) 1999-2003 Insight Software Consortium
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
#  * Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 
#  * Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 
#  * The name of the Insight Software Consortium, or the names of any
#    consortium members, or of any contributors, may not be used to endorse or
#    promote products derived from this software without specific prior written
#    permission.
# 
#  * Modified source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# Sets
#   FFMPEG_FOUND.  If false, don't try to use ffmpeg
#   FFMPEG_INCLUDE_DIR
#   FFMPEG_LIBRARIES

SET( FFMPEG_FOUND "NO" )


FIND_PATH( FFMPEG_INCLUDE_DIR ffmpeg/avcodec.h
  /usr/include
  /usr/local/include
)

IF( FFMPEG_INCLUDE_DIR )

FIND_PROGRAM( FFMPEG_CONFIG ffmpeg-config
  /usr/bin
  /usr/local/bin
  ${HOME}/bin
)

IF( FFMPEG_CONFIG )
  EXEC_PROGRAM( ${FFMPEG_CONFIG} ARGS "--libs avformat" OUTPUT_VARIABLE FFMPEG_LIBS )
  SET( FFMPEG_FOUND "YES" )
  SET( FFMPEG_LIBRARIES "${FFMPEG_LIBS}" )
  
ELSE( FFMPEG_CONFIG )

  FIND_LIBRARY( FFMPEG_avcodec_LIBRARY avcodec
    /usr/lib
    /usr/local/lib
    /usr/lib64
    /usr/local/lib64
  )

  FIND_LIBRARY( FFMPEG_avformat_LIBRARY avformat
    /usr/lib
    /usr/local/lib
    /usr/lib64
    /usr/local/lib64
  )
  
  IF( FFMPEG_avcodec_LIBRARY )
  IF( FFMPEG_avformat_LIBRARY )

    SET( FFMPEG_FOUND "YES" )
    SET( FFMPEG_LIBRARIES ${FFMPEG_avformat_LIBRARY} ${FFMPEG_avcodec_LIBRARY} )

  ENDIF( FFMPEG_avformat_LIBRARY )
  ENDIF( FFMPEG_avcodec_LIBRARY )

ENDIF( FFMPEG_CONFIG )

ENDIF( FFMPEG_INCLUDE_DIR )
