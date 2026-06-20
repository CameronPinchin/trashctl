#include "../include/trashctl.h"
/* This file covers the trashctl delete option. Logic currently housed within the opt_empty.c file, but is to be moved here. */

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
        if((err = execl(TRASHCTL_SH_PATH, "sh", "-c", shell_command, "> /dev/null 2>&1",NULL)) == -1){
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

static int delete_file(struct environment_info* env, char* file_name)
{
    int err;
    errno = 0;

    char delete_file_command[128] = "rm ";
    char usr_target_file_path[64] = {};

    char *cmd_ptr = delete_file_command;

    strcpy(usr_target_file_path, env->trash_dir);
    strcat(usr_target_file_path, file_name);
    strcat(delete_file_command, usr_target_file_path);

    if((err = init_shell(env, cmd_ptr) == 1)){
        fprintf(stderr, "[ERROR] Failed to open shell for file deletion.\n");
        return 1;
    }

    return 0;
}

int trashctl_delete(struct environment_info* env, char* file_name)
{
    return delete_file(env, file_name);
}
