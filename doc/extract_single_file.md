# Extract a single file from an archive

- Source:
  - libarchive: [libarchive/extract_file.cpp](../src/libarchive/extract_file.cpp)
  - libzip: [libzip/extract_file.cpp](../src/libzip/extract_file.cpp)

## Prerequisites

In order to extract the contents of a zip archive, an archive us needed.

A sample archive names `test.zip` can be created using `create_test_archive`:

```bash
cmake -B build
cmake --build build
./build/create_test_archive
```

Alternativly, a test archive can also be created using the `zip` command line tool:

```bash
zip -r test.zip CMakeLists.txt src
```

## Extract a file using unzip command line tool

```bash
unzip test.zip README.md
```

This command will extract the file `README.md` from
the archive `test.zip`. If the file already exists it
will promp how to proceed.

```bash
unzip -p test.zip README.md 

#README

This is an exmaple
```
 The `unzip` command also allows to display file on `stdout` by using the `-p` option.

 ## Extract a file using libarchive

To extract an existing file using libarchive, the following steps are
performed:

1. Open the archive file
2. Find the file and write it to a file desciptor
4. Close the archive file

### 1. Open the archive file

```C++
#include <archive.h>
#include <archive_entry.h>

// ...

archive * a = archive_read_new();
archive_read_support_filter_all(a);
archive_read_support_format_all(a);

int rc = archive_read_open_filename(a, archive_filename.c_str(), 10240);
if (rc != ARCHIVE_OK)
{
    std::cerr << "failed to open archive" << std::endl;
    archive_free(a);
    return EXIT_FAILURE;
}
```

To open an archive for reading, `archive_read_new` is used. The next step
is to add supported filters and file formats. For general purpose applications
that can deal with a wide variety of filters and file formats, all filters
and or file formats supported by libarchive can be added using `archive_read_support_filter_all` and `archive_read_support_format_all`.
Specialized applications may only add the filters and file formats they need.

After that, the archive file can be opened using `archive_read_open_filename`.
Note that the required buffer size is determined by libarchive dependeing on
the archive type.

### 2. Find the file and write it to a file desciptor

```C++
bool found = false;
archive_entry * entry;
while ((!found) && (archive_read_next_header(a, &entry) == ARCHIVE_OK))
{
    const mode_t type = archive_entry_filetype(entry);
    char const * const current_filename = archive_entry_pathname(entry);

    if ((S_ISREG(type)) && (filename == current_filename))
    {
        rc = archive_read_data_into_fd(a, STDOUT_FILENO);
        if (rc != ARCHIVE_OK)
        {
            std::cerr << "error: failed to read file";
            exit_code = EXIT_FAILURE;
        }
        found = true;
    }
    else
    {
        archive_read_data_skip(a);
    }
}
```

In order to find the file to extract, it is necessary to iterate over all
entries of the archive using `archive_read_next_header`.

Fortunately, there is a convenience function `archive_read_data_into_fd` to
read the whole file into a file descriptor. To read chunks of the file,
take a look at `archive_read_data` in libarchive's documentation.

Note that a call to `archive_read_data_skip` is required to skip
uninteresting entries.

### 3. Close the archive file

```C++
archive_free(a);
```

Finally, the archive is freed using `archive_free`.

## Extract a file using libzip

In order to extract a file using libzip, the following steps are performed:

1. Open the archive file
2. Locate the file in the archive
3. Write the file contents to a file
4. Close the archive

### 1. Open the archive file

```C++
#include <zip.h>

// ...

zip_t * archive = zip_open(archive_filename.c_str(), ZIP_RDONLY, nullptr);
if (nullptr == archive)
{
    std::cerr << "failed to open archive" << std::endl;
    return EXIT_FAILURE;
}
```

To open an archive file for reading, `zip_open` is called using the
`ZIP_RDONLY` flag.

### 2. Locate the file in the archive

```C++
auto const id = zip_name_locate(archive, filename.c_str(), 0);
```

To locate an entry in a zip archive, libzip provides the convenience
function `zip_name_locate`.

### 3. Write the file contents to a file

```C++
auto * const source = zip_fopen_index(archive, id, 0);
if (nullptr == source)
{
    std::cerr << "error: failed to open archived file" << std::endl;
    zip_close(archive);
    return EXIT_FAILURE;
}

char buffer[10240];
auto length = zip_fread(source, buffer, 10240);
while (0 < length)
{
    fwrite(buffer, 1, length, stdout);
    length = zip_fread(source, buffer, 10240);
}

zip_fclose(source);
```

Unfortunately, libzip does not provide any convenience funtion to read
archive entries into files nor file descriptors. Therefore it is necessary
to open the file using `zip_fopen`, consecutively call `zip_fread` and
finally close the archive entry using `zip_fclose`.

### 4. Close the archive

```C++
zip_close(archive);
```

Finally, the archive is closed using `zip_close`.
