#include "../include/trashctl.h"

static const char* valid_arguments[] = {TRASHCTL_ARG_PUT, TRASHCTL_ARG_LIST, \
    TRASHCTL_ARG_RESTORE, TRASHCTL_ARG_EMPTY, TRASHCTL_ARG_DELETE
};

static const char* standalone_arguments[] = {TRASHCTL_ARG_LIST, TRASHCTL_ARG_PUT, \
    TRASHCTL_ARG_EMPTY, TRASHCTL_ARG_DELETE
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

    for(i = 0; i < 4; ++i){
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

    return 0;
}

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

/* This will likely need redoing, originally I wanted to use a switch-case statement but strcmp() cannot be used.
 *   (switch-case statements require case labels to be compile-time constants, of which strcmp() is not )
 */
static int parse_command(struct environment_info* env, char* arg)
{
    int err;

    if(strcmp(arg, "empty") == 0){
        return trashctl_empty(env);
    } else if(strcmp(arg, "list") == 0){
        return trashctl_list(env);
    } else {
        fprintf(stderr, "[ERROR] Argument input is invalid, please try again.\n");
        return 1;
    }

}

static int parse_command_with_file(struct environment_info* env, char* arg_1, char* arg_2)
{
    if(strcmp(arg_1, "delete") == 0){
        return trashctl_delete(env, arg_2);
    } else if(strcmp(arg_1, "put") == 0){
        return trashctl_put(env, arg_2);
    } else {
        fprintf(stderr, "[ERROR] Argument input is invalid, please try again.\n");
        return 1;
    }
}

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

            /* Need a switch-case function to determine what action to take based on argv[1] */
            /* Add parsing logic to determine what the second argument is and proceed accordingly */
            /*  i.e., list vs empty are both two argument commands */

            /* test trashctl list */
            //trashctl_list(&env);

            /* test trashctl empty*/
            //trashctl_empty(&env);

            parse_command(&env, argv[1]);

            return EXIT_SUCCESS;
        case TRASHCTL_ARG_CNT_THREE:

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

            /* test trashctl delete */
            //trashctl_delete(&env, argv[2]);

            /* test trashctl put */
            //trashctl_put(&env, argv[2]);

            parse_command_with_file(&env, argv[1], argv[2]);


            return EXIT_SUCCESS;
        default:

            return EXIT_FAILURE;
    }


    return EXIT_SUCCESS;
}

