// SPDX-License-Identifier: Unlicense

#include <zip.h>

#include <cstdio>
#include <iostream>
#include <string>

namespace
{

int extract_file(
    std::string const & archive_filename,
    std::string const & filename)
{
    int exit_code = EXIT_SUCCESS;

    zip_t * archive = zip_open(archive_filename.c_str(), ZIP_RDONLY, nullptr);
    if (nullptr == archive)
    {
        std::cerr << "failed to open archive" << std::endl;
        return EXIT_FAILURE;
    }

    auto const id = zip_name_locate(archive, filename.c_str(), 0);
    if (0 <= id)
    {
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
    }
    else
    {
        std::cerr << "error: file not found in archive" << std::endl;
        exit_code = EXIT_FAILURE;
    }

    zip_close(archive);

    return exit_code;
}

}

int main(int argc, char* argv[])
{
    bool show_help = (argc != 3);
    int exit_code = EXIT_SUCCESS;

    if (argc == 3)
    {
        std::string const archive_filename = argv[1];
        std::string const filename = argv[2];

        if ((archive_filename != "-h") && (archive_filename != "--help"))
        {
            exit_code = extract_file(archive_filename, filename);
        }
        else
        {
            show_help = true;
        }
    }

    if (show_help)
    {
        std::cout << R"(libarchive_extract_file, (c) 2023 Falk Werner
Extract file from archive

Usage:
    libarchive_extract_file <archive> <filename>

Arguments:
    archive  - filename of an existing zip archvie
    filename - name of the file to extract 
)";
    }
    

    return exit_code;
}
