cmake_minimum_required(VERSION 3.10...3.31)
project(Cube VERSION 0.1.0 LANGUAGES C)

set(APP_ICON_RESOURCE "${CMAKE_CURRENT_SOURCE_DIR}/icons")

include(CTest)
enable_testing()
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
add_library(
console 
"${CMAKE_CURRENT_SOURCE_DIR}/include/console.h"
"${CMAKE_CURRENT_SOURCE_DIR}/src/console.c")

add_executable(${PROJECT_NAME} main.c "${APP_ICON_RESOURCE}/icon.rc")
target_link_libraries(${PROJECT_NAME} console)

set(CPACK_GENERATOR NSIS)
set(CPACK_NSIS_MODIFY_PATH "ON")
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
set(CPACK_PACKAGE_EXECUTABLES ${PROJECT_NAME};${PROJECT_NAME})
set(CPACK_CREATE_DESKTOP_LINKS ${PROJECT_NAME})

set(subdir "./bin")
set(CPACK_NSIS_MUI_ICON "${APP_ICON_RESOURCE}\\\\cube.ico")
set(CPACK_NSIS_INSTALLED_ICON_NAME "${subdir}/${PROJECT_NAME}.exe")
set(CPACK_NSIS_MUI_UNICON "${APP_ICON_RESOURCE}\\\\cube.ico")

set(CPACK_PACKAGE_ICON "${APP_ICON_RESOURCE}\\\\installer.bmp")
set(CPACK_NSIS_DISPLAY_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Narendra Hindwar")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)