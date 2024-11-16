cmake_minimum_required(VERSION 3.20)
project(libarchive-example)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBARCHIVE REQUIRED IMPORTED_TARGET libarchive>3.0)
pkg_check_modules(LIBZIP REQUIRED IMPORTED_TARGET libzip>1.0)

add_executable(create_test_archive src/create_test_archive.cpp)
target_link_libraries(create_test_archive PRIVATE PkgConfig::LIBZIP)


# list archive

add_executable(libarchive_list_archive src/libarchive/list_archive.cpp)
target_link_libraries(libarchive_list_archive PRIVATE PkgConfig::LIBARCHIVE)

add_executable(libzip_list_archive src/libzip/list_archive.cpp)
target_link_libraries(libzip_list_archive PRIVATE PkgConfig::LIBZIP)


# create archive

add_executable(libarchive_create_archive src/libarchive/create_archive.cpp)
target_link_libraries(libarchive_create_archive PRIVATE PkgConfig::LIBARCHIVE)

add_executable(libzip_create_archive src/libzip/create_archive.cpp)
target_link_libraries(libzip_create_archive PRIVATE PkgConfig::LIBZIP)


# extract file

add_executable(libarchive_extract_file src/libarchive/extract_file.cpp)
target_link_libraries(libarchive_extract_file PRIVATE PkgConfig::LIBARCHIVE)

add_executable(libzip_extract_file src/libzip/extract_file.cpp)
target_link_libraries(libzip_extract_file PRIVATE PkgConfig::LIBZIP)


# add files

add_executable(libzip_add_files src/libzip/add_files.cpp)
target_link_libraries(libzip_add_files PRIVATE PkgConfig::LIBZIP)
