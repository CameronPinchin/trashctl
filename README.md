
# **trashctl**
**Author: Cameron Pinchin | <cwpinchin@outlook.com>**

### trashctl | description

There is a Bash-based utility called trash-cli that provides a utility to manage your *trashbin* on Linux systems. This aims to emulate that functionality, written in C. There are other C-based versions of this, so this is more of a learning project.

Trashctl will check for the presence of a *Trash* directory located at the following location:


> */home/${USER}/.local/share/Trash/files/*

If errno is set to [**ENOENT**] the directory and its necessary parent directories will be created, handled by function:

> static int trash_dir_create_dir_p(struct environment_info* env);

If successful, the following environment variable will be set: 

> *$TRASH_DIR = /home/${USER}/.local/share/Trash/files/*

The *files* directory will have its permission bits set to: **755**.

### trashctl-* | -put,-list,-restore,-empty,-pick

