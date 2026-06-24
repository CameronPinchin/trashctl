#include "../include/trashctl.h"
#include <dirent.h>
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
    errno = 0;

    DIR *dirp = opendir(env->trash_dir);
    struct dirent *dir_entry;

    if(dirp == NULL){
        fprintf(stderr, "[ERROR] %s\n", strerror(errno));
        return 1;
    }
    /* if a Trashed file is deleted, its corresponding info file must also be deleted. */
    while((dir_entry = readdir(dirp)) != NULL){
        if((strcmp(dir_entry->d_name, "..") == 0) || strcmp(dir_entry->d_name, ".") == 0){
            continue;
        }

        char tmp_trash[TRASHCTL_PATH_MAX] = { 0 };
        char tmp_info[TRASHCTL_PATH_MAX] = { 0 };

        strlcpy(tmp_trash, env->trash_dir, TRASHCTL_PATH_MAX);
        strlcat(tmp_trash, dir_entry->d_name, TRASHCTL_PATH_MAX);
        strlcpy(tmp_info, env->info_dir, TRASHCTL_PATH_MAX);
        strlcat(tmp_info, dir_entry->d_name, TRASHCTL_PATH_MAX);
        strlcat(tmp_info, ".trashinfo", TRASHCTL_PATH_MAX);

        const char* trash_path = tmp_trash;
        const char* info_path = tmp_info;

        unlink(trash_path);
        unlink(info_path);
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
