
# **trashctl**
**Author: Cameron Pinchin | <cwpinchin@outlook.com>**

There is a Bash-based utility called trash-cli that provides a utility to manage your *trashbin* on Linux systems. This aims to emulate that functionality, written in C. There are other C-based versions of this, so this is more of a learning project.

## trashctl | Overview

Why trashctl? This project aims to emulate the 'Trash' or 'Recycling Bin' functionality found on Linux (non-headless) and Windows environments. It provides users options to 'put' a file into the Trash, 'empty' the Trash to clear up space, 'list' all current contents of the Trash directory, and 'delete' a specific file from the Trash directory.

## trashctl | Usage

How do you use trashctl? Here are short descriptions of each command with an example of its usage.

#### trashctl list

The **list** command writes out a list of files in the users Trash directory. If the directory is empty, the user will be informed that 0 files have been found.

Example: *> trashctl list* 

![list demo](https://i.imgur.com/Iefh6JF.gif)

#### trashctl empty

The **empty** command removes all files in the users Trash directory. If the directory is empty, the user will be informed that 0 files have been found.

Example: *> trashctl empty* 

![empty demo](https://i.imgur.com/FVKkxHx.gif)

#### trashctl delete file_path

The **delete** command deletes a specific file contained within the users Trash directory. If a file matching the file_path argument is not found, nothing will be returned.

Example: *> trashctl delete file_name* 

![delete demo](https://i.imgur.com/fwWdH7P.gif)

#### trashctl put

The **put** command moves a file from the current working directory into the users Trash directory. If a file matching the file_name argument is not found, the user will be informed that the file could not be identified.

Example: *> trashctl put file_name* 

![put demo](https://i.imgur.com/BUiYT9P.gif)

## trashctl | Trash Directory Location

For most Linux distributions, the Trash directory can be found at:
***/home/${USER}/.local/share/Trash/files/***

Trashctl will use this directory if it exists, or create the directory if it doesn't.

## trashctl | Behaviour

The first run of trashctl will check if the users Trash directory exists. If it does not exist, the Trash directory will be created in the location defined above. 

Trashctl leverages subshells to operate based on user input, and is insulated against fatal errors occurring in the subshell. 

Trashctl can operate on any singular file, but currently lacks the ability to operate on directories. This functionality is actively being worked on.

## trashctl | Implementation Notes




