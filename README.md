# archive example

This repository contains examples how to use libarchive and libzip to
read, write and manipulate zip archives.

## Examples

| Example | libarchive | libzip |
| ------- | ---------- | ------ |
| [list contents of an archive](doc/list_archive.md) | :white_check_mark: | :white_check_mark: |
| create archive | :soon: | :soon: |
| extract a single file | :soon: | :soon: |
| add a single file to an existing archive | :x: | :soon: |

## Build examples

```bash
cmake -B build
cmake --build build
./build/create_test_archive
```

## References 

- [libarchive](https://www.libarchive.org/)
- [libzip](https://libzip.org/)