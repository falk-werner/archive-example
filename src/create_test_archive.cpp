#include <iostream>
#include <zip.h>

namespace
{

void add_textfile(zip_t * archive, std::string const & filename, std::string const & contents)
{
    zip_source_t * source = zip_source_buffer(archive, contents.c_str(), contents.size(), 0);
    auto const rc = zip_file_add(archive, filename.c_str(), source, ZIP_FL_OVERWRITE | ZIP_FL_ENC_UTF_8);
    if (rc == -1)
    {
        std::cerr << "error: failed to add file " << filename << std::endl;
        zip_source_free(source);
    }
}

}

int main(int argc, char * argv[])
{
    zip_t * archive = zip_open("test.zip", ZIP_CREATE | ZIP_TRUNCATE, nullptr);
    if (nullptr == archive)
    {
        std::cerr << "error: failed to create zip archive" << std::endl;
        return EXIT_FAILURE;
    }

    std::string const readme = R"(#README

This is an exmaple)";
    add_textfile(archive, "README.md", readme);

    zip_add_dir(archive, "subdir");
    add_textfile(archive, "subdir\\some_file.txt", "some contents");

    zip_close(archive);
    return EXIT_SUCCESS;
}