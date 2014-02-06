find_package(SDL2  REQUIRED)
find_package(X11)
find_package( Boost 1.50 COMPONENTS filesystem)

include_directories(${SDL2_INCLUDE_DIR})

add_executable(avplayer player.cpp main.cpp)

target_link_libraries(avplayer av avsource avaudio avaudio avvideo)

if(bt)
	target_link_libraries(avplayer torrent-rasterbar)
endif()

target_link_libraries(avplayer ${X11_LIBRARIES})
target_link_libraries(avplayer ${SDL2_LIBRARIES})
target_link_libraries(avplayer ${Boost_LIBRARIES})
target_link_libraries(avplayer -pthread)

INSTALL(TARGETS avplayer RUNTIME DESTINATION bin)
