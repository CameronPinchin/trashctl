#include "../include/trashctl.h"
#include <dirent.h>
/* This will cover the list command line option for trashctl */

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
    errno = 0;

    DIR *dir = opendir(env->trash_dir);
    struct dirent *dir_entry;

    if(dir == NULL){
        fprintf(stderr, "[ERROR] %s\n", strerror(errno));
        return 1;
    }

    while((dir_entry = readdir(dir)) != NULL){
        if((strcmp(dir_entry->d_name, "..") == 0) || strcmp(dir_entry->d_name, ".") == 0){
            continue;
        }
        printf("%s\n", dir_entry->d_name);
    }

    closedir(dir);
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
