#include "../include/trashctl.h"
/* This will cover the list command line option for trashctl */

static int trash_dir_access_check(struct environment_info *env)
{
    int err;
    errno = 0;

    if((err = access(env->trash_dir, F_OK)) == -1) {
        fprintf(stderr, "[ERROR]: %s \n", strerror(errno));
        return 1;
    }

    return 0;
}

static int trashctl_list_operation(struct environment_info *env)
{
    int err, status;
    errno = 0;

    pid_t pid_ls = fork();

    if(pid_ls < 0){
        fprintf(stderr, "[ERROR]: fork failure.\n");
        return 1;
    } else if (pid_ls == 0){

    /* TO-DO
     * - clean up this function (namely local variable initializations)
     */
    char *usr_trash_dir = strdup(env->trash_dir);
    size_t usr_uname_length = strlen(usr_trash_dir);
    char usr_uname_temp[64] = "USER=";
    char *usr_uname = strncat(usr_uname_temp, env->uname, usr_uname_length); // this is malloc'd, must be free()'d

    char *const argv[] = {"ls", "-l", usr_trash_dir, NULL};
    char *const envp[] = {usr_uname, usr_trash_dir, NULL};

    execve(TRASHCTL_LS_PATH, argv, envp);

    } else {

        if((err = waitpid(pid_ls, &status, 0)) == -1){
            fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
            return 1;
        }

        return 0;
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
