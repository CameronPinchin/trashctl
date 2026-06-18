#include "../include/trashctl.h"
/* This will cover the empty command line option for trashctl */

/* The case where a user just calls trashctl empty, resulting in the entire trashbin being emptied */
/*  instead of using the rm binary, I could instead delegate this to a shell /bin/sh and then run the command through that*/
static int empty_all_files(struct environment_info* env)
{
    int err, status;
    errno = 0;

    pid_t pid_rm = fork();

    if(pid_rm < 0){
        fprintf(stderr, "[ERROR] %s\n", strerror(errno));
        return 1;

    } else if(pid_rm == 0) {

        /* we are the child process */
        char *t_usr_trash_dir = strdup(env->trash_dir);
        char *t_usr_uname = strdup(env->uname);

        char *const argv[] = {"find", t_usr_trash_dir, "-mindepth", "1", "-delete", NULL};
        char *const envp[] = {t_usr_uname, t_usr_trash_dir, NULL};

        execve(TRASHCTL_FIND_PATH, argv, envp);

    } else {

        if((err = waitpid(pid_rm, &status, 0)) == -1){
            fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
            return 1;
        }

        return 0;
    }

    return 0;
}

static int empty_one_file(struct environment_info* env, char* file_name)
{
    int err, status;
    errno = 0;

    pid_t pid_sh = fork();

    if(pid_sh < 0){
        fprintf(stderr, "[ERROR] %s\n", strerror(errno));
        return 1;
    } else if(pid_sh == 0){

        char usr_target_file_path[64] = {};
        char *t_usr_trash_dir = strdup(env->trash_dir);
        char *t_usr_uname = strdup(env->uname);

        strcpy(usr_target_file_path, t_usr_trash_dir);
        strcat(usr_target_file_path, file_name);

        printf("[DBG] target: %s\n", usr_target_file_path);

        char *const argv[] = {"find", usr_target_file_path, "-mindepth", "1", "-delete", NULL};
        char *const envp[] = {t_usr_uname, usr_target_file_path, NULL};

        execve(TRASHCTL_FIND_PATH, argv, envp);

    } else {

        if((err = waitpid(pid_sh, &status, 0)) == -1){
            fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
            return 1;
        }

        return 0;
    }

    return 0;
}

int trashctl_empty(struct environment_info* env)
{
    int err;

    //empty_all_files(env);
    empty_one_file(env, "test03");

    return 0;
}
