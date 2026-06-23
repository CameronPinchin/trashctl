#include "../include/trashctl.h"
/* This covers the case of moving files to your trash directory */

/**
 * @brief Internal function called by trashctl_put().
 *
 * Moves the file matching file_name into the users trash directory.
 *
 * @param[in] env A pointer to a environment_info struct.
 * @param[in] file_name A pointer to a user-inputted file_name used to create a shell command.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
static int put_file(struct environment_info* env, char* file_name)
{
    int err, cwd_length;
    errno = 0;

    char cwd[TRASHCTL_SUBSHELL_CMD_LEN] = {0};
    getcwd(cwd, sizeof(cwd));
    cwd_length = strnlen(cwd, TRASHCTL_SUBSHELL_CMD_LEN);

    char put_file_command[TRASHCTL_SUBSHELL_CMD_LEN] = "mv ";
    char put_file_path[TRASHCTL_SUBPATH_LEN] = {0};

    strlcpy(put_file_path, env->trash_dir, sizeof(put_file_path));

    char *cmd_ptr = put_file_command;
    cwd[cwd_length] = '/';
    strlcat(cwd, file_name, sizeof(cwd));
    cwd_length = strnlen(cwd, TRASHCTL_SUBPATH_LEN);
    cwd[cwd_length] = ' ';

    strlcat(cwd, put_file_path, sizeof(cwd));
    strlcat(put_file_command, cwd, sizeof(put_file_command));

    if((err = init_shell(env, cmd_ptr)) == 1){
        fprintf(stderr, "[ERROR] Fork failed for put command.\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Exposed function called upon by initalize when a user inputs an 'put' call.
 *
 * Simply calls put_file(env, file_name) and returns its output.
 *
 * @param[in] env A pointer to a environment_info struct.
 * @param[in] file_name A pointer to a user-inputted file_name to be passed to init_shell().
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
int trashctl_put(struct environment_info* env, char* file_name)
{
    return put_file(env, file_name);
}
