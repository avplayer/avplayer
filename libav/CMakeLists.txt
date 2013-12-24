
add_library(av STATIC avplay.c)


target_link_libraries(av ${FFMPEG_LIBRARIES})
