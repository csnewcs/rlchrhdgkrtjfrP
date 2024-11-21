
#ifndef GETINFO_DEFINEDED
#define GETINFO_DEFINEDED
typedef struct {
  char *infoName;
  char *info;
} Info;
#endif /* ifndef GETINFO_DEFINEDED */
char *getInfo();
char *getLine(char *str, char *startStr);
Info GetCpuInfo();
Info GetMemoryInfo();
Info GetGpuInfo();
