# Find the FFmpeg library
# The file was found in a repostitory as part of: 
# (edited to search for static library files), btw. the file is last edited in 06-jul-2006.
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

  FIND_FILE( FFMPEG_AVCODEC_FILE libavcodec.a
    /usr/lib
    /usr/local/lib
    /usr/lib64
    /usr/local/lib64
  )

  FIND_FILE( FFMPEG_AVFORMAT_FILE libavformat.a
    /usr/lib
    /usr/local/lib
    /usr/lib64
    /usr/local/lib64
  )
    
  FIND_FILE( FFMPEG_AVUTIL_FILE libavutil.a
    /usr/lib
    /usr/local/lib
    /usr/lib64
    /usr/local/lib64
  )
  FIND_FILE( FFMPEG_SWSCALE_FILE libswscale.a
    /usr/lib
    /usr/local/lib
    /usr/lib64
    /usr/local/lib64
  )

  FIND_FILE( FFMPEG_X264_FILE libx264.a
    /usr/lib
    /usr/local/lib
    /usr/lib64
    /usr/local/lib64
  )
  IF( FFMPEG_AVUTIL_FILE )
  IF( FFMPEG_AVCODEC_FILE )
  IF( FFMPEG_AVFORMAT_FILE )
  IF( FFMPEG_SWSCALE_FILE )
    SET( FFMPEG_FOUND "YES" )
    SET( FFMPEG_FILES ${FFMPEG_AVFORMAT_FILE}  ${FFMPEG_AVCODEC_FILE} ${FFMPEG_AVUTIL_FILE} ${FFMPEG_SWSCALE_FILE})
    IF( FFMPEG_X264_FILE )
    	SET( FFMPEG_FILES ${FFMPEG_FILES} ${FFMPEG_X264_FILE})
    ENDIF( FFMPEG_X264_FILE )
    SET( FFMPEG_LIBRARIES ${FFMPEG_FILES} )

  ENDIF( FFMPEG_SWSCALE_FILE )
  ENDIF( FFMPEG_AVFORMAT_FILE )
  ENDIF( FFMPEG_AVCODEC_FILE )
  ENDIF( FFMPEG_AVUTIL_FILE )


ENDIF( FFMPEG_INCLUDE_DIR )
