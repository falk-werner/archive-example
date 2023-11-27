// SPDX-License-Identifier: Unlicense

#include <archive.h>
#include <archive_entry.h>

#include <iostream>
#include <string>

namespace
{

int extract_file(
    std::string const & archive_filename,
    std::string const & filename)
{
    int exit_code = EXIT_SUCCESS;

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

    archive_free(a);

    if (!found)
    {
        std::cerr << "error: file not found in archive" << std::endl;
        exit_code = EXIT_FAILURE;
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
