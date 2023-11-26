# List files in archive

- Source:
  - libarchive: [libarchive/list_archive.cpp](../src/libarchive/list_archive.cpp)
  - libzip: [libzip/list_archive.cpp](../src/libzip/list_archive.cpp)

## Prerequisites

In order to list the contents of a zip archive, a archive is needed.

A sample archive named `test.zip` can be created using `create_test_archive`:

```bash
cmake -B build
cmake --build build
./build/create_test_archive
```

Alternativly, a test archive can also be created using the `zip` command line tool:

```bash
zip -r test.zip CMakeLists.txt src
```

## List archive using unzip command line tool

```bash
unzip -l test.zip

Archive:  test.zip
  Length      Date    Time    Name
---------  ---------- -----   ----
       27  2023-11-26 15:24   README.md
        0  2023-11-26 15:24   subdir/
       13  2023-11-26 15:24   subdir/some_file.txt
---------                     -------
       40                     3 file
```

## List archive using libarchive C API

## List archive using libzip C API

### Note about file separators

According to [zip file format specification](https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT) section 4.4.17, forward slashes are used as path sepators:

> The name of the file, with optional relative path.
> The path stored MUST NOT contain a drive or
> device letter, or a leading slash.  All slashes
> MUST be forward slashes '/' as opposed to
> backwards slashes '\' for compatibility with Amiga
> and UNIX file systems etc.  If input came from standard
> input, there is no file name field.  
