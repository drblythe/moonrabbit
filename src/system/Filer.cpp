#include "Filer.hpp"
#include <filesystem>

namespace System
{

Filer::Filer():
    showDotFiles(false)
{
}


/* Creation */
bool
Filer::createDir(const fs::path &path)
{
    File dirent = File(path);
    if (dirent.exists())
        {
            return false;
        }
    // TODO: Check permissions of parent dir

    return fs::create_directory(path);
}

bool
Filer::createFile(const fs::path &path)
{
    std::ofstream ofs(path);
    ofs.close();
    return 0;
}

/* Deletion */
bool
Filer::deleteDir(const fs::path &path)
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
Filer::deleteFile(const fs::path &path)
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
Filer::getPermissions(const fs::path &path)
{
    return fs::status(path).permissions();
}

fs::perms
Filer::getPermissions(const File &dirent)
{
    return fs::status(dirent.path()).permissions();
}

bool
Filer::setPermissions(const fs::perms &newPerms, const fs::path &path)
{
    /*
    https://stackoverflow.com/questions/46835440/set-permission-for-all-files-in-a-folder-in-c
    https://stackoverflow.com/questions/46835440/set-permission-for-all-files-in-a-folder-in-c

    //File(path).status().permissions(newPerms); // WRONG

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
Filer::permsToString(const fs::perms &perms)
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

std::string
Filer::typeToString(const fs::file_status &fileStatus)
{
    std::string typeStr;
    switch (fileStatus.type())
        {
        case fs::file_type::none: typeStr = "none"; break;
        case fs::file_type::not_found: typeStr = "not_found"; break;
        case fs::file_type::regular: typeStr = "regular"; break;
        case fs::file_type::directory: typeStr = "directory"; break;
        case fs::file_type::symlink: typeStr = "symlink"; break;
        case fs::file_type::block: typeStr = "block"; break;
        case fs::file_type::character: typeStr = "character"; break;
        case fs::file_type::fifo: typeStr = "fifo"; break;
        case fs::file_type::socket: typeStr = "socket"; break;
        case fs::file_type::unknown: typeStr = "unknown"; break;
        default: typeStr = "?"; break;
        }
    return typeStr;
}


// File List
bool
Filer::updateFileList(const fs::path &path)
{
    this->fileList.clear();
    for (const File &entry : fs::directory_iterator(path))
        {
            fileList.push_back(fs::directory_entry(entry));
        }

    return true;
}

FileList
Filer::getFileList()
{
    return this->fileList;
}

bool
Filer::pathIsValid(const fs::path &path)
{
    return fs::exists(path);
}


bool
Filer::setCwd(const fs::path &path)
{
    if (!pathIsValid(path))
        {
            return false;
        }
    return updateFileList(path);
}

int
Filer::getNumFilesInDir()
{
	return this->fileList.size();
}

} // namespace System
