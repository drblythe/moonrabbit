#include "../src/system/Filer.hpp"
#include <filesystem>
#include <functional>


using namespace System;

void
test_CreateDelete(Filer *filer);
void
test_Permissions(Filer *filer);

int
main()
{
    Filer *filer = new Filer();

    test_CreateDelete(filer);
    test_Permissions(filer);
}


void
test_Permissions(Filer *filer)
{
	// Tests:
	// Filer.getPermissions() overloads
	// Filer.permstostring
	
    fs::directory_entry direntToTest =
        fs::directory_entry(fs::path("permTestFile.txt"));

	fs::perms p1 = filer->getPermissions(direntToTest);
	fs::perms p2 = filer->getPermissions(fs::path("permTestFile.txt"));
	fs::perms p3 = filer->getPermissions(direntToTest.path());

	std::cout << filer->permsToString(p1) << std::endl;
	std::cout << filer->permsToString(p2) << std::endl;
	std::cout << filer->permsToString(p3) << std::endl;

	filer->setPermissions(fs::perms::all, direntToTest);

	std::cout << filer->permsToString(direntToTest.status().permissions()) << std::endl;


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
