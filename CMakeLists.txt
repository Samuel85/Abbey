cmake_minimum_required (VERSION 3.5)

project (abbey)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${abbey_SOURCE_DIR}/cmake")
set(BIN_DIR ${abbey_SOURCE_DIR}/bin)

SET(CMAKE_SYSTEM_NAME Linux)
option(RG350 "Switching to MIPS compiler" OFF)
if(RG350)
	SET(CMAKE_C_COMPILER   "/opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-gcc")
	SET(CMAKE_CXX_COMPILER "/opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-g++")
	SET(CMAKE_RANLIB "/opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-ranlib")
	SET(CMAKE_LINKER "/opt/gcw0-toolchain/usr/bin/mipsel-gcw0-linux-uclibc-ld")
	SET(CMAKE_FIND_ROOT_PATH "/opt/gcw0-toolchain/usr/mipsel-gcw0-linux-uclibc/sysroot")
	SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
	SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
	SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif(RG350)

find_package(SDL2 REQUIRED)
find_package(SDL2_Mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

include_directories ("src")
file(GLOB_RECURSE vigasoco_core_SOURCES "vigasoco/*.cpp")
include_directories ("vigasoco")
file(GLOB_RECURSE vigasoco_SOURCES "src/*.cpp")



add_executable (abbey ${vigasoco_SOURCES} ${vigasoco_core_SOURCES})
target_link_libraries(abbey ${SDL2_LIBRARY} ${SDL2_MIXER_LIBRARY} ${SDL2_TTF_LIBRARY})
install(TARGETS abbey RUNTIME DESTINATION ${BIN_DIR} )

