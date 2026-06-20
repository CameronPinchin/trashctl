#include "../include/trashctl.h"
/* This will cover the list command line option for trashctl */

static int trashctl_list_operation(struct environment_info *env)
{
    int err, status;
    errno = 0;

    pid_t pid_ls = fork();

    if(pid_ls < 0){
        fprintf(stderr, "[ERROR]: fork failure.\n");
        return 1;
    } else if (pid_ls == 0){

    char *t_usr_trash_dir = strdup(env->trash_dir);
    char *t_usr_uname = strdup(env->uname);

    char *const argv[] = {"ls", "-l", t_usr_trash_dir, NULL};
    char *const envp[] = {t_usr_uname, t_usr_trash_dir, NULL};

    execve(TRASHCTL_LS_PATH, argv, envp);

    free(t_usr_uname);
    free(t_usr_trash_dir);

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
