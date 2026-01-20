// SPDX-License-Identifier: Unlicense

#include <zip.h>

#include <cstdio>
#include <iostream>
#include <string>


int main(int argc, char* argv[])
{
    if (argc != 4)
    {
        std::cout << R"(libzip_extract_encrypted_file, (c) 2026 Falk Werner
Extract file from an encrypted archive

Usage:
    libzip_extrace_encrypted_file <archive> <password> <filename>

Arguments:
    archive  - filename of an existing encrypted zip archive
    password - password
    filename - name of the file to extract
)";
    }

    std::string const archive_filename = argv[1];
    std::string const password = argv[2];
    std::string const filename = argv[3];

    auto * const archive = zip_open(archive_filename.c_str(), ZIP_RDONLY, nullptr);
    if (archive == nullptr)
    {
        std::cerr << "error: failed to open archive" << std::endl;
        return EXIT_FAILURE;
    }

    auto const i = zip_name_locate(archive, filename.c_str(), 0);
    if (i == -1)
    {
        std::cerr << "error: failed to find file in archive" << std::endl;
        zip_close(archive);
        return EXIT_FAILURE;
    }

    auto * const file = zip_fopen_encrypted(archive, filename.c_str(), 0, password.c_str());
    if (file == nullptr)
    {
        std::cerr << "error: failed to open archived file" << std::endl;
        zip_close(archive);
        return EXIT_FAILURE;
    }

    char buffer[10240];
    auto length = zip_fread(file, buffer, 10240);
    while (0 < length)
    {
        fwrite(buffer, 1, length, stdout);
        length = zip_fread(file, buffer, 10240);
    }

    zip_fclose(file);
    zip_close(archive);
    return EXIT_SUCCESS;
}