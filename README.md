[![Build](https://github.com/falk-werner/archive-example/actions/workflows/build.yml/badge.svg)](https://github.com/falk-werner/archive-example/actions/workflows/build.yml)

# archive example

This repository contains examples how to use libarchive and libzip to
read, write and manipulate zip archives.

## Examples

| Example | libarchive | libzip |
| ------- | ---------- | ------ |
| [list contents of an archive](doc/list_archive.md) | :white_check_mark: | :white_check_mark: |
| [create archive](doc/create_archive.md) | :white_check_mark: | :white_check_mark: |
| [extract a single file](doc/extract_single_file.md) | :white_check_mark: | :white_check_mark: |
| [add files to an existing archive](doc/add_files.md) | :x: | :white_check_mark: |
| create encrypted archive | ? | :white_check_mark: |
| extract encrypted archive | ? | :white_check_mark: |

## Build examples

```bash
cmake -B build
cmake --build build
./build/create_test_archive
```

## References 

- [libarchive](https://www.libarchive.org/)
- [libzip](https://libzip.org/)