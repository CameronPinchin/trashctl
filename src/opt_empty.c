#include "../include/trashctl.h"
/* This will cover the empty command line option for trashctl */

/* The case where a user just calls trashctl empty, resulting in the entire trashbin being emptied */
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
        char *usr_trash_dir = strdup(env->trash_dir);
        size_t usr_uname_length = strlen(usr_trash_dir);
        char usr_uname_temp[64] = "USER=";
        char *usr_uname = strncat(usr_uname_temp, env->uname, usr_uname_length); // this is malloc'd, must be free()'d

        char *usr_trash_dir_rm_string = strcat(usr_trash_dir, "*");

        char *const argv[] = {"rm", usr_trash_dir_rm_string, NULL};
        char *const envp[] = {usr_uname, usr_trash_dir, NULL};

        execve(TRASHCTL_RM_PATH, argv, envp);

    } else {

        if((err = waitpid(pid_rm, &status, 0)) == -1){
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

    empty_all_files(env);

    return 0;

}
