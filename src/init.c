#include "../include/trashctl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <unistd.h>

static const char* valid_arguments[] = {TRASHCTL_ARG_PUT, TRASHCTL_ARG_LIST, \
    TRASHCTL_ARG_RESTORE, TRASHCTL_ARG_EMPTY, TRASHCTL_ARG_DELETE
};

static const char* standalone_arguments[] = {TRASHCTL_ARG_LIST, TRASHCTL_ARG_PUT, \
    TRASHCTL_ARG_EMPTY
};

static void help_message()
{
    fputs("traschtl [put, list, restore, empty, delete] [file]\n", stdout);
    fputs("     put [file]          Move a [file] to trash.\n", stdout);
    fputs("     list                List trashed files.\n", stdout);
    fputs("     restore             Restore a trashed file.\n", stdout);
    fputs("     empty               Empty the trash.\n", stdout);
    fputs("     empty [n]           Remove files older than [n] days.\n", stdout);
    fputs("     delete [pattern]    Delete a specific trashed file.\n", stdout);
}

static int init_validate_argument(const char* arg)
{
    int i;

    for(i = 0; i < 5; ++i){
        if(strcmp(arg, valid_arguments[i]) == 0){
            return 0;
        }
    }

    return 1;
}

/* Probably will find a better name for this.
    *  - intended for case '2', where list, restore, or empty do not require a third argument
    */
static int init_standalone_arg_check(const char* arg)
{
    int i;

    for(i = 0; i < 3; ++i){
        if(strcmp(arg, standalone_arguments[i]) == 0) {
            return 0;
        }
    }

    return 1;
}

static int env_info_populate(struct environment_info* env)
{
    int err;
    struct passwd* usr;
    errno = 0;

    usr = getpwuid(getuid());
    if(errno != 0) {
        fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
        return 1;
    }

    env->uname = usr->pw_name;
    env->home_dir = usr->pw_dir;
    env->trash_dir_len = ((strlen(env->home_dir)) + TRASHCTL_TRASH_DIR_LEN);

    if((err = snprintf(env->trash_dir_mut, env->trash_dir_len, "%s%s", env->home_dir, TRASHCTL_TRASH_DIR)) == -1) {
        fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
        return 1;
    }

    env->trash_dir = env->trash_dir_mut;

    return 0;
}

static int env_info_access_trash_dir(struct environment_info* env)
{
    int err;

    if((err = access(env->trash_dir, F_OK)) == -1) {
        fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
        return 1;
    }

    printf("%s exists.\n", env->trash_dir);
    return 0;
}

/* TO-DO: Need to do an environment check. I.e., do they have a trashbin directory? */
int initialize(int argc, char** argv)
{
    int err;
    struct environment_info env;

    switch(argc){
        case TRASHCTL_ARG_CNT_ONE:

            help_message();

            return EXIT_SUCCESS;
        case TRASHCTL_ARG_CNT_TWO:

            if((err = init_validate_argument(argv[1])) == 1) {
                return EXIT_FAILURE;
            }

            if((err = init_standalone_arg_check(argv[1])) == 1) {
                return EXIT_FAILURE;
            }
            /* argument is valid, and doesn't require a third argument */
            if((err = env_info_populate(&env)) == 1) {
                return EXIT_FAILURE;
            }

            /* argument is valid, and doesn't require a third argument */
            if((err = env_info_access_trash_dir(&env)) == 1) {
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;
        case TRASHCTL_ARG_CNT_THREE:

            if((err = init_validate_argument(argv[1])) == 1) {
                return EXIT_FAILURE;
            }

            /* a two-argument command shall not have three arguments */
            if((err = init_standalone_arg_check(argv[1])) == 1) {
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;
        default:

            return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}

