#include "../include/trashctl.h"
/* This covers the case of moving files to your trash directory */

static int init_shell(struct environment_info* env, char* shell_command)
{
    int err, status, fd;
    errno = 0;
    fd = open(TRASHCTL_DEV_NULL_PATH, O_WRONLY);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    pid_t pid_sh = fork();

    if(pid_sh < 0){
        return 1;
    } else if(pid_sh == 0){
        /* child process */
        if((err = execl(TRASHCTL_SH_PATH, "sh", "-c", shell_command, NULL)) == -1){
            return 1;
        }
    } else {
        if((err = waitpid(pid_sh, &status, 0)) == -1){
            return 1;
        }
    }

    close(fd);

    return 0;
}

static int put_file(struct environment_info* env, char* file_name)
{
    int err, cwd_length;
    errno = 0;

    char cwd[1024] = {};
    getcwd(cwd, sizeof(cwd)); // grab the current working directory of the user
    cwd_length = strlen(cwd);

    char put_file_command[256] = "mv ";
    char put_file_trash_dir[128] = {};
    strcpy(put_file_trash_dir, env->trash_dir);

    char *cmd_ptr = put_file_command;

    /* this should be changed, too much repetition and complexity */
    /*  update, changed it, but still dont really like the solution. */
    /*  it feels not as readable as I would like, nor is it as maintainable as I would like */
    cwd[cwd_length] = '/';
    strcat(cwd, file_name);
    cwd_length = strlen(cwd);
    cwd[cwd_length] = ' ';

    strcat(cwd, put_file_trash_dir);
    strcat(put_file_command, cwd);

    if((err = init_shell(env, cmd_ptr)) == 1){
        fprintf(stderr, "[ERROR] Fork failed for put command.\n");
        return 1;
    }

    return 0;
}

int trashctl_put(struct environment_info* env, char* file_name)
{
    return put_file(env, file_name);
}
