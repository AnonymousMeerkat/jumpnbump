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

include_directories(.)
add_executable(jumpnbump ${SOURCES})
target_link_libraries(jumpnbump ${LIBRARIES})
