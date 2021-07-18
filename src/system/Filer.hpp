#ifndef FILER_HPP
#define FILER_HPP

#include "System.hpp"
#include <filesystem>

namespace System
{

class Filer
{
public:
    Filer();

    bool        updateFileList(const fs::path &path);
    FileList    getFileList();
	int 		getNumFilesInDir();
	bool 		showDotFiles;

    std::string getCwd();
    bool 		setCwd(const fs::path &path);

    bool        createFile(const fs::path &path);
    bool        deleteFile(const fs::path &path);
    bool        createDir(const fs::path &path);
    bool        deleteDir(const fs::path &path);

    fs::perms   getPermissions(const fs::path &path);
    fs::perms   getPermissions(const File &dirent);
    bool        setPermissions(const fs::perms &newPerms, const fs::path &path);

    std::string typeToString(const fs::file_status &fileStatus);
    std::string permsToString(const fs::perms &perms);

private:
	fs::path 	cwd;
    FileList    fileList;
    bool        pathIsValid(const fs::path &path);

};

static Filer *FILER = new Filer();

}

#endif
