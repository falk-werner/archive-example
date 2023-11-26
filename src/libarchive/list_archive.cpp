#include <iostream>
#include <archive.h>
#include <archive_entry.h>

int main(int argc, char * argv[])
{
    archive * a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    int rc = archive_read_open_filename(a, "test.zip", 10240);
    if (rc != ARCHIVE_OK)
    {
        std::cerr << "failed to open file" << std::endl;
        return 1;
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

    return 0;
}