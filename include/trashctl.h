#include <stddef.h>

#ifndef TRASHCTL_H
#define TRASHCTL_H

/* INIT: DIRECTORY MACROS */
#define TRASHCTL_TRASH_DIR          "/.local/share/Trash/files"
#define TRASHCTL_TRASH_DIR_LEN      ((size_t)26)

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

/* FORWARD DECLARATIONS */
int initialize(int argc, char** argv);

/* STRUCT DEFINITIONS */
struct environment_info {
    const char* uname;                /* Identifier for the username                        */
    const char* home_dir;             /* Identifier for the home directory of the user      */
    const char* trash_dir;            /* Identifier for the trash directory of the user     */
    char trash_dir_mut[256];          /* For the concatenation of home_dir and trash_dir    */
    size_t trash_dir_len;             /* To store the length of the pathname for trash_dir  */
};

#endif
