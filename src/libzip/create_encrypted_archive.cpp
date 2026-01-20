// SPDX-License-Identifier: Unlicense

#include <zip.h>

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace
{

void print_usage()
{
    std::cout << R"(libzip_create_encrypted_archive, (c) 2026 Falk Werner
Creates an encrypted archive

Usage:
    libzip_create_encrypted_archive <archive> <password> <filename>...

Arguments:
  archive  - filename of the newly created archive
  password - password used for encryption
  filename - name of the file to add

Example:
  libzip_create_encrypted_archive test.zip. secret README.md
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
        show_help = (argc <= 3);
        if (argc > 3)
        {
            archive_filename = argv[1];
            password = argv[2];

            for(int i = 3; i < argc; i++)
            {
                std::string const arg = argv[i];
                if ((arg == "-h") || (arg == "--help"))
                {
                    show_help = true;
                    break;
                }

                entry e = {arg, arg};
                auto const pos = arg.find(':');
                if (pos != std::string::npos)
                {
                    e = { arg.substr(0, pos), arg.substr(pos +1) };
                }
                entries.push_back(e);
            }
        }
    }

    bool show_help;
    std::string archive_filename;
    std::string password;
    std::vector<entry> entries;
};

int create_encrypted_archive(
    std::string const & filename,
    std::string const & password,
    std::vector<entry> const & entries)
{
    int exit_code = EXIT_SUCCESS;

    auto * const archive = zip_open(filename.c_str(), ZIP_CREATE | ZIP_TRUNCATE, nullptr);
    if (archive == nullptr)
    {
        std::cerr << "error: failed to create archive" << std::endl;
        return EXIT_FAILURE;
    }

    for(auto const & e: entries)
    {
        if (!std::filesystem::is_regular_file(e.local_filename))
        {
            continue;
        }

        auto * source = zip_source_file(archive, e.local_filename.c_str(), 0,
            std::filesystem::file_size(e.local_filename));
        auto const i = zip_add(archive, e.archive_filename.c_str(), source);
        if (i == -1)
        {
            std::cerr << "error: failed to add file: " << e.local_filename << std::endl;
            exit_code = EXIT_FAILURE;
            zip_source_free(source);
        }

        zip_file_set_encryption(archive, i, ZIP_EM_AES_256, password.c_str());
        std::cout << e.local_filename << std::endl;
    }

    zip_close(archive);
    return exit_code;
}

}


int main(int argc, char* argv[])
{
    context ctx(argc, argv);
    int exit_code = EXIT_SUCCESS;

    if (!ctx.show_help)
    {
        exit_code = create_encrypted_archive(ctx.archive_filename, ctx.password, ctx.entries);
    }
    else
    {
        print_usage();
    }

    return exit_code;
}