// SPDX-License-Identifier: Unlicense

#include <archive.h>
#include <archive_entry.h>

#include <iostream>
#include <string>

int main(int argc, char * argv[])
{
    bool show_help = (argc <= 1);
    int exit_code = EXIT_SUCCESS;

    if (argc > 1)
    {
        for(int i = 1; i < argc; i++)
        {
            std::string const filename = argv[i];
            if ((filename == "-h") || (filename == "--help"))
            {
                show_help = true;
                break;
            }

            archive * a = archive_read_new();
            archive_read_support_filter_all(a);
            archive_read_support_format_all(a);

            int rc = archive_read_open_filename(a, filename.c_str(), 10240);
            if (rc != ARCHIVE_OK)
            {
                std::cerr << "failed to open file" << std::endl;
                exit_code = EXIT_FAILURE;
                continue;
            }

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
            archive_read_free(a);
        }
    }

    if (show_help)
    {
        std::cout << R"(libarchive_list_archive, (c) 2023 Falk Werner
list contents of an archive.

Usage:
    listarchive_list_archive <filename>...

Arguments:
    filename - name of the archive to list
)";
    }

    return exit_code;
}