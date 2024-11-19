
typedef struct {
  char *infoName;
  char *info;
} Info;

char *getInfo();
char *getLine(char *str, char *startStr);
Info GetCpuInfo();
Info GetMemoryInfo();
Info GetGpuInfo();