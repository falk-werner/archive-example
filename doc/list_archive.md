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

There is a commented example how to list contents of an archive using libarchive
contained in the official [documentation](https://github.com/libarchive/libarchive/wiki/Examples#user-content-List_contents_of_Archive_stored_in_File). Please refer to this for further information.

In order to list the contents of an archive, the following steps are performed:

1. Create a new archive for reading
2. Open the archive
3. Iterate over the archive entries
4. Close the archive

### 1. Create a new archive for reading

```C++
#include <archive.h>
#include <archive_entry.h>

//...

archive * a = archive_read_new();
archive_read_support_filter_all(a);
archive_read_support_format_all(a);
```

Libarchive differentiates between archives created for reading and
archives created for writing. To create a new archive used for
reading `archive_read_new` is used.

The next step is to add filters (compression algorithms) and 
formats (file formats) to support. If the application supports
only a specific set for compression algorithms and / or
file formats, it is a good idea to be specific at this point.

For general purpose applications, all supported compressions
algorithms and file formats can be added as shown above.

### 2. Open the archive

```C++
int rc = archive_read_open_filename(a, filename.c_str(), 10240);
if (rc != ARCHIVE_OK)
{
    std::cerr << "failed to open file" << std::endl;
    exit_code = EXIT_FAILURE;
    continue;
}
```

To open an existing file `archive_read_open_filename` is used. Note
that for compatibility reasons, a buffer size should be provided. As
stated in official example, newer versions of the library will
determine the buffer size depending on the archive file to open.

### 3. Iterate over the archive entries

```C++
archive_entry * entry;
while (archive_read_next_header(a, &entry) == ARCHIVE_OK)
{
    const mode_t type = archive_entry_filetype(entry);
    if (S_ISREG(type))
    {
        char const * const filename = archive_entry_pathname(entry);
        auto const size = archive_entry_size(entry);
        std::cout << size << '\t' << filename << std::endl;
    }
    archive_read_data_skip(a);
}
```

To iterate over the entries of the archive `archive_read_next_header` is
used. On success, it initialized the provided `archive_entry`. There are
access functions to get fields of the entry.

Since the contents of the entry is needed in this use case, it must be
skipped using `archive_read_data_skip`.

### 4. Close the archive

```C++
archive_read_free(a);
```

Finally, `archive_read_free` is used to close the archive.

## List archive using libzip C API

In contrast to libarchive, libzip does not differenciate between read
and write access to an archive. This allows to also modify archives,
which will be covered by a separate example.

In order to list the contents of an archive using libzip, the following
steps are performed:

1. Open the archive
2. Iterate over the entries of an archive
3. Get meta data of each entry and print it
4. Close the archive

### 1. Open the archive

```C++
#include <zip.h>

// ...


zip_t * archive = zip_open(filename.c_str(), ZIP_RDONLY, nullptr);
if (nullptr == archive)
{
    std::cerr << "failed to read archive " << filename << std::endl;
    exit_code = EXIT_FAILURE;
    continue;
}
```

To open an archive `zip_open` is used. Ths functions supports both,
open an archive for reading and open an archive for writing. Flags are
provided to specify the open mode. When only reading is desired,
`ZIP_RDONLY` is used.

### 2. Iterate over the entries of an archive

```C++
zip_int64_t const count = zip_get_num_entries(archive, 0);
for(zip_int64_t j = 0; j < count; j++)
{
    // ...
}
```

Entry of an archive are accessed using index numbers for each entry.
To iterate over the entries, the number of entries is determined
using `zip_ger_num_entries`. Than, a for-loop is used to iterate
over the entries.

### 3. Get meta data of each entry and print it

```C++

bool is_dir(std::string const & filename)
{
    return (!filename.empty()) && ('/' == filename[filename.size() - 1]);
}

// ...

zip_stat_t info;
int const rc = zip_stat_index(archive, j, 0, &info);
zip_uint64_t const file_flags = ZIP_STAT_NAME | ZIP_STAT_SIZE;
if ((rc == 0) && (file_flags == (info.valid & file_flags)) && (!is_dir(info.name)))
{
    std::cout << info.size << '\t' << info.name << std::endl;
}
```

For each entry, meta data is obtaining using `zip_stat_index`. This initializes
the provided `zip_stat_t` struct with the entries meta data. Before a field is
accessed, it's validity must be checked using the `valid` field.

To identify whether an entry describes a file or a directory, the last
character of it's name is checked. If a file ends with a slash (`/`), it
describes a directory.

Note that according to [zip file format specification](https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT) section 4.4.17, forward slashes are used as path separators:

> The name of the file, with optional relative path.
> The path stored MUST NOT contain a drive or
> device letter, or a leading slash.  All slashes
> MUST be forward slashes '/' as opposed to
> backwards slashes '\' for compatibility with Amiga
> and UNIX file systems etc.  If input came from standard
> input, there is no file name field.

Checking the last character of the filename to determin whether it
describes a directory or not is inspired by Java's implementation of
the [ZipEntry.isDirectory](https://github.com/JetBrains/jdk8u_jdk/blob/master/src/share/classes/java/util/zip/ZipEntry.java) method.

### 4. Close the archive

```C++
zip_close(archive);
```

Finally, the archive is closed using `zip_close`.
