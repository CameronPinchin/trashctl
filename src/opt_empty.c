#include "../include/trashctl.h"
/* This will cover the empty command line option for trashctl */

/* fork and exec /bin/sh to then just run rm commands */
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

        return 0;
    }

    return 0;
}

static int empty_all_files(struct environment_info* env)
{
    int err;
    errno = 0;

    char empty_all_files_command[128] = "rm ";
    char usr_target_file_path[64] = {};

    char *cmd_ptr = empty_all_files_command;

    strcpy(usr_target_file_path, env->trash_dir);
    strcat(usr_target_file_path, "*");
    strcat(empty_all_files_command, usr_target_file_path);

    if((err = init_shell(env, cmd_ptr) == 1)){
        fprintf(stderr, "[ERROR] Fork failure\n");
        return 1;
    }

    return 0;
}

static int empty_one_file(struct environment_info* env, char* file_name)
{
    int err;
    errno = 0;

    char empty_one_file_command[128] = "rm ";
    char usr_target_file_path[64] = {};

    char *cmd_ptr = empty_one_file_command;

    strcpy(usr_target_file_path, env->trash_dir);
    strcat(usr_target_file_path, file_name);
    strcat(empty_one_file_command, usr_target_file_path);

    if((err = init_shell(env, cmd_ptr) == 1)){
        fprintf(stderr, "[ERROR] Failed to open shell for file deletion.\n");
        return 1;
    }

    return 0;
}

int trashctl_empty(struct environment_info* env)
{
    int err;

    empty_all_files(env);
    //empty_one_file(env, "test03");

    return 0;
}
