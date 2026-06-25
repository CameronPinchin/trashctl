#include "../include/trashctl.h"
#include <dirent.h>

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

    DIR* dirp = opendir(env->trash_dir);
    struct dirent* dir_entry;

    if(dirp == NULL){
        fprintf(stderr, "[ERROR] %s\n", strerror(errno));
        return 1;
    }

    while((dir_entry = readdir(dirp)) != NULL){
        if((strcmp(dir_entry->d_name, "..") == 0) || strcmp(dir_entry->d_name, ".") == 0){
            continue;
        }

        if((strcmp(dir_entry->d_name, file_name)) == 0){

            char tmp_trash[TRASHCTL_PATH_MAX] = { 0 };
            char tmp_info[TRASHCTL_PATH_MAX] = { 0 };
            const char* trash_path = tmp_trash;
            const char* info_path = tmp_info;

            construct_path(tmp_trash, env->trash_dir, dir_entry->d_name, NULL, TRASHCTL_PATH_MAX);
            construct_path(tmp_info, env->info_dir, dir_entry->d_name, ".trashinfo", TRASHCTL_PATH_MAX);

            if((err = unlink(trash_path)) != 0){
                fprintf(stderr, "[ERROR] %s\n", strerror(errno));
                return 1;
            }

            if((err = unlink(info_path)) != 0){
                fprintf(stderr, "[ERROR] %s\n", strerror(errno));
                return 1;
            }

            break;
        }
    }

    closedir(dirp);

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
