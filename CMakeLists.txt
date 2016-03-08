cmake_minimum_required(VERSION 2.6)
project(jumpnbump)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

set(CMAKE_C_FLAGS "-Wall -Wextra -ffast-math -funroll-loops")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_BUILD_TYPE Debug)

set(PREFIX ${CMAKE_INSTALL_PREFIX})

add_definitions("-Dstricmp=strcasecmp")
add_definitions("-Dstrnicmp=strncasecmp")
add_definitions("-DNDEBUG")
add_definitions("-DUSE_NET")
add_definitions("-DUSE_SDL")
add_definitions("-DZLIB_SUPPORT")
add_definitions("-DBZLIB_SUPPORT")
add_definitions("-D_DEFAULT_SOURCE")
add_definitions("-DPREFIX=\"${PREFIX}\"")

find_package(SDL2 COMPONENTS mixer net REQUIRED)
find_package(BZip2 REQUIRED)
find_package(ZLIB REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2MIXER_INCLUDE_DIR} ${SDL2NET_INCLUDE_DIR} ${BZIP2_INCLUDE_DIR} ${ZLIB_INCLUDE_DIRS})
set(LIBRARIES ${LIBRARIES} m ${SDL2_LIBRARY} ${SDL2MIXER_LIBRARY} ${SDL2NET_LIBRARY} ${BZIP2_LIBRARIES} ${ZLIB_LIBRARY})

include_directories(src)

add_subdirectory(src)
add_subdirectory(extras)

install(FILES data/jumpbump.dat DESTINATION share/jumpnbump)
install(FILES jumpnbump.6 DESTINATION share/man/man6)
