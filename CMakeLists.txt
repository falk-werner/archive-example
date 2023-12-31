cmake_minimum_required(VERSION 3.20)
project(libarchive-example)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall -Wextra -Wpedantic)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBARCHIVE REQUIRED libarchive>3.0)
pkg_check_modules(LIBZIP REQUIRED libzip>1.0)

add_executable(create_test_archive src/create_test_archive.cpp)
target_compile_options(create_test_archive PRIVATE ${LIBZIP_CFLAGS}  ${LIBZIP_CFLAGS_OTHER})
target_include_directories(create_test_archive PRIVATE ${LIBZIP_INCLUDE_DIRS})
target_link_libraries(create_test_archive PRIVATE ${LIBZIP_LINK_LIBRARIES})


# list archive

add_executable(libarchive_list_archive src/libarchive/list_archive.cpp)
target_compile_options(libarchive_list_archive PRIVATE ${LIBARCHIVE_CFLAGS}  ${LIBARCHIVE_CFLAGS_OTHER})
target_include_directories(libarchive_list_archive PRIVATE ${LIBARCHIVE_INCLUDE_DIRS})
target_link_libraries(libarchive_list_archive PRIVATE ${LIBARCHIVE_LINK_LIBRARIES})

add_executable(libzip_list_archive src/libzip/list_archive.cpp)
target_compile_options(libzip_list_archive PRIVATE ${LIBZIP_CFLAGS}  ${LIBZIP_CFLAGS_OTHER})
target_include_directories(libzip_list_archive PRIVATE ${LIBZIP_INCLUDE_DIRS})
target_link_libraries(libzip_list_archive PRIVATE ${LIBZIP_LINK_LIBRARIES})


# create archive

add_executable(libarchive_create_archive src/libarchive/create_archive.cpp)
target_compile_options(libarchive_create_archive PRIVATE ${LIBARCHIVE_CFLAGS}  ${LIBARCHIVE_CFLAGS_OTHER})
target_include_directories(libarchive_create_archive PRIVATE ${LIBARCHIVE_INCLUDE_DIRS})
target_link_libraries(libarchive_create_archive PRIVATE ${LIBARCHIVE_LINK_LIBRARIES})

add_executable(libzip_create_archive src/libzip/create_archive.cpp)
target_compile_options(libzip_create_archive PRIVATE ${LIBZIP_CFLAGS}  ${LIBZIP_CFLAGS_OTHER})
target_include_directories(libzip_create_archive PRIVATE ${LIBZIP_INCLUDE_DIRS})
target_link_libraries(libzip_create_archive PRIVATE ${LIBZIP_LINK_LIBRARIES})


# extract file

add_executable(libarchive_extract_file src/libarchive/extract_file.cpp)
target_compile_options(libarchive_extract_file PRIVATE ${LIBARCHIVE_CFLAGS}  ${LIBARCHIVE_CFLAGS_OTHER})
target_include_directories(libarchive_extract_file PRIVATE ${LIBARCHIVE_INCLUDE_DIRS})
target_link_libraries(libarchive_extract_file PRIVATE ${LIBARCHIVE_LINK_LIBRARIES})

add_executable(libzip_extract_file src/libzip/extract_file.cpp)
target_compile_options(libzip_extract_file PRIVATE ${LIBZIP_CFLAGS}  ${LIBZIP_CFLAGS_OTHER})
target_include_directories(libzip_extract_file PRIVATE ${LIBZIP_INCLUDE_DIRS})
target_link_libraries(libzip_extract_file PRIVATE ${LIBZIP_LINK_LIBRARIES})


# add files

add_executable(libzip_add_files src/libzip/add_files.cpp)
target_compile_options(libzip_add_files PRIVATE ${LIBZIP_CFLAGS}  ${LIBZIP_CFLAGS_OTHER})
target_include_directories(libzip_add_files PRIVATE ${LIBZIP_INCLUDE_DIRS})
target_link_libraries(libzip_add_files PRIVATE ${LIBZIP_LINK_LIBRARIES})
