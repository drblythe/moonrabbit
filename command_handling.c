#include "command_handling.h"

int cmd_cd(char** p_cwd, char* dir_name)
{
/*
    strcat(*p_cwd, "/");
    strcat(*p_cwd, dir_name);
*/
	strcpy(*p_cwd,dir_name);
	/* return 0 if the specified dir is not valid*/
    return 1;
}


int cmd_delete(const char * file_path)
{
/* 'Confirm deletion function' in another file */
	int ret;
	ret = remove(file_path);
	if (ret) return 0;
	else return 1;
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
        return 0;
    }

  out_error:
    saved_errno = errno;

    close(fd_src);
    if (fd_dest >= 0)
        close(fd_dest);

    errno = saved_errno;
    return -1;	
}

int handle_cmd(const char *input, char** p_cwd)
{

	/* separate command from argument*/
	int split_index = 0;
	for (int i = 0; i < strlen(input); i++) 
		if (input[i] == ' ')
			split_index = i;
	printf("split index=%d\n",split_index);
	
	/*make command variable */
	char cmd[split_index];
	//char cmd[3];
	for (int i = 0; i < split_index; i++)
		cmd[i] = input[i];
	cmd[split_index] = '\0';

	/* make arg var */
	int arg_len = strlen(input) - split_index;
	char arg[arg_len];
	for (int i = 0; i < arg_len; i++)
		arg[i] = input[split_index+1+i];

	/* appropriate function calls */
	if (!strcmp(cmd, "cd")) {
		strcpy(*p_cwd,arg);
	}
	else if (!strcmp(cmd,"cp")) {
		cmd_copy("/home/charlie/careful/test",arg);
	}
	else if (!strcmp(cmd,"touch")) {
		int fd;
		fd = creat (arg,0644);
		if (fd == -1) {
    		printf("error\n");
    		return -1;
		}

	}
	else if (!strcmp(cmd,"mkdir")) {
		mkdir(arg, S_IRWXU | S_IRWXG | S_IRWXO);
	}
		
	//printf("\n--%d--%s--%s--\n",split_index,cmd,arg);
	//char c = getchar();

	return 1;
}

int split_arguments(const char* argument) 
{
	return 0;
}





























