add_executable(gobpack gobpack.c)
target_link_libraries(gobpack m)

add_executable(jnbpack jnbpack.c)
target_link_libraries(jnbpack m)

add_executable(jnbunpack jnbunpack.c)
target_link_libraries(jnbunpack ${LIBRARIES} jnb_util)

configure_file(jnbmenu.tcl.in jnbmenu.tcl @ONLY)
