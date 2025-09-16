cmake_minimum_required(VERSION 3.12)
project(DDoSer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 依赖查找
find_package(PkgConfig REQUIRED)
pkg_check_modules(NCURSES REQUIRED ncurses)
pkg_check_modules(CJSON REQUIRED libcjson)

# 包含目录
include_directories(include)
include_directories(${NCURSES_INCLUDE_DIRS})
include_directories(${CJSON_INCLUDE_DIRS})

# 源代码文件
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# 可执行文件
add_executable(ddoser ${SOURCES} ${HEADERS})

# 链接库
target_link_libraries(ddoser ${NCURSES_LIBRARIES} ${CJSON_LIBRARIES} pthread)

# 安装目标
install(TARGETS ddoser DESTINATION bin)
