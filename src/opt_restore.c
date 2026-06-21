#include "../include/trashctl.h"
/* This covers the case of moving files from your trash directory */

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
    cwd_len = strlen(cwd);
    cwd[cwd_len] = '/';

    char restore_file_command[256] = "mv ";
    char restore_file_path[512] = {};

    strlcpy(restore_file_path, env->trash_dir, sizeof(restore_file_path));
    restore_file_path_len = strlen(restore_file_path);

    char *cmd_ptr = restore_file_command;

    strcat(restore_file_path, file_name);
    restore_file_path_len = strlen(restore_file_path);
    restore_file_path[restore_file_path_len] = ' ';

    strcat(restore_file_path, cwd);
    strcat(restore_file_command, restore_file_path);

    if((err = init_shell(env, cmd_ptr)) == 1){
        fprintf(stderr, "[ERROR] Fork failed for restore command.\n");
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
