# Add files to an archive

- Source: [libzip/add_files.cpp](../src/libzip/add_files.cpp)

## Prerequisites

In order to add files to a zip archive, an archive us needed.

A sample archive names `test.zip` can be created using `create_test_archive`:

```bash
cmake -B build
cmake --build build
./build/create_test_archive
```

Alternativly, a test archive can also be created using the `zip` command line tool:

```bash
zip -r test.zip README.md
```

## Add files using zip command line tool

```bash
zip test.zip CMakeLists.txt
```

## Notes about libarchive

Unfortunately, libarchive does not support to modify an archive natively.
In order to modify an archive using libarchive, it has to be re-created.

## Create an archive using libzip

To add a file to a zip archive using libzip the following steps are
performed:

1. Open a zip file
2. Add files to the archive
3. Close the archive

### 1. Open a zip file

```C++
#include <zip.h>

// ...

zip_t * archive = zip_open(filename.c_str(), ZIP_CREATE, nullptr);
if (nullptr == archive)
{
    std::cerr << "error: failed to create archive" << std::endl;
    return EXIT_FAILURE;
}
```

To create a new zip file, `zip_open` is used. The flags `ZIP_CREATE` is provided to
create a new archive if it does not exist.

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
