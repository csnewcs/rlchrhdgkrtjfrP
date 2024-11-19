
// ================= 콘솔 제어 문자열과 매크로 =================
#define CURSOR_UP "\033[A"
#define CURSOR_DOWN "\033[B"
#define CURSOR_FRONT "\033[C"
#define CURSOR_BACK "\033[D"
#define COLOR_RED "\033[31m"
#define COLOR_RESET "\033[0m"
#define PRINT_WITH_COLOR(COLOR, STRING)                                        \
  printf("%s%s%s", COLOR, STRING, COLOR_RESET)
#define ERASE_CONSOLE() printf("\033[2J")
#define GET_CONSOLE() printf("\033[?1049h")
#define RELEASE_CONSOLE() printf("\033[?1049l")
#define CURSOR_MOVE(X, Y) printf("\033[%d;%dH", X, Y)
#define CURSOR_HIDE() printf("\033[?25l")
#define CURSOR_SHOW() printf("\033[?25h")
#define HIDE_INPUT_TEXT() printf("\033[?25l")
