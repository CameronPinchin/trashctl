#include "../include/trashctl.h"

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

static int trash_dir_path_creation(struct environment_info* env)
{
    char usr_uname_temp[64];
    strcpy(usr_uname_temp, env->uname);
    strcat(usr_uname_temp, TRASHCTL_TRASH_DIR); // free this
    env->trash_dir = strdup(usr_uname_temp);
    printf("[DBG] env->trash_dir (pathcreate): %s\n", env->trash_dir);
    return 0;
}

/* the reason this always failes is due to env->trash_dir always being uninitalized at the time this function is called*/
static int trash_dir_access_check(struct environment_info* env)
{
    int err;
    errno = 0;
    printf("[DBG] trash_dir path, in access_check: %s\n", env->trash_dir);
    if((err = access(env->trash_dir, F_OK)) == -1) {
        fprintf(stderr, "[ERROR]: %s (must create trash_dir)\n", strerror(errno));
        return 1;
    }

    return 0;
}


/* this still needs work, as it only uses the static /home/cameron option at the moment.
 *  - Path construction works though, and the directories do get created.
 */
static int trash_dir_create_dir_p(struct environment_info* env)
{
    int err, i;
    mode_t dir_mode = S_IFDIR | S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    char working_directory[256] = "/home/cameron"; /* opts: env->home_dir (fix), /home/cameron */
    
    size_t capacity = sizeof(working_directory);
    size_t offset = strlen(working_directory);
    size_t remaining = capacity;
    int written;
    for(i = 0; i < 4; ++i) {
        fprintf(stderr, "[%d] c: %ld o: %ld\n", i, capacity, offset);
        remaining = capacity - offset;

        if((written = snprintf(working_directory + offset, remaining, "/%s", parent_directories[i])) == -1){
            fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
            return 1;
        }
        offset += written;

        const char *path = working_directory;
        if((err = mkdir(path, dir_mode)) == -1){
            fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
            continue;
        }
    }
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

            if((err = trash_dir_access_check(&env)) == 1) {
                if((err = trash_dir_create_dir_p(&env)) == 1) {
                    return EXIT_FAILURE;
                }
            }

            /* test trashctl list */
            trashctl_list(&env);

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

