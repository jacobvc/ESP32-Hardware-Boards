#Add sources from ui directory
file(GLOB_RECURSE SRC_UI ${CMAKE_SOURCE_DIR} "*.cpp" "ui/*.c" "ui/*.cpp")

idf_component_register(SRCS "messaging.cpp" "main.cpp" ${SRC_UI}
  INCLUDE_DIRS "." "ui"
  EMBED_FILES "../html/index.html" "../html/favicon.ico" "../html/joy.js")
