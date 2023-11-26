# List files in archive

- Source:
  - libarchive: [libarchive/list_archive.cpp](../src/libarchive/list_archive.cpp)
  - libzip: [libzip/list_archive.cpp](../src/libzip/list_archive.cpp)

## Prerequisites

## List archive using unzip command line tool

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
