add_executable(ct_complex2num complex2num.c)
add_executable(ct_cutcomplex cutcomplex.c)

install(TARGETS ct_complex2num DESTINATION bin)
install(TARGETS ct_cutcomplex DESTINATION bin)
