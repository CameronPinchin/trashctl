#include "../include/trashctl.h"
#include <time.h>
/* This covers the case of moving files to your trash directory */

static int put_create_info_entry(struct environment_info* env, const char* original_file_path, char* file_name)
{
    mode_t info_file_perms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

    int fd, formatted_length;

    char tmp[TRASHCTL_PATH_MAX] = { 0 };
    char formatted_data[TRASHCTL_BUF_MAX] = {0};
    char timestamp_str[32];
    const char* file_name_with_suffix = tmp;
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    construct_path(tmp, env->info_dir, file_name, ".trashinfo", TRASHCTL_PATH_MAX);

    if((fd = open(file_name_with_suffix, O_WRONLY | O_CREAT | O_TRUNC, info_file_perms )) < 0){
        fprintf(stderr, "[ERROR] Failed to open file.\n");
        return 1;
    }

    strftime(timestamp_str, sizeof(timestamp_str), "%Y-%m-%dT%H:%M:%S", local); //2026-05-04T11:08:18

    formatted_length = snprintf(formatted_data, TRASHCTL_BUF_MAX,"[Trash Info]\nPath=%s\nDeletionDate=%s\n", original_file_path, timestamp_str);

    ssize_t bytes_written = write(fd, formatted_data, formatted_length);
    if(bytes_written < 0){
        fprintf(stderr, "[ERROR] %s\n", strerror(errno));
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

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
    int err;
    errno = 0;

    char tmp_new_file_path[TRASHCTL_PATH_MAX] = { 0 };
    char tmp[TRASHCTL_PATH_MAX] = {0};
    const char* original_file_path = tmp;
    const char* new_file_path = tmp_new_file_path;

    getcwd(tmp, sizeof(tmp));

    construct_path(tmp_new_file_path, env->trash_dir, file_name, NULL, TRASHCTL_PATH_MAX);
    construct_path(tmp, NULL, "/", file_name, TRASHCTL_PATH_MAX);
    // move files
    if((err = rename(original_file_path, new_file_path) != 0)){
        fprintf(stderr, "[ERROR] Failed to rename file.\n");
        return 1;
    }
    // create .trashinfo entry
    if((err = put_create_info_entry(env, original_file_path, file_name)) == 1){
        fprintf(stderr, "[ERROR] Failed to create .trashinfo file entry.\n");
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
