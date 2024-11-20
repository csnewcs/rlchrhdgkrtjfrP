#include <sys/ioctl.h>
#include <termios.h>

// ================= 콘솔 제어 문자열과 매크로 =================
#define CURSOR_UP "\033[A"
#define CURSOR_DOWN "\033[B"
#define CURSOR_FRONT "\033[C"
#define CURSOR_BACK "\033[D"
#define COLOR_RED "\033[31m"
#define COLOR_RESET "\033[0m"
#define RESET_ALL "\033[0m"
#define BOLD "\033[1m"
#define NOT_BOLD "\033[23m"
#define UNDERLINE "\033[4m"
#define NOT_UNDERLINE "\033[24m"
#define PRINT_WITH_COLOR(COLOR, STRING)                                        \
  printf("%s%s%s", COLOR, STRING, COLOR_RESET)
#define ERASE_CONSOLE() printf("\033[2J")
#define GET_CONSOLE() printf("\033[?1049h")
#define RELEASE_CONSOLE() printf("\033[?1049l")
#define CURSOR_MOVE(X, Y) printf("\033[%d;%dH", X, Y)
#define CURSOR_HIDE() printf("\033[?25l")
#define CURSOR_SHOW() printf("\033[?25h")
#define HIDE_INPUT_TEXT() printf("\033[?25l")

extern struct winsize w;
extern struct termios term;
extern int step; // 0: 스펙 확인, 1: 게임 리스트 가져오기 완료 후 선택, 2: 게임
                 // 상태 확인, 3: 끝
extern int selectedGame;
extern int gameCount;
// ================= 기능 함수 헤더 =================
void initConsole();
char *strMultiply(char *str, int num);
int printSpec(char *cpuName, char *gpuName,
              char *memSize); // 0: Succeed, 1: Failed
void *printThread(void *running);
void setStatusStr();
