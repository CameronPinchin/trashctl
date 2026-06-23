#include "../include/trashctl.h"
/* This file covers the trashctl delete option. Logic currently housed within the opt_empty.c file, but is to be moved here. */

/**
 * @brief Initializes a subshell using the fork and exec flow.
 *
 * Starts a subshell if the fork is successful and runs a user-inputted command.
 *
 * @param[in] env A pointer to a environment_info struct.
 * @param[in] shell_command A pointer to a complete shell command to be run in the subshell.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
static int init_shell(struct environment_info* env, char* shell_command)
{
    int err, status, fd;
    errno = 0;
    fd = open(TRASHCTL_DEV_NULL_PATH, O_WRONLY);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    pid_t pid_sh = fork();

    if(pid_sh < 0){
        return 1;
    } else if(pid_sh == 0){
        /* child process */
        if((err = execl(TRASHCTL_SH_PATH, "sh", "-c", shell_command, NULL)) == -1){
            return 1;
        }
    } else {
        if((err = waitpid(pid_sh, &status, 0)) == -1){
            return 1;
        }
    }

    close(fd);

    return 0;
}

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
    char usr_target_file_path[TRASHCTL_SUBPATH_LEN] = {};

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
