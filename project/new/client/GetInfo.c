#include "GetInfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *getLine(char *str, char *startStr);
Info GetCpuInfo() {
  const char *COMMAND = "cat ./cpuinfo";
  Info info = {"CPU", ""};
  char* cpuInfo = getInfo(COMMAND);
  char* cpuName = getLine(cpuInfo, "model name");
  for(int i = 0; i < strlen(cpuName); i++) {
    if(cpuName[i] == ':') {
      info.info = (cpuName + i + 2);
    }
  }
  return info;
}
Info GetMemoryInfo() {
  const char *COMMAND = "free --giga -h";
  Info info = {"Memory", ""};
  char* memInfo = getInfo(COMMAND);
  char* memSize = getLine(memInfo, "Mem:");
  int originSize = strlen(memSize);
  int i = 0;
  while (memSize[i] == ' ') i++;
  for(int j = 0; j < i; j++) {
    if(memSize[i + j] == ' ') {
      memSize[j-1] = '\0';
      break;
    }
    memSize[j] = memSize[i + j];
  }
  info.info = memSize;
  return info;
}
Info GetGpuInfo() {
  const char *COMMAND = "cat ./lshw";
  Info info = {"GPU", getInfo(COMMAND)};
  return info;
}
char *getInfo(char *command) {
  FILE *fp = popen(command, "r");
  if (fp == NULL) {
    return NULL;
  }
  char *buffer = calloc(5000, sizeof(char *));
  fread(buffer, 5000, 1, fp);
  pclose(fp);
  return buffer;
}
char *getLine(char *str, char *startStr) {
  char *line = strtok(str, "\n");
  for (; line != NULL; line = strtok(NULL, "\n")) {
    if (!strncmp(line, startStr, strlen(startStr))) {
      return (line + strlen(startStr));
    }
  }
  return "";
}
