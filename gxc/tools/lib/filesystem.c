#include <stdio.h>

#include "filesystem.h"

int(*fs_load_file)(const char *path, void* address, unsigned int* size) = SELF_FS_LOAD_FILE;
void(*fs_mount)(const char *partition, const char *type, const char *path) = NULL;
void(*fs_unmount)(const char *path) = SELF_FS_UNMOUNT;

void* find_fs_mount() {
	return find_function("fs_mount", TARGET_BASEADDR, TARGET_BASEADDR);
}

void* find_fs_unmount() {
	return 0;
}

void* find_fs_load_file() {
	return 0;
}

int fs_init() {
	fs_mount = find_fs_mount();
	if(fs_mount == NULL) {
		puts("Unable to find fs_mount\n");
	} else {
		printf("Found fs_mount at 0x%x\n", fs_mount);
	}

	if(fs_unmount == NULL) {
		puts("Unable to find fs_unmount\n");
	} else {
		printf("Found fs_unmount at 0x%x\n", fs_unmount);
	}

	if(fs_load_file == NULL) {
		puts("Unable to find fs_load_file\n");
	} else {
		printf("Found fs_load_file at 0x%x\n", fs_load_file);
	}

	if(fs_mount && fs_unmount && fs_load_file) {
		cmd_add("fs", &fs_cmd, "perform operations on the gxc extraction");
	}

	return 0;
}

int fs_cmd(int argc, CmdArg* argv) {
	char* path = NULL;
	char* action = NULL;
	char* solution = NULL;
	char* device = NULL;
	void* address = NULL;
	unsigned int* size = 0;

	if(argc < 3) {
		puts("usage: fs <mount/unmount/load> [options]\n");
		puts("  mount <device> <path> \tmount device to path\n");
		puts("  unmount <path>        \tunmount specified path\n");
		puts("  load <path> <address> \tload file from path to address\n\n");
		return 0;
	}

	action = argv[1].string;
	if(argc == 3) {
		if(strcmp(action, "umount")) {
				path = argv[2].string;
				fs_unmount(path);
		}

	} else if(argc == 4) {
		if(!strcmp(action, "mount")) {
			path = argv[3].string;
			device = argv[2].string;
			fs_mount(device, "hfs", path);

		} else if(!strcmp(action, "load")) {
			path = argv[1].string;
			address = (void*) argv[3].uinteger;
			fs_load_file(path, address, size);
		}
	}

	return 0;
}
