 cmake_minimum_required(VERSION 3.2)
 PROJECT(geohash)
 set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "list of supported configurations." FORCE)
# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin)
# 3.head file path，头文件目录
INCLUDE_DIRECTORIES(
inc
src
)
# set_property(_USE_MATH_DEFINES ON)
add_definitions(-D_USE_MATH_DEFINES)
# 4.source directory，源文件目录
AUX_SOURCE_DIRECTORY(src SOURCE_FILES)

# 5.set environment variable，设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
# SET(TEST_MATH
# ${SOURCE_FILES}
# )
set(dir inc)
file (GLOB HEADER_FILES  "${dir}/*.h")
message("My HEADER_FILES: " ${HEADER_FILES})

# 6.add executable file，添加要编译的可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT geohash)

# 7.add link library，添加可执行文件所需要的库，比如我们用到了libm.so（命名规则：lib+name+.so），就添加该库的名称
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)