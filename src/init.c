#include "../include/trashctl.h"

static const char* valid_arguments[] = {TRASHCTL_ARG_PUT, TRASHCTL_ARG_LIST, \
    TRASHCTL_ARG_RESTORE, TRASHCTL_ARG_EMPTY, TRASHCTL_ARG_DELETE
};

enum commands { CMD_UNKNOWN, CMD_PUT, CMD_LIST, CMD_EMPTY, CMD_DELETE, CMD_RESTORE };

/**
 * @brief Takes a pointer to a user-inputted argument and determines its validity.
 *
 * Compares and constrasts an inputted user command against four acceptable options to determine what action to take.
 *
 * @param[in] cmd A pointer to a user-inputted command.
 * @return The integer representation of the command derived from the commands enum.
 */
static enum commands get_command_id(const char* cmd)
{
    if(strcmp(cmd, "put") == 0){ return CMD_PUT; }
    if(strcmp(cmd, "list") == 0){ return CMD_LIST; }
    if(strcmp(cmd, "empty") == 0){ return CMD_EMPTY; }
    if(strcmp(cmd, "delete") == 0){ return CMD_DELETE; }
    if(strcmp(cmd, "restore") == 0){ return CMD_RESTORE; }
    return CMD_UNKNOWN;
}

/**
 * @brief Prints out a help message.
 *
 * Prints out the default help message when trashctl is run without any parameters.
 */
static void help_message()
{
    fputs("traschtl [put, list, restore, empty, delete] [file]\n", stdout);
    fputs("     put [file]          Move a [file] to trash.\n", stdout);
    fputs("     list                List trashed files.\n", stdout);
    fputs("     restore             Restore a trashed file.\n", stdout);
    fputs("     empty               Empty the trash.\n", stdout);
    fputs("     delete [pattern]    Delete a specific trashed file.\n", stdout);
}

/**
 * @brief Takes a pointer to a user-inputted argument and determines its validity.
 *
 * Compares and constrasts an inputted user command against four acceptable options to determine what action to take.
 *
 * @param[in] arg A pointer to a user-inputted command.
 * @return If the input is valid, 0 is returned on success. If the function fails, 1 will be returned.
 */
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

/**
 * @brief Populates an environment_info struct with user-specific information.
 *
 * Takes in a pointer to a environment_info struct and populates each field based on user information.
 *
 * @param[in] env A pointer to a environment_info struct instantiated in the initialize function.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
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
    env->trash_dir_len = ((strnlen(env->home_dir, TRASHCTL_MAX_HOME_DIR_LEN)) + TRASHCTL_TRASH_DIR_LEN);

    if((err = snprintf(env->trash_dir_mut, env->trash_dir_len, "%s%s", env->home_dir, TRASHCTL_TRASH_DIR)) == -1) {
        fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
        return 1;
    }
    env->trash_dir = env->trash_dir_mut;

    strlcpy(env->info_dir_mut, env->home_dir, TRASHCTL_PATH_MAX); // copy home directory into empty char array
    strlcat(env->info_dir_mut, TRASHCTL_INFO_DIR, TRASHCTL_PATH_MAX);

    env->info_dir = env->info_dir_mut;

    return 0;
}

/**
 * @brief Determines if the users trash directory exists.
 *
 * Takes in a pointer to a environment_info struct and determines if the trash directory exists based on the trash_dir field.
 *
 * @param[in] env A pointer to a environment_info struct.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
static int init_check_for_trash_dir(struct environment_info* env)
{
    int err;

    if((err = access(env->trash_dir, F_OK)) == -1) {
        fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
        return 1;
    }

    return 0;
}

static int init_check_for_info_dir(struct environment_info* env)
{
    int err;

    if((err = access(env->info_dir, F_OK)) == -1) {
        fprintf(stderr, "[ERROR]: %s\n", strerror(errno));
        return 1;
    }
    return 0;
}

static void mkdir_p(const char* dir_path)
{
    mode_t dir_mode = S_IFDIR | S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH;
    char tmp[TRASHCTL_PATH_MAX];
    char* p;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", dir_path);
    len = strnlen(tmp, TRASHCTL_PATH_MAX);

    if(tmp[len - 1] == '/'){
        tmp[len - 1] = 0;
    }

    for(p = tmp + 1; *p; p++){
        if(*p == '/'){
            *p = 0;
            mkdir(tmp, dir_mode);
            *p = '/';
        }
    }
    mkdir(tmp, dir_mode);
}

/**
 * @brief Parses a user-inputted argument to determine what action to take.
 *
 * Determines the action to take based on a user-inputted argument then calls the appropriate function.
 *
 * @param[in] env A pointer to a environment_info struct instantiated in the initialize function.
 * @param[in] arg A pointer to a user-inputted argument used to determine the command ID.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
static int parse_command(struct environment_info* env, char* arg)
{
    switch(get_command_id(arg)) {
        case CMD_LIST:
            return trashctl_list(env);
        case CMD_EMPTY:
            return trashctl_empty(env);
        default:
            return 1;
    }
}

/**
 * @brief Parses a user-inputted argument to determine what action to take.
 *
 * Determines the action to take based on a user-inputted argument then calls the appropriate function.
 *
 * @param[in] env A pointer to a environment_info struct instantiated in the initialize function.
 * @param[in] arg_1 A pointer to a user-inputted argument used to determine the command ID.
 * @param[in] arg_2 A pointer to a user-inputted argument that is used to identify the target file for the user.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
static int parse_command_with_file(struct environment_info* env, char* arg_1, char* arg_2)
{
    switch(get_command_id(arg_1)) {
        case CMD_DELETE:
            return trashctl_delete(env, arg_2);
        case CMD_PUT:
            return trashctl_put(env, arg_2);
        case CMD_RESTORE:
            return trashctl_restore(env, arg_2);
        default:
            return 1;
    }
}

/**
 * @brief Parent function for init.c that is called upon in trashctl.c. Used to initialize all aspects of trashctl.
 *
 * Initalizes all necessary components of trashctl and determines what action to take based on user-input.
 *
 * @param[in] argc An integer representation of the number of commands passed to trashctl.
 * @param[in] argv An array of char pointers that contain each inputted argument.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
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

            if((err = env_info_populate(&env)) == 1) {
                return EXIT_FAILURE;
            }

            if((err = init_check_for_trash_dir(&env)) == 1) {
                mkdir_p(env.home_dir);
            }

            if((err = init_check_for_info_dir(&env)) == 1){
                mkdir_p(env.info_dir);
            }

            if((err = parse_command(&env, argv[1])) == 1){
                fprintf(stderr, "Sorry! We couldn't map argument: [%s] to any of our commands. Check your input and try again.\n", argv[1]);
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;
        case TRASHCTL_ARG_CNT_THREE:

            if((err = init_validate_argument(argv[1])) == 1) {
                return EXIT_FAILURE;
            }

            if((err = env_info_populate(&env)) == 1) {
                return EXIT_FAILURE;
            }

            if((err = init_check_for_trash_dir(&env)) == 1) {
                mkdir_p(env.home_dir);
            }

            if((err = init_check_for_info_dir(&env)) == 1){
                mkdir_p(env.info_dir);
            }

            if((err = parse_command_with_file(&env, argv[1], argv[2])) == 1){
                fprintf(stderr, "Sorry! We couldn't map arguments: [%s, %s] to any of our commands. Check your input and try again.\n", argv[1], argv[2]);
                return EXIT_FAILURE;
            }

            return EXIT_SUCCESS;
        default:

            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
