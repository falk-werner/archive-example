// SPDX-License-Identifier: Unlicense

#include <zip.h>

#include <iostream>
#include <string>

namespace
{

bool is_dir(std::string const & filename)
{
    // Note: according to zip file format specification, forward slashes
    //       are used as path separators (see section 4.4.17 of https://pkware.cachefly.net/webdocs/casestudies/APPNOTE.TXT).
    //
    //       This implementation is inspired by Java's isDirectory funtion
    //       (see https://github.com/JetBrains/jdk8u_jdk/blob/master/src/share/classes/java/util/zip/ZipEntry.java).
    return (!filename.empty()) && ('/' == filename[filename.size() - 1]);
}

}

int main(int argc, char* argv[])
{
    bool show_help = (argc <= 1);
    int exit_code = EXIT_SUCCESS;

    if (argc > 1)
    {
        for(int i = 1; i < argc; i++)
        {
            std::string const filename(argv[i]);
            if ((filename == "-h") || (filename == "--help"))
            {
                show_help = true;
                break;
            }

            zip_t * archive = zip_open(filename.c_str(), ZIP_RDONLY, nullptr);
            if (nullptr == archive)
            {
                std::cerr << "failed to read archive " << filename << std::endl;
                exit_code = EXIT_FAILURE;
                continue;
            }

            zip_int64_t const count = zip_get_num_entries(archive, 0);
            zip_uint64_t const file_flags = ZIP_STAT_NAME | ZIP_STAT_SIZE;
            for(zip_int64_t j = 0; j < count; j++)
            {
                zip_stat_t info;
                int const rc = zip_stat_index(archive, j, 0, &info);
                if ((rc == 0) && (file_flags == (info.valid & file_flags)) && (!is_dir(info.name)))
                {
                    std::cout << info.size << '\t' << info.name << std::endl;
                }
            }

            zip_close(archive);
        }
    }

    if (show_help)
    {
        std::cout << R"(libzip_list_archive, (c) 2023 Falk Werner
List contents of a zip archive using libzip.

Usage:
    libzip_list_archive <filename>...

Arguments:
    filename - name of the zip file to list        
)";
    }

    return exit_code;
}