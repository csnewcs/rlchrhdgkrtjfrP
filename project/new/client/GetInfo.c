#include "GetInfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *getLine(char *str, char *startStr);
Info GetCpuInfo() {
  const char *COMMAND = "cat ./cpuinfo";
  Info info = {"CPU", ""};
  char *cpuInfo = getInfo(COMMAND);
  char *cpuName = getLine(cpuInfo, "model name");
  for (int i = 0; i < strlen(cpuName); i++) {
    if (cpuName[i] == ':') {
      info.info = (cpuName + i + 2);
    }
  }
  return info;
}
Info GetMemoryInfo() {
  const char *COMMAND = "free --giga -h";
  Info info = {"Memory", ""};
  char *memInfo = getInfo(COMMAND);
  char *memSize = getLine(memInfo, "Mem:");
  int originSize = strlen(memSize);
  int i = 0;
  while (memSize[i] == ' ')
    i++;
  for (int j = 0; j < i; j++) {
    if (memSize[i + j] == ' ') {
      memSize[j - 1] = '\0';
      break;
    }
    memSize[j] = memSize[i + j];
  }
  info.info = memSize;
  return info;
}
Info GetGpuInfo() {
  const char *COMMAND = "lshw -C display -short"; // lshw -C display -short
  char* result = getInfo(COMMAND);
  char *line = strtok(result, "\n");
  line = strtok(NULL, "\n");
  line = strtok(NULL, "\n"); // 1줄(헤더), 2줄(구분자) 건너뜀
  // //0/100/1.1/0/0/0                    display     Navi 14 [Radeon RX 5500/5500M / Pro 5500M]
  char temp[100] = {0};
  int length = strlen(line);
  for(; *line != '[' && length > 0; length--) line++;
  if(length == 0) return (Info){"GPU", ""};
  line++;
  for(int i = 0; line[i] != ']'; i++) {
    temp[i] = line[i];
  }
  char *gpuName = calloc(strlen(temp), sizeof(char));
  strcpy(gpuName, temp);
  Info info = {"GPU", gpuName};
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
