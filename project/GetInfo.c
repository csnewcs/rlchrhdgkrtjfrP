#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* infoName;
    char* info;
} Info;

int* getInfo();
Info GetCpuInfo() {
    const char* COMMAND = "lscpu";
    Info info ={"CPU", getInfo(COMMAND)};
    return info;
}
Info GetMemoryInfo() {
    const char* COMMAND = "lsmem";
    // return getInfo(COMMAND);
    Info info ={"Memory", getInfo(COMMAND)};
    return info;
}
Info GetPartitionInfo() {
    const char* COMMAND = "lsblk";
    // return getInfo(COMMAND);
    Info info ={"Partition", getInfo(COMMAND)};
    return info;
}
Info GetOsInfo() {
    const char* COMMAND = "lsb_release -a";
    Info info ={"OS", getInfo(COMMAND)};
    return info;
}
int* getInfo(char* command) {
    FILE* fp = popen(command, "r");
    if(fp == NULL) {
        return NULL;
    }
    char* buffer = calloc(5000, sizeof(char*));
    fread(buffer, 5000, 1, fp);
    pclose(fp);
    return buffer;
}