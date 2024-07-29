cmake_minimum_required(VERSION 3.13)

set(CMAKE_CXX_STANDARD 11)
set(SERVICE_COMPONENTS s3)

project("MaxObject")

set(CMAKE_TOOLCHAIN_FILE "C:/Program Files/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/Program Files/vcpkg/installed/x64-windows")

find_package(AWSSDK REQUIRED COMPONENTS ${SERVICE_COMPONENTS})

add_executable(${PROJECT_NAME} main.cc)

target_link_libraries(${PROJECT_NAME} ${AWSSDK_LINK_LIBRARIES})
