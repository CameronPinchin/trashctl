#include "../include/trashctl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>
#include <unistd.h>

static const char* valid_arguments[] = {TRASHCTL_ARG_PUT, TRASHCTL_ARG_LIST, \
    TRASHCTL_ARG_RESTORE, TRASHCTL_ARG_EMPTY, TRASHCTL_ARG_DELETE
};

static const char* standalone_arguments[] = {TRASHCTL_ARG_LIST, TRASHCTL_ARG_PUT, \
    TRASHCTL_ARG_EMPTY
};

static const char* parent_directories[] = {TRASHCTL_PARENT_DIR_LOCAL, TRASHCTL_PARENT_DIR_SHARE, \
    TRASHCTL_PARENT_DIR_TRASH, TRASHCTL_PARENT_DIR_FILES
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

static int trash_dir_access_check(struct environment_info* env)
{
    int err;
    errno = 0;

    if((err = access(env->trash_dir, F_OK)) == -1) {
        fprintf(stderr, "[ERROR]: %s (must create trash_dir)\n", strerror(errno));
        return 1;
    }

    printf("%s exists.\n", env->trash_dir);
    return 0;
}

/* this fails currently because its parent directories do not exist, need to make a recursive version
 *  - good function for a personal library aswell
 */
static int trash_dir_create_dir_p(struct environment_info* env)
{
    int err;
    mode_t dir_mode = S_IFDIR | S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; //755
    errno = 0;

    if((err = mkdir(env->trash_dir, dir_mode)) == -1) {
        fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
        return 1;
    }

    printf("%s has been successfully created.\n", env->trash_dir);
    return 0;
}

// recursive ver in charge of creating parent dirs 
// TO-DO: Review and make test cases for this function
/*
 * Currently, this function is incorrectly building the path for the trashbin.
 *  For ex:
 *      - expected: /home/$USER/.local/share/Trash/files
 *      - actual: /home
 *                /$USER
 *                /.local
 *                /share
 *                /Trash
 *                /files
 *
 * Such that the buffer clears itself through each iteration of the loop.
 *
 *  This is attributable to working_directory. I never actual build working_directory but rely on it for path construction.
 *   - Just need to additional be building working_directory at the same time.
 *   - The reason I have two is for temporary storage as you cannot
 **/

static int trash_dir_create_dir_p_(struct environment_info* env)
{
    int err, i;
    mode_t dir_mode = S_IFDIR | S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    char working_directory[256] = "/home/cameron"; /* opts: env->home_dir (fix), /home/cameron*/
    
    memset(working_directory, 0, sizeof(working_directory));

    size_t capacity = sizeof(working_directory);
    size_t offset = strlen(working_directory);
    size_t remaining = capacity;
    int written;
    for(i = 0; i < 4; ++i) {
        fprintf(stderr, "[%d] c: %ld o: %ld\n", i, capacity, offset);
        remaining = capacity - offset;
        if((err = snprintf(working_directory + offset, remaining, "/%s", parent_directories[i])) == -1){
            fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
            return 1;
        }
        fprintf(stderr, "[characters written] %d\n", err);
        offset += err;
    }
    fprintf(stderr, "[success]: %s\n", working_directory);
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

            if((err = env_info_populate(&env)) == 1) {
                return EXIT_FAILURE;
            }
            /* if this function returns 1, we need to create the trash directory */
            if((err = trash_dir_access_check(&env)) == 1) {
                if((err = trash_dir_create_dir_p_(&env)) == 1) {
                    return EXIT_FAILURE;
                }
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

