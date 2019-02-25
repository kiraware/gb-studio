#include <fstream>
#include <cassert>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

void touch(const std::string& fileName)
{
    std::ofstream f(fileName, std::ios_base::out);
    assert(f.good());
}

void doTest(const std::string& uniqueDirName, const std::string& fileName)
{
    printf("test in %s: %s\n", uniqueDirName.c_str(), fileName.c_str());

    auto ret = mkdir(uniqueDirName.c_str(), 0777);
    assert(ret == 0);

    touch(uniqueDirName + "/" + fileName);

    auto fd = open(uniqueDirName.c_str(), O_RDONLY | O_DIRECTORY);
    assert(fd > 0);

    for (;;)
    {
        dirent d;
        auto nread = getdents(fd, &d, sizeof(d));
        assert(nread != -1);
        if (nread == 0)
        {
            break;
        }

        if (strcmp(d.d_name, ".") == 0 || strcmp(d.d_name, "..") == 0)
        {
            continue;
        }

        printf("d.d_name = %s\n", d.d_name);
    }
}

int main()
{
    // Non-ascii file name.
    doTest("test_dir", u8"абвгд");

    // File name exceeds the limit of 255 chars and is truncated.
    doTest("test_dir2", std::string(300, '1'));
}
