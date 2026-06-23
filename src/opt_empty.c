#include "../include/trashctl.h"
/* This will cover the empty command line option for trashctl */

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
    char usr_target_file_path[TRASHCTL_SUBPATH_LEN] = {};

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
