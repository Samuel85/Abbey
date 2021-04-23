cmake_minimum_required(VERSION 3.5)

if (VITA)
	add_compile_definitions(VITA)

	if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
	  if(DEFINED ENV{VITASDK})
	    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
	  else()
	    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
	  endif()
	endif()
endif()

project (abbey)


if(RG350)
	add_compile_definitions(RG350)
	SET(CMAKE_C_COMPILER   "/opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc")
	SET(CMAKE_CXX_COMPILER "/opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-g++")
	SET(CMAKE_RANLIB "/opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-ranlib")
	SET(CMAKE_LINKER "/opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-ld")
	SET(CMAKE_FIND_ROOT_PATH "/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot")
	SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
	add_compile_definitions(RG350)
endif(RG350)

if (VITA)
	include("${VITASDK}/share/vita.cmake" REQUIRED)
	set(VITA_APP_NAME "Abbey")
	set(VITA_TITLEID  "ABBEY0001")
	set(VITA_VERSION  "01.00")
endif(VITA)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wl,-q -Wall -fno-lto -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wl,-q -Wall -fno-lto -std=c++11")

file(GLOB_RECURSE vigasoco_core_SOURCES "vigasoco/*.cpp")
file(GLOB_RECURSE vigasoco_SOURCES "src/*.cpp")

include_directories(
  src
  vigasoco
  SDL2
)

add_executable(${PROJECT_NAME} 
 ${vigasoco_SOURCES} 
 ${vigasoco_core_SOURCES}
 )

if (VITA)
target_link_libraries(${PROJECT_NAME}
  SDL2_mixer
  SDL2
  vita2d
  SceDisplay_stub
  SceCtrl_stub
  SceAudio_stub
  SceSysmodule_stub
  SceGxm_stub
  SceCommonDialog_stub
  SceAppMgr_stub
  SceTouch_stub
  SceHid_stub
  SceMotion_stub
  m
  FLAC
  mikmod
  mpg123
  freetype
  png
  vorbisfile
  vorbis
  ogg
  z
)
else(VITA)
target_link_libraries(${PROJECT_NAME}
  SDL2_mixer
  SDL2
)
endif(VITA)

########################################
## Create packages for VITA and the RG350
########################################
if (VITA)
vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE build_vita/sce_sys/icon0.png sce_sys/icon0.png
  FILE build_vita/sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE build_vita/sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE build_vita/sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  FILE build_vita/gamecontrollerdb.txt gamecontrollerdb.txt
  FILE build/fonts/arial.ttf fonts/arial.ttf
  FILE build/roms/abadia/abadia.dsk roms/abadia/abadia.dsk
  FILE build/roms/abadia/GraficosCPC roms/abadia/GraficosCPC
  FILE build/roms/abadia/GraficosVGA roms/abadia/GraficosVGA
  FILE build/roms/abadia/abrir.wav roms/abadia/abrir.wav
  FILE build/roms/abadia/aporrear.wav roms/abadia/aporrear.wav
  FILE build/roms/abadia/campanas.wav roms/abadia/campanas.wav
  FILE build/roms/abadia/cerrar.wav roms/abadia/cerrar.wav
  FILE build/roms/abadia/coger.wav roms/abadia/coger.wav
  FILE build/roms/abadia/dejar.wav roms/abadia/dejar.wav
  FILE build/roms/abadia/espejo.wav roms/abadia/espejo.wav
  FILE build/roms/abadia/final.wav roms/abadia/final.wav
  FILE build/roms/abadia/fondo.wav roms/abadia/fondo.wav
  FILE build/roms/abadia/inicio.wav roms/abadia/inicio.wav  
  FILE build/roms/abadia/pasos.wav roms/abadia/pasos.wav
  FILE build/roms/abadia/tintineo.wav roms/abadia/tintineo.wav
) 
endif(VITA)

if (RG350)
add_custom_command(TARGET abbey
  POST_BUILD
  COMMAND sh make_opk.sh
  COMMENT "Creating OPK file..."
)
endif(RG350)
