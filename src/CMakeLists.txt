add_executable(src sdc_controller.c)
target_link_libraries(src LINK_PUBLIC connections)
target_link_libraries(src LINK_PUBLIC remoteApi)
