#ifndef FILER_HPP
#define FILER_HPP

#include "System.hpp"
#include <filesystem>

namespace System
{

class Filer
{
public:
    // TODO: Which should be references (e.g. fs::path &path)
    Filer();
    bool        createDir(const fs::path &path);
    bool        createFile(const fs::path &path);

    bool        deleteDir(const fs::path &path);
    bool        deleteFile(const fs::path &path);

    fs::perms   getPermissions(const fs::path &path);
    fs::perms   getPermissions(const File &dirent);
    bool        setPermissions(const fs::perms &newPerms, const fs::path &path);

    std::string getCwd();
    std::string setCwd(const std::string &path);

    bool        updateFileList(const fs::path &path);
    FileList    getFileList();

    std::string typeToString(const fs::file_status &fileStatus);
    std::string permsToString(const fs::perms &perms);

private:
    std::string cwd;
    FileList    fileList;
    bool        pathIsValid(const fs::path &path);

};
}

#endif
