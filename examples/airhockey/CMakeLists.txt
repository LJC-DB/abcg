project(airhockey)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp player.cpp board.cpp puck.cpp)

enable_abcg(${PROJECT_NAME})
