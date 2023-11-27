// SPDX-License-Identifier: Unlicense

#include <archive.h>
#include <archive_entry.h>

#include <cstdio>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace
{

void print_usage()
{
    std::cout << R"(libarchive_create_archive, (c) 2023 Falk Werner
Creates a zip archive

Usage:
    libarchive_create_archive <archive> <filename>...

Arguments:
    archive  - filename of the newly created archive
    filename - name of the file to add

Example:
    libarchive_create_archive test.zip README.md
)";
}

struct entry
{
    std::string local_filename;
    std::string archive_filename;
};

struct context
{
    context(int argc, char* argv[])
    {
        show_help = (argc <= 2);
        if (argc > 2)
        {
            archive_filename = argv[1];

            for(int i = 2; i < argc; i++)
            {
                std::string const arg = argv[i];
                if ((arg == "-h") || (arg == "--help"))
                {
                    show_help = true;
                    break;
                }

                entry e = { arg, arg};
                auto const pos = arg.find(':');
                if (pos == std::string::npos)
                {
                    e  = { arg.substr(0, pos), arg.substr(pos + 1) };
                }

                entries.push_back(e);
            }
        }

    }

    bool show_help;
    std::string archive_filename;
    std::vector<entry> entries;
};

int create_archive(
    std::string const & filename,
    std::vector<entry> const & entries)
{
    int exit_code = EXIT_SUCCESS;

    archive * a = archive_write_new();
    archive_write_set_format_zip(a);

    int rc = archive_write_open_filename(a, filename.c_str());
    if (rc != ARCHIVE_OK)
    {
        std::cerr << "error: failed to create archive" << std::endl;
        archive_write_free(a);
        return EXIT_FAILURE;
    }

    for (auto const & e: entries)
    {
        if (std::filesystem::is_regular_file(e.local_filename))
        {
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
            else
            {
                std::cerr << "error: failed to open file " << e.local_filename << std::endl;
                exit_code = EXIT_FAILURE;
            }

            archive_entry_free(ae);
        }
    }

    archive_write_close(a);
    archive_write_free(a);
    return exit_code;
}

}


int main(int argc, char * argv[])
{
    context ctx(argc, argv);
    int exit_code = EXIT_SUCCESS;

    if (!ctx.show_help)
    {
        exit_code = create_archive(ctx.archive_filename, ctx.entries);
    }
    else
    {
        print_usage();
    }

    return exit_code;   
}