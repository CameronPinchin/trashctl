#include "../include/trashctl.h"
/* This will cover the empty command line option for trashctl */

/**
 * @brief Internal function called by trashctl_empty().
 *
 * Removes all files based on the users trash directory. This function is internal and called upon by trashctl_empty().
 *
 * @param[in] env A pointer to a environment_info struct.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
static int empty_all_files(struct environment_info* env)
{
    int err;
    errno = 0;

    char empty_all_files_command[TRASHCTL_SUBSHELL_CMD_LEN] = "rm ";
    char usr_target_file_path[TRASHCTL_SUBPATH_LEN] = {0};

    char *cmd_ptr = empty_all_files_command;

    //strcpy(usr_target_file_path, env->trash_dir); // change to strlcpy()
    strlcpy(usr_target_file_path, env->trash_dir, sizeof(usr_target_file_path));
    strlcat(usr_target_file_path, "*", sizeof(usr_target_file_path));
    strlcat(empty_all_files_command, usr_target_file_path, sizeof(empty_all_files_command));

    if((err = init_shell(env, cmd_ptr) == 1)){
        fprintf(stderr, "[ERROR] Fork failure\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Exposed function called upon by initalize when a user inputs an 'empty' call.
 *
 * Simply calls empty_all_files(env) and returns its output.
 *
 * @param[in] env A pointer to a environment_info struct.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
int trashctl_empty(struct environment_info* env)
{
    return empty_all_files(env);
}
