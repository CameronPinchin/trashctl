#include "../include/trashctl.h"
/* This covers the case of moving files from your trash directory */

/**
 * @brief Internal function called by trashctl_restore().
 *
 * Removes a file matching the file_name input from the users Trash directory into the current working directory.
 *
 * @param[in] env A pointer to a environment_info struct.
 * @param[in] file_name A pointer to a user-inputted file_name used to create a shell command.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
static int restore_file(struct environment_info* env, char* file_name)
{
    int err, restore_file_path_len, cwd_len;
    errno = 0;

    char cwd[256] = {};
    getcwd(cwd, sizeof(cwd));
    cwd_len = strnlen(cwd, TRASHCTL_SUBSHELL_CMD_LEN);
    cwd[cwd_len] = '/';

    char restore_file_command[TRASHCTL_SUBSHELL_CMD_LEN] = "mv ";
    char restore_file_path[TRASHCTL_SUBPATH_LEN] = {0};

    strlcpy(restore_file_path, env->trash_dir, sizeof(restore_file_path));
    restore_file_path_len = strnlen(restore_file_path, TRASHCTL_SUBPATH_LEN);

    char *cmd_ptr = restore_file_command;

    strlcat(restore_file_path, file_name, sizeof(restore_file));
    restore_file_path_len = strnlen(restore_file_path, TRASHCTL_SUBPATH_LEN);
    restore_file_path[restore_file_path_len] = ' ';

    strlcat(restore_file_path, cwd, sizeof(restore_file_path));
    strlcat(restore_file_command, restore_file_path, sizeof(restore_file_command));

    if((err = init_shell(env, cmd_ptr)) == 1){
        return 1;
    }

    return 0;
}

/**
 * @brief Exposed function called upon by initalize when a user inputs an 'restore' call.
 *
 * Simply calls restore_file(env, file_name) and returns its output.
 *
 * @param[in] env A pointer to a environment_info struct.
 * @param[in] file_name A pointer to a user-inputted file_name to be passed to init_shell().
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
int trashctl_restore(struct environment_info* env, char* file_name)
{
    return restore_file(env, file_name);
}
