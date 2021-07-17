#ifndef FILER_HPP
#define FILER_HPP

#include "System.hpp"

namespace System
{



class Filer
{
public:
    // TODO: Which should be references (e.g. fs::path &path)
    Filer();
    bool         createDir(fs::path path);
    bool         createFile(fs::path path);

    bool         deleteDir(fs::path path);
    bool         deleteFile(fs::path path);

    fs::perms    getPermissions(fs::path path);
    fs::perms    getPermissions(DirEnt dirent);
    bool         setPermissions(fs::perms newPerms, fs::path path);
    std::string  permsToString(fs::perms perms);

    std::string getCwd();
    std::string setCwd(std::string path);

    FileList    getFileList(fs::path path);

private:
    std::string cwd;
    FileList      fileList;

    bool        pathIsValid(fs::path path);
    bool        updateFileList(fs::path path);
	bool 		getFilesInDir(fs::path path);

};
}

#endif
