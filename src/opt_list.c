#include "../include/trashctl.h"
/* This will cover the list command line option for trashctl */

static int init_shell(struct environment_info* env, char* shell_command)
{
    int err, status;
    errno = 0;

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

    return 0;
}

static int trashctl_list_operation(struct environment_info *env)
{
    int err;
    errno = 0;

    char list_files_command[128] = "ls -l ";
    char *cmd_ptr = list_files_command;

    strcat(list_files_command, env->trash_dir);

    if((err = init_shell(env, cmd_ptr) == 1)){
        fprintf(stderr, "[ERROR] Fork failed for list operation.\n");
        return 1;
    }

    return 0;
}

int trashctl_list(struct environment_info* env)
{
    /* check for */
    int err;
    errno = 0;

    if((err = trashctl_list_operation(env)) == 1){
        fputs("[ERROR]: trashctl was unable to list files from your trash directory.\n", stderr);
        return 1;
    }

    return 0;
}
