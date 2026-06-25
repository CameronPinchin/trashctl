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
    /* THIS NEEDS TO BE SPLIT INTO MULTIPLE FUNCTIONS, MAYBE TWO OR THREE */


    int fd, err;
    char trash_file_tmp[TRASHCTL_PATH_MAX] = { 0 };
    char info_file_tmp[TRASHCTL_PATH_MAX] = { 0 };
    char original_file_tmp[TRASHCTL_PATH_MAX] = { 0 };
    const char* info_file = info_file_tmp;
    const char* org_file = original_file_tmp;
    const char* trash_file = trash_file_tmp;

    // strlcpy(info_file_tmp, env->info_dir, TRASHCTL_PATH_MAX);
    // strlcat(info_file_tmp, file_name, TRASHCTL_PATH_MAX);
    // strlcat(info_file_tmp, ".trashinfo", TRASHCTL_PATH_MAX);
    construct_path(info_file_tmp, env->info_dir, file_name, ".trashinfo", TRASHCTL_PATH_MAX);
    //strlcpy(trash_file_tmp, env->trash_dir, TRASHCTL_PATH_MAX);
    //strlcat(trash_file_tmp, file_name, TRASHCTL_PATH_MAX);
    construct_path(trash_file_tmp, env->trash_dir, file_name, NULL, TRASHCTL_PATH_MAX);

    if((fd = open(info_file, O_RDONLY)) < 0){
        fprintf(stderr, "[ERROR] %s\n", strerror(errno));
        return 1;
    }

    FILE *filep = fdopen(fd, "r");
    if(filep == NULL){
        fprintf(stderr, "[ERROR] %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    char line[TRASHCTL_LINE_MAX] = { 0 };
    const char* prefix = "Path=";
    size_t prefix_length = strlen(prefix);

    while(fgets(line, TRASHCTL_LINE_MAX, filep) != NULL){
        if(strncmp(line, prefix, prefix_length) == 0){
            strncpy(original_file_tmp, line + prefix_length, TRASHCTL_PATH_MAX);
            size_t len = strlen(original_file_tmp);
            original_file_tmp[len - 1] = '\0';
            break;
        }
    }

    if((err = rename(trash_file, org_file)) != 0){
        fprintf(stderr, "[ERROR] Failed to move file.\n");
        return 1;
    }

    if((err = unlink(info_file)) != 0){
        fprintf(stderr, "[ERROR] Failed to delete trashinfo file.\n");
        return 1;
    }

    fclose(filep);

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
