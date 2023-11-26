cmake_minimum_required(VERSION 3.20)
project(libarchive-example)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBARCHIVE REQUIRED libarchive>3.0)
pkg_check_modules(LIBZIP REQUIRED libzip>1.0)

add_executable(create_test_archive src/create_test_archive.cpp)
target_compile_options(create_test_archive PRIVATE ${LIBZIP_CFLAGS}  ${LIBZIP_CFLAGS_OTHER})
target_include_directories(create_test_archive PRIVATE ${LIBZIP_INCLUDE_DIRS})
target_link_libraries(create_test_archive PRIVATE ${LIBZIP_LINK_LIBRARIES})



add_executable(libarchive_list_archive src/libarchive/list_archive.cpp)
target_compile_options(libarchive_list_archive PRIVATE ${LIBARCHIVE_CFLAGS}  ${LIBARCHIVE_CFLAGS_OTHER})
target_include_directories(libarchive_list_archive PRIVATE ${LIBARCHIVE_INCLUDE_DIRS})
target_link_libraries(libarchive_list_archive PRIVATE ${LIBARCHIVE_LINK_LIBRARIES})

add_executable(libzip_list_archive src/libzip/list_archive.cpp)
target_compile_options(libzip_list_archive PRIVATE ${LIBZIP_CFLAGS}  ${LIBZIP_CFLAGS_OTHER})
target_include_directories(libzip_list_archive PRIVATE ${LIBZIP_INCLUDE_DIRS})
target_link_libraries(libzip_list_archive PRIVATE ${LIBZIP_LINK_LIBRARIES})
