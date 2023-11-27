# Create an archive

- Source:
  - libarchive: [libarchive/create_archive.cpp](../src/libarchive/create_archive.cpp)
  - libzip: [libzip/create_archive.cpp](../src/libzip/create_archive.cpp)

## Create archive using zip command line tool

```bash
zip test.zip README.md doc/create_archive.md

  adding: README.md (deflated 55%)
  adding: doc/create_archive.md (stored 0%)
```

The example above shows how to create a new archive `test.zip`
and add some files to it. Note that these files are added or
updated if the archive already exists.

To display the contents of the archive, the `unzip` command
can be used:

```bash
unzip -l test.zip

Archive:  test.zip
  Length      Date    Time    Name
---------  ---------- -----   ----
      811  2023-11-26 15:39   README.md
        0  2023-11-27 16:21   doc/create_archive.md
---------                     -------
      811                     2 files
```

## Create an archive using libarchive

In order to create a zip archive using libarchive, the following
steps are performed:

1. Create a new archive file
2. Add files to the archive
3. Close the archive file

### 1. Create a new archive file

```C++
#include <archive.h>
#include <archive_entry.h>

// ...

archive * a = archive_write_new();
archive_write_set_format_zip(a);

int rc = archive_write_open_filename(a, filename.c_str());
if (rc != ARCHIVE_OK)
{
    std::cerr << "error: failed to create archive" << std::endl;
    archive_write_free(a);
    return EXIT_FAILURE;
}
```

Libarchive differenciates between reading and writing an archive.
To create a new archive for writing `archive_write_new` is used.

The next step is to set filters and the file format. Using filters is
very common for tar file format but not used for zip files. Therefore,
in the example only the file format is set using `archive_write_set_format_zip`.

To open the archive file, `archive_write_open_filename` is used. It
will create a new, empty archive. Note that existing files are
overwritten.

### 2. Add files to the archive

```C++
archive_entry * ae = archive_entry_new();
archive_entry_set_filetype(ae, AE_IFREG);
archive_entry_set_pathname(ae, e.archive_filename.c_str());
archive_entry_set_size(ae, std::filesystem::file_size(e.local_filename));
archive_write_header(a, ae);

FILE * file = fopen(e.local_filename.c_str(), "rb");
if (nullptr != file)
{
    char buffer[10240];
    size_t len = fread(buffer, 1, 10240, file);
    while (len > 0)
    {
        archive_write_data(a, buffer, len);
        len = fread(buffer, 1, 10240, file);
    }
    fclose(file);
}

archive_entry_free(ae);
```

For each file to add, an `archive_entry` is created to store 
meta data of the file. The minimum required information are

- pathname
- file type
- file size

There are more fields available, e.g. file time and access
rights. Note that not all archive formats support every field.

Once the archive entry is constructed it is added to the archive
using `archive_write_header`. The the data of the entry are added.
Since there is no convenience function to add files from disk,
each file must be opened and written using `archive_write_data`.

Finally, the archive entry is freed using `archive_entry_free`.

### 3. Close the archive file

```C++
archive_write_close(a);
archive_write_free(a);
```

As stated in the [official libarchive example](https://github.com/libarchive/libarchive/wiki/Examples), it is perfectly fine to omit
`archive_write_close` since `archive_write_free` will call it when
needed. The difference is that `archive_write_close` has a return
code that can be used to dermine whether the operation succeeded.

## Create an archive using libzip

To create a zip archive using libzip the following steps are
performed:

1. Create a zip file
2. Add files to the archive
3. Close the archive

### 1. Create a zip file

```C++
#include <zip.h>

// ...

zip_t * archive = zip_open(filename.c_str(), ZIP_CREATE | ZIP_TRUNCATE, nullptr);
if (nullptr == archive)
{
    std::cerr << "error: failed to create archive" << std::endl;
    return EXIT_FAILURE;
}
```

To create a new zip file, `zip_open` is used. FLags are provided to
specify the open mode:

- `ZIP_CREATE`: create a new file if it does not exist
- `ZIP_TRUNCATE`: clear the file contents if it does exist

Note: To add or update files in an existing archive, just remove 
`ZIP_TRUNCATE`.

### 2. Add files to the archive

```C++
zip_source_t * file_source = zip_source_file(archive, e.local_filename.c_str(), 0, std::filesystem::file_size(e.local_filename));
auto const rc = zip_add(archive, e.archive_filename.c_str(), file_source);
if (rc == -1)
{
    std::cerr << "error: failed to open file " << e.local_filename << std::endl;
    exit_code = EXIT_FAILURE;
    zip_source_free(file_source);
}
```

To add new files to an archive, a `zip_source` is needed. There are diffent
types of `zip_source`s provided by libzip, including

- file source to add local files
- buffer source to add in-memory data
- function source to add custom data, e.g. dynamically generated data

There are other sources available. Please refer to libzip documentation
for further information.

Once a source is created, it is added to the archive using `zip_add`.
Note that, sources must be freed manually only if `zip_add` fails.
Otherwise the source is handeled by the archive.

Also note that some source, e.g. buffer source, require specific
resource handling. Since modifications are actually written when
the archive is closed, resources must be available until then. 

### 3. Close the archive

Finally, the archive is closed using `zip_close`. Note that
modifications are written, when this function is called.
Libzip even provides APIs to modify and rewind changes. Please
refer to libzip documentation for further information.
