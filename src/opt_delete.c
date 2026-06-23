#include "../include/trashctl.h"
/* This file covers the trashctl delete option. Logic currently housed within the opt_empty.c file, but is to be moved here. */

/**
 * @brief Internal function called by trashctl_delete().
 *
 * Removes a file matching the trash_dir/file_name path.
 *
 * @param[in] env A pointer to a environment_info struct.
 * @param[in] file_name A pointer to a user-inputted file_name used to create a shell command.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
static int delete_file(struct environment_info* env, char* file_name)
{
    int err;
    errno = 0;

    char delete_file_command[TRASHCTL_SUBSHELL_CMD_LEN] = "rm ";
    char usr_target_file_path[TRASHCTL_SUBPATH_LEN] = {0};

    char *cmd_ptr = delete_file_command;

    strlcpy(usr_target_file_path, env->trash_dir, sizeof(usr_target_file_path));
    strlcat(usr_target_file_path, file_name, sizeof(usr_target_file_path));
    strlcat(delete_file_command, usr_target_file_path, sizeof(delete_file_command));

    if((err = init_shell(env, cmd_ptr) == 1)){
        fprintf(stderr, "[ERROR] Failed to open shell for file deletion.\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Exposed function called upon by initalize when a user inputs an 'delete' call.
 *
 * Simply calls empty_all_files(env) and returns its output.
 *
 * @param[in] env A pointer to a environment_info struct.
 * @param[in] file_name A pointer to a user-inputted file_name to be passed to init_shell().
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
int trashctl_delete(struct environment_info* env, char* file_name)
{
    return delete_file(env, file_name);
}
