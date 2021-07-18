#include "../../src/system/Filer.hpp"
#include <filesystem>
#include <functional>


using namespace System;

void
test_CreateDelete(Filer *filer);
void
test_Permissions(Filer *filer);
void
test_FileListOperations(Filer *filer);

int
main()
{
    test_CreateDelete(System::FILER);
    test_Permissions(System::FILER);
    test_FileListOperations(System::FILER);
}


void
test_FileListOperations(Filer *filer)
{
    // Tests:
    // Filer.getFileList()
    // Filer.updateFileList()


    fs::path path = fs::path("./");
    filer->updateFileList(path);
    for (const auto &file : filer->getFileList())
        {
            std::cout << std::endl;

            std::cout << "Absolute Path:\t" << fs::absolute(file) << std::endl;;
            std::cout << "Relative Path:\t" << file.path() << std::endl;
            std::cout << "Filename: \t" << file.path().filename() << std::endl;
            std::cout << "Permissions:\t" << filer->permsToString(
                          file.status().permissions()) << std::endl;

            if (!fs::is_directory(file.path()))
                {
                    std::cout << "Size:\t" << fs::file_size(file.path()) << std::endl;
                }
        }

    filer->updateFileList("/");
    for (const auto &file : filer->getFileList())
        {
            std::cout << "Absolute Path:\t" << fs::absolute(file) << std::endl;;
        }


}

void
test_Permissions(Filer *filer)
{
    // Tests:
    // Filer.getPermissions() overloads
    // Filer.permstostring

    fs::path permTestFile = fs::path("permTestFile.txt");

    if (fs::exists(permTestFile))
        {
            filer->deleteFile(permTestFile);
            std::cout << "deleted that bitch" << std::endl;
        }
    else
        {
            filer->createFile(permTestFile);
            std::cout << "created that bitch" << std::endl;
        }

    File direntToTest =
        File(fs::path(permTestFile));

    fs::perms p1 = filer->getPermissions(direntToTest);
    fs::perms p2 = filer->getPermissions(fs::path(permTestFile));
    fs::perms p3 = filer->getPermissions(direntToTest.path());

    std::cout << filer->permsToString(p1) << std::endl;
    std::cout << filer->permsToString(p2) << std::endl;
    std::cout << filer->permsToString(p3) << std::endl;

    filer->setPermissions(fs::perms::all, direntToTest);

    std::cout << filer->permsToString(direntToTest.status().permissions()) <<
              std::endl;

}

void
test_CreateDelete(Filer *filer)
{
    // Tests:
    // Filer.createDir
    // Filer.createFile
    // Filer.deleteDir
    // Filer.deleteFile

    std::string hey;

    filer->createDir(fs::path("newdir"));
    std::cout << "Created new dir" << std::endl;
    //std::cin >> hey;

    filer->createFile(fs::path("newdir/newfile1.txt"));
    filer->createFile(fs::path("newdir/newfile2.txt"));
    std::cout << "Created 2 new files inside new dir" << std::endl;
    //std::cin >> hey;

    filer->deleteFile(fs::path("newdir/newfile1.txt"));
    std::cout << "Deleted file newdir/newfile1.txt" << std::endl;
    //std::cin >> hey;

    filer->createDir(fs::path("newdir/newdir2"));
    std::cout << "Created newdir2" << std::endl;
    //std::cin >> hey;

    filer->deleteDir(fs::path("newdir"));
    std::cout << "Deleted main dir recursively" << std::endl;
}
