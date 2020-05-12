#include <stdio.h>
#include <ftw.h>
#include <unistd.h>
#include <sys/inotify.h>
#include "logger.h"

#define EVENT_LENGTH (1024 * ((sizeof(struct inotify_event)) + 16))

struct inotify_event *event;

int main(int argc, char** argv){
	int initNotify;
	int watchDir;
	int info;
	char buff[EVENT_LENGTH];
	if(argc < 2) {
        warnf("USAGE: ./monitor \"path\"\n");
        return -1;
    }
    infof("Reading path %s\n", argv[1]);
    if((initNotify = inotify_init()) == -1) {
        warnf("Error initializing inotify\n");
    }
    if((watchDir = inotify_add_watch(initNotify, argv[1], IN_ALL_EVENTS)) == -1){
        warnf("Error watching directory\n");
    }
    while(1) {
        info = read(initNotify, buff, EVENT_LENGTH);
        for (int i = 0; i < info; 1) {
            event = (struct inotify_event * ) &buff[i];
            if (event->mask & IN_ACCESS) {
                infof("File was accessed\n");
            } else if (event->mask & IN_ATTRIB) {
                infof("Metadata changed—for example, permissions\n");
            } else if (event->mask & IN_CLOSE_WRITE) {
                infof("File opened for writing was closed\n");
            } else if (event->mask & IN_CLOSE_NOWRITE) {
                infof("File or directory not opened for writing was closed\n");
            } else if (event->mask & IN_CREATE) {
                infof("File/directory created in watched directory\n");
            } else if (event->mask & IN_DELETE) {
                infof("ile/directory deleted from watched directory\n");
            } else if (event->mask & IN_DELETE_SELF) {
                infof("Watched file/directory was itself deleted\n");
            } else if (event->mask & IN_MODIFY) {
                infof("File was modified\n");
            } else if (event->mask & IN_MOVE_SELF) {
                infof("Watched file/directory was itself moved\n");
            } else if (event->mask & IN_MOVED_FROM) {
                infof("Generated for the directory containing the old filename when a file is renamed\n");
            } else if (event->mask & IN_MOVED_TO) {
                infof("Generated for the directory containing the new filename when a file is renamed\n");
            } else if (event->mask & IN_OPEN) {
                infof("File or directory was opened\n");
            }
            i += ( sizeof (struct inotify_event) ) + event->len;
        }
    }
    inotify_rm_watch(initNotify, watchDir);
    close(initNotify);
    return 0;
}