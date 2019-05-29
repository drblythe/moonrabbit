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
        return 1;
    }

  out_error:
    saved_errno = errno;

    close(fd_src);
    if (fd_dest >= 0)
        close(fd_dest);

    errno = saved_errno;
    return -1;	
}

int handle_cmd(char **p_input, char** p_cwd)
{
	char ** arg_vec;
	arg_vec = tokenize_command(*p_input);

	if (!arg_vec) {
		return -1;
	}

	// command is arg_vec[0]
	// rest is arguments
	

	if (!strcmp(arg_vec[0], "cd")) {
		strcpy(*p_cwd,arg_vec[1]);
	}
	else if (!strcmp(arg_vec[0],"cp")) {
		/* Have a "confirm" function called from header with access to ncurses */
		/*
		printf("\nCopy %s to %s?",arg_vec[1],arg_vec[2]); 
		c = getchar();
		*/
		cmd_copy(arg_vec[2],arg_vec[1]);
	}
	else if (!strcmp(arg_vec[0],"touch")) {
		int fd;
		fd = creat (arg_vec[1],0644);
		if (fd == -1) {
    		printf("error\n");
    		return -1;
		}

	}
	else if (!strcmp(arg_vec[0],"mkdir")) {
		mkdir(arg_vec[1], S_IRWXU | S_IRWXG | S_IRWXO);
	}

	
	free(arg_vec);
	return 1;
}

char** tokenize_command(char* command) 
{
	/* strtok_r() */
	/* strtok() */
	
	char a_delim = ' ';
	char** result    = 0;
    size_t count     = 0;
    char* tmp        = command;
    char* last_space = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_space = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_space < (command + strlen(command) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
	count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(command, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}




