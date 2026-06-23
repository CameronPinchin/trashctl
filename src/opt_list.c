#include "../include/trashctl.h"
/* This will cover the list command line option for trashctl */

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
    int err, status;
    errno = 0;

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

    return 0;
}


/**
 * @brief Internal function called by trashctl_list().
 *
 * Performs string manipulation to create a valid shell command and passes it to init_shell().
 *
 * @param[in] env A pointer to a environment_info struct.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
static int trashctl_list_operation(struct environment_info *env)
{
    int err;
    errno = 0;

    char list_files_command[TRASHCTL_SUBSHELL_CMD_LEN] = "ls -l ";
    char *cmd_ptr = list_files_command;

    strlcat(list_files_command, env->trash_dir, sizeof(list_files_command));

    if((err = init_shell(env, cmd_ptr) == 1)){
        fprintf(stderr, "[ERROR] Fork failed for list operation.\n");
        return 1;
    }

    return 0;
}

/**
 * @brief Exposed function called upon by initalize when a user inputs an 'list' call.
 *
 * Simply calls trashctl_list_operation(env).
 *
 * @param[in] env A pointer to a environment_info struct.
 * @return If the operation is successful, 0 is returned. Otherwise, a 1 is returned on failure.
 */
int trashctl_list(struct environment_info* env)
{
    int err;
    errno = 0;

    if((err = trashctl_list_operation(env)) == 1){
        fputs("[ERROR]: trashctl was unable to list files from your trash directory.\n", stderr);
        return 1;
    }

    return 0;
}
