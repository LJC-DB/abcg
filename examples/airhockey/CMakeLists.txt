project(airhockey)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp player.cpp)

enable_abcg(${PROJECT_NAME})
