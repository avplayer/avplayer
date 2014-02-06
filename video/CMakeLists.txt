
find_package(SDL2  REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})


add_library(avvideo STATIC sdl_render.cpp)


target_link_libraries(avvideo -lswscale )
target_link_libraries(avvideo ${SDL2_LIBRARIES})
