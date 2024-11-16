// SPDX-License-Identifier: Unlicense

#include <zip.h>

#include <cstdio>
#include <iostream>
#include <string>
#include <memory>

namespace
{

void close_archive(zip_t * archive)
{
    zip_close(archive);
}

void close_zip_file(zip_file_t * file)
{
    zip_fclose(file);
}

using zip_ptr = std::unique_ptr<zip_t, void(*)(zip_t*)>;
using zip_file_ptr = std::unique_ptr<zip_file_t, void(*)(zip_file_t*)>;


int extract_file(
    std::string const & archive_filename,
    std::string const & filename)
{
    int exit_code = EXIT_SUCCESS;

    zip_t * raw_archive = zip_open(archive_filename.c_str(), ZIP_RDONLY, nullptr);
    if (nullptr == raw_archive)
    {
        std::cerr << "failed to open archive" << std::endl;
        return EXIT_FAILURE;
    }
    zip_ptr archive(raw_archive, close_archive);

    auto * const raw_source = zip_fopen(archive.get(), filename.c_str(), 0);
    if (nullptr == raw_source)
    {
        std::cerr << "error: failed to open archived file" << std::endl;
        return EXIT_FAILURE;
    }
    zip_file_ptr source(raw_source, close_zip_file);

    char buffer[10240];
    auto length = zip_fread(source.get(), buffer, 10240);
    while (0 < length)
    {
        fwrite(buffer, 1, length, stdout);
        length = zip_fread(source.get(), buffer, 10240);
    }

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
