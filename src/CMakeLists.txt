add_executable(ct_complex2num complex2num.c)
add_executable(ct_cutcomplex cutcomplex.c)
add_executable(ct_activitykeep activitykeep.c)
target_link_libraries(ct_activitykeep m)

install(TARGETS ct_complex2num DESTINATION bin)
install(TARGETS ct_cutcomplex DESTINATION bin)
install(TARGETS ct_activitykeep DESTINATION bin)
