cmake_minimum_required(VERSION 3.0.0...3.29.3)
project(cube VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
#link_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/")
add_library(
console  
"${CMAKE_CURRENT_SOURCE_DIR}/include/console.h"
"${CMAKE_CURRENT_SOURCE_DIR}/src/console.c")

add_executable(main main.c)
if (UNIX)
target_link_libraries(main m)
endif (UNIX)
#target_link_libraries(main console)
target_link_libraries(main console)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)