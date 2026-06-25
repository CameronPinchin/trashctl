#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <pwd.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#ifndef TRASHCTL_H
#define TRASHCTL_H

#define TRASHCTL_VER                0.2.2 /* MAJOR, MINOR (new stuff), PATCH (bugs,cleanup)*/

#define TRASHCTL_PATH_MAX           256
#define TRASHCTL_BUF_MAX            512
#define TRASHCTL_LINE_MAX           1024

/* TEST: MACROS*/
#define TRASHCTL_LS_PATH            "/bin/ls"
#define TRASHCTL_RM_PATH            "/bin/rm"
#define TRASHCTL_FIND_PATH          "/bin/find"
#define TRASHCTL_SH_PATH            "/bin/sh"
#define TRASHCTL_DEV_NULL_PATH      "/dev/null"

/* INIT: DIRECTORY MACROS */
#define TRASHCTL_TRASH_DIR          "/.local/share/Trash/files/"
#define TRASHCTL_INFO_DIR           "/.local/share/Trash/info/"
#define TRASHCTL_TRASH_DIR_LEN      ((size_t)27)
#define TRASHCTL_MAX_HOME_DIR_LEN   ((size_t)40) /* Usernames can be at most 32 characters, and the home prefix is 6 */

/* INIT: PARENT DIRECTORIES */
#define TRASHCTL_PARENT_DIR_LOCAL   ".local"
#define TRASHCTL_PARENT_DIR_SHARE   "share"
#define TRASHCTL_PARENT_DIR_TRASH   "Trash"
#define TRASHCTL_PARENT_DIR_FILES   "files"
#define TRASHCTL_PARENT_DIR_INFO    "info"

/* ARGUMENT: MACROS */
#define TRASHCTL_ARG_CNT_ONE        1
#define TRASHCTL_ARG_CNT_TWO        2
#define TRASHCTL_ARG_CNT_THREE      3
#define TRASHCTL_ARG_CNT_FOUR       4

/* ARGUMENT: VALID INPUTS */
#define TRASHCTL_ARG_PUT            "put"
#define TRASHCTL_ARG_LIST           "list"
#define TRASHCTL_ARG_RESTORE        "restore"
#define TRASHCTL_ARG_EMPTY          "empty"
#define TRASHCTL_ARG_DELETE         "delete"


struct trashctl_env {
    char p_trash_dir[TRASHCTL_PATH_MAX];
    char p_info_dir[TRASHCTL_PATH_MAX];
    char p_home_dir[TRASHCTL_PATH_MAX];
};

/* STRUCT DEFINITIONS */
struct environment_info {
    const char* uname;                              /* Identifier for the username                        */
    const char* home_dir;                           /* Identifier for the home directory of the user      */
    const char* trash_dir;                          /* Identifier for the trash directory of the user     */
    const char* info_dir;
    char trash_dir_mut[TRASHCTL_PATH_MAX];          // this is awkward and should be either changed or removed
    char info_dir_mut[TRASHCTL_PATH_MAX];
    size_t trash_dir_len;                           /* To store the length of the pathname for trash_dir  */
    struct trashctl_env trashenv;
};

/* Consider:
   struct trashctl_env {
        char trash_dir[TRASHCTL_PATH_MAX];
        char info_dir[TRASHCTL_PATH_MAX];
        char home_dir[TRASHCTL_PATH_MAX];
   }

 */

/* FORWARD DECLARATIONS */
int initialize(int argc, char** argv);
int trashctl_list(struct environment_info* env);
int trashctl_empty(struct environment_info* env);
int trashctl_delete(struct environment_info* env, char* file_path);
int trashctl_put(struct environment_info* env, char* file_path);
int trashctl_restore(struct environment_info* env, char* file_path);
int init_shell(struct environment_info* env, char* shell_command);
void construct_path(char *buf, const char* trashdir, const char* file, const char* opt_srcprefix, int size);

#endif
