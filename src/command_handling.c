#include "command_handling.h"

int cmd_cd(char** p_cwd, char* dir_name)
{
	strcpy(*p_cwd,dir_name);
    return 1;
}

int cmd_mkdir(char* path)
{
	mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
	return 1;
}

int cmd_mkdir_splitpath(char* path, char* file_name)
{
	char full_path[strlen(path)+1+strlen(file_name)+1];
	strcpy(full_path,path);
	strcat(full_path, "/");
	strcat(full_path, file_name);
	mkdir(full_path, S_IRWXU | S_IRWXG | S_IRWXO);
	return 1;
}

int cmd_copy_dir(int del_after_move, char* dest_dir, char* src_dir)
{
	struct dirent **dir_contents;
	int dir_size;

	dir_size = scandir(src_dir, &dir_contents, NULL, alphasort);
	cmd_mkdir(dest_dir);

	for (int i = 0; i < dir_size; i++) {
		if (!strcmp(dir_contents[i]->d_name, ".") || !strcmp(dir_contents[i]->d_name, "..")) {
			continue;
		}
		char sub_dest_path[strlen(dest_dir)+1+strlen(dir_contents[i]->d_name)+1];
		strcpy(sub_dest_path, dest_dir);
		strcat(sub_dest_path, "/");
		strcat(sub_dest_path, dir_contents[i]->d_name);

		char sub_src_path[strlen(src_dir)+1+strlen(dir_contents[i]->d_name)+1];
		strcpy(sub_src_path, src_dir);
		strcat(sub_src_path, "/");
		strcat(sub_src_path, dir_contents[i]->d_name);

		if (is_directory(src_dir,dir_contents[i]->d_name)) {
			cmd_copy_dir(del_after_move, sub_dest_path, sub_src_path);
			if (del_after_move)
				cmd_delete(sub_src_path);
		}
		else {
			cmd_copy(sub_dest_path, sub_src_path);
			if (del_after_move)
				cmd_delete(sub_src_path);
		}
		free(dir_contents[i]);
	}
	if (del_after_move)
		cmd_delete(src_dir);
	free(dir_contents);
	return 1;
}

int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
        perror(fpath);

    return rv;
}

int rmrf(const char *path)
{
	if(!strcmp(path,"/")) {
		return -1;
	}
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

int cmd_delete_dir(const char* dir_path)
{
	rmrf(dir_path);
	return 1;
}

int cmd_delete(const char * file_path)
{
	int ret;
	ret = remove(file_path);
	if (ret) return 0;
	else return 1;
}

int cmd_move(const char *dest, const char *src)
{
	if (!cmd_copy(dest,src))
		return 0;
	else {
		cmd_delete(src);
		return 1;
	}
}

int cmd_copy(const char *dest, const char *src)
{
    int fd_dest, fd_src;
    char buf[4096];
    ssize_t nread;
    int saved_errno;

    fd_src = open(src, O_RDONLY);
    if (fd_src < 0)
        return -1;

    fd_dest = open(dest, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (fd_src < 0)
        goto out_error;

    while (nread = read(fd_src, buf, sizeof buf), nread > 0)
    {
        char *out_ptr = buf;
        ssize_t nwritten;

        do {
            nwritten = write(fd_dest, out_ptr, nread);

            if (nwritten >= 0)
            {
                nread -= nwritten;
                out_ptr += nwritten;
            }
            else if (errno != EINTR)
            {
                goto out_error;
            }
        } while (nread > 0);
    }

    if (nread == 0)
    {
        if (close(fd_dest) < 0)
        {
            fd_dest = -1;
            goto out_error;
        }
        close(fd_src);

        /* Success! */
        return 1;
    }

  out_error:
    saved_errno = errno;

    close(fd_src);
    if (fd_dest >= 0)
        close(fd_dest);

    errno = saved_errno;
    return 0;	
}

int handle_cmd(char **p_input, char** p_cwd)
{
	char ** arg_vec;
	arg_vec = tokenize_command(*p_input);

	if (!arg_vec) {
		return -1;
	}

	if (!strcmp(arg_vec[0], "cd")) {
		strcpy(*p_cwd,arg_vec[1]);
	}
	else if (!strcmp(arg_vec[0],"cp")) {
		cmd_copy(arg_vec[2],arg_vec[1]);
	}
	else if (!strcmp(arg_vec[0], "mv")) {
		cmd_move(arg_vec[2],arg_vec[1]);
	}
	else if (!strcmp(arg_vec[0],"touch")) {
		int fd;
		fd = creat (arg_vec[1],0644);
		if (fd == -1) {
    		printf("error\n");
    		return -1;
		}

	}
	else if (!strcmp(arg_vec[0], "mkdir")) {
		mkdir(arg_vec[1], S_IRWXU | S_IRWXG | S_IRWXO);
	}
	else if (!strcmp(arg_vec[0], "rm")) {
		cmd_delete(arg_vec[1]);
	}
	
	free(arg_vec);
	return 1;
}