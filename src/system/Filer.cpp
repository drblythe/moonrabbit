#include "Filer.hpp"
#include <filesystem>

namespace fs = std::filesystem;
namespace System
{

Filer::Filer()
{

}


/* Creation */
bool
Filer::createDir(fs::path path)
{
    DirEnt dirent = DirEnt(path);
    if (dirent.exists())
        {
            return false;
        }
    // TODO: Check permissions of parent dir

    return fs::create_directory(path);
}

bool
Filer::createFile(fs::path path)
{
    std::ofstream ofs(path);
    ofs.close();
    return 0;
}

/* Deletion */
bool
Filer::deleteDir(fs::path path)
{
    if (!fs::exists(path))
        {
            return false;
        }

    // TODO:
    // Forr all these functions:
    // Call the overloaded remove/create... where an error code is recorded
    // remove_all(path, &ec) for example
    fs::remove_all(path);
    return true;
}

bool
Filer::deleteFile(fs::path path)
{
    if (!fs::exists(path))
        {
            return false;
        }
    fs::remove(path);
    return true;
}


bool
find_file(const fs::path &dir_path,          // in this directory,
          const std::string &file_name,      // search for this name,
          fs::path &path_found )             // placing path here if found
{
    if ( !exists( dir_path ) )
        {
            return false;
        }
    fs::directory_iterator end_itr; // default construction yields past-the-end
    for (fs::directory_iterator itr(dir_path); itr != end_itr; ++itr )
        {
            if ( is_directory(itr->status()) )
                {
                    if ( find_file( itr->path(), file_name, path_found ) )
                        {
                            return true;
                        }
                }
            else if ( itr->path().filename() == file_name )
                {
                    path_found = itr->path();
                    return true;
                }
        }
    return false;
}

/* Permissions */
fs::perms
Filer::getPermissions(fs::path path)
{
    return fs::status(path).permissions();
}

fs::perms
Filer::getPermissions(DirEnt dirent)
{
    return fs::status(dirent.path()).permissions();
}

bool
Filer::setPermissions(fs::perms newPerms, fs::path path)
{
    /*
    https://stackoverflow.com/questions/46835440/set-permission-for-all-files-in-a-folder-in-c
    https://stackoverflow.com/questions/46835440/set-permission-for-all-files-in-a-folder-in-c

    //DirEnt(path).status().permissions(newPerms); // WRONG

     TODO:
     In all these functions
     Check if user has permissions to do these actions

    */

    if (!fs::exists(path))
        {
            return false;
        }

    fs::permissions(path, newPerms);

    return true;
}

std::string
Filer::permsToString(fs::perms perms)
{
    std::string permString;
    permString.append(((perms & fs::perms::owner_read) != fs::perms::none ? "r" :
                       "-")  );
    permString.append(((perms & fs::perms::owner_write) != fs::perms::none ? "w" :
                       "-") );
    permString.append(((perms & fs::perms::owner_exec) != fs::perms::none ? "x" :
                       "-")  );
    permString.append(((perms & fs::perms::group_read) != fs::perms::none ? "r" :
                       "-")  );
    permString.append(((perms & fs::perms::group_write) != fs::perms::none ? "w" :
                       "-") );
    permString.append(((perms & fs::perms::group_exec) != fs::perms::none ? "x" :
                       "-")  );
    permString.append(((perms & fs::perms::others_read) != fs::perms::none ? "r" :
                       "-") );
    permString.append(((perms & fs::perms::others_write) != fs::perms::none ? "w" :
                       "-"));
    permString.append(((perms & fs::perms::others_exec) != fs::perms::none ? "x" :
                       "-") );
    return permString;
}

// File List


bool
Filer::updateFileList(fs::path path)
{
	std::cout << path << std::endl;
    return true;
}

bool
Filer::getFilesInDir(fs::path path)
{
    for (const DirEnt &entry : fs::directory_iterator(path))
        {
            fileList.push_back(fs::directory_entry(entry));
            //const DirEnt newDirEnt = entry;
            //std::cout << entry.path() << std::endl;
        }

    return true;
}

FileList
Filer::getFileList(fs::path path)
{
	this->getFilesInDir(path);
    return this->fileList;
}

}
