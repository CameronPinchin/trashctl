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

    printf("%s exists.\n", env->trash_dir);
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
    }

    char *usr_trash_dir = strdup(env->trash_dir);
    fprintf(stderr, "[DBG] usr_trash_dir: %s\n[DBG] env->trash_dir: %s\n", usr_trash_dir, env->trash_dir);

    char *const argv[] = {"ls", "-l", NULL};
    char *const envp[] = {"USER=root", usr_trash_dir, NULL};

    execve(TRASHCTL_LS_PATH, argv, envp);

    fprintf(stderr, "[DBG] pid_ls: %d\n", pid_ls);

    if((err = waitpid(pid_ls, &status, 0)) == -1){
        fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
        return 1;
    }

    fprintf(stderr, "Parent: child process has terminated.\n");
    return 0;
}

int trashctl_list(struct environment_info* env)
{
    /* check for */
    int err;
    errno = 0;

    fprintf(stderr, "[DBG] trashctl, env->trash_dir: %s\n", env->trash_dir);

    if((err = trash_dir_access_check(env)) == 1){
        fputs("[ERROR]: trashctl was unable to find your trash directory.\n", stderr);
        return 1;
    }

    if((err = trashctl_list_operation(env)) == 1){
        fputs("[ERROR]: trashctl was unable to list files from your trash directory.\n", stderr);
        return 1;
    }

    return 0;
}
