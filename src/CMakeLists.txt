set(UTIL_SOURCES
  io/io_read.c
  io/io_write.c

  dat/dat_read.c
  )

add_library(jnb_util ${UTIL_SOURCES})

set(SOURCES
  sdl/gfx.c
  sdl/input.c
  sdl/interrpt.c
  sdl/sound.c

  network.c
  filter.c
  menu.c
  main.c
  )

add_executable(jumpnbump ${SOURCES})
target_link_libraries(jumpnbump ${LIBRARIES} jnb_util)

install(TARGETS jumpnbump DESTINATION bin)
