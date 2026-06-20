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

/* TEST: MACROS*/
#define TRASHCTL_LS_PATH            "/bin/ls"
#define TRASHCTL_RM_PATH            "/bin/rm"
#define TRASHCTL_FIND_PATH          "/bin/find"
#define TRASHCTL_SH_PATH            "/bin/sh"
#define TRASHCTL_DEV_NULL_PATH      "/dev/null"

/* INIT: DIRECTORY MACROS */
#define TRASHCTL_TRASH_DIR          "/.local/share/Trash/files/"
#define TRASHCTL_TRASH_DIR_LEN      ((size_t)27)

/* INIT: PARENT DIRECTORIES */
#define TRASHCTL_PARENT_DIR_LOCAL   ".local"
#define TRASHCTL_PARENT_DIR_SHARE   "share"
#define TRASHCTL_PARENT_DIR_TRASH   "Trash"
#define TRASHCTL_PARENT_DIR_FILES   "files"

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

//#define CMD_PUT                     1
//#define CMD_DELETE                  2
//#define CMD_EMPTY                   3
//#define CMD_LIST                    4

/* STRUCT DEFINITIONS */
struct environment_info {
    const char* uname;                /* Identifier for the username                        */
    const char* home_dir;             /* Identifier for the home directory of the user      */
    const char* trash_dir;            /* Identifier for the trash directory of the user     */
    char trash_dir_mut[256];          /* For the concatenation of home_dir and trash_dir    */
    size_t trash_dir_len;             /* To store the length of the pathname for trash_dir  */
};

/* FORWARD DECLARATIONS */
int initialize(int argc, char** argv);
int trashctl_list(struct environment_info* env);
int trashctl_empty(struct environment_info* env);
int trashctl_delete(struct environment_info* env, char* file_path);
int trashctl_put(struct environment_info* env, char* file_path);

#endif
