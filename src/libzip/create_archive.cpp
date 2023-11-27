// SPDX-License-Identifier: Unlicense

#include <zip.h>

#include <cstdio>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace
{

void print_usage()
{
    std::cout << R"(libzip_create_archive, (c) 2023 Falk Werner
Creates a zip archive

Usage:
    libzip_create_archive <archive> <filename>...

Arguments:
    archive  - filename of the newly created archive
    filename - name of the file to add

Example:
    libzip_create_archive test.zip README.md
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

    zip_t * archive = zip_open(filename.c_str(), ZIP_CREATE | ZIP_TRUNCATE, nullptr);
    if (nullptr == archive)
    {
        std::cerr << "error: failed to create archive" << std::endl;
        return EXIT_FAILURE;
    }

    for (auto const & e: entries)
    {
        if (std::filesystem::is_regular_file(e.local_filename))
        {
            zip_source_t * file_source = zip_source_file(archive, e.local_filename.c_str(), 0, std::filesystem::file_size(e.local_filename));
            auto const rc = zip_add(archive, e.archive_filename.c_str(), file_source);
            if (rc == -1)
            {
                std::cerr << "error: failed to open file " << e.local_filename << std::endl;
                exit_code = EXIT_FAILURE;
                zip_source_free(file_source);
            }
        }
    }

    zip_close(archive);
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