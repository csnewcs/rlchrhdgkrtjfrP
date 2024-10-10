#include <stdio.h>
#include <stdarg.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include "GetInfo.c"

char* makeHtml(int, ...);
char* replace(char*, char*, char*);

int main() {
    printf("Dependencies: xdg-open, lscpu, lsblk, lsmem, lsb_release\n");
    printf("Generating system information...\n");

    Info cpuInfo = GetCpuInfo();
    Info memoryInfo = GetMemoryInfo();
    Info partitionInfo = GetPartitionInfo();
    Info osInfo = GetOsInfo();
    char* html = makeHtml(4, cpuInfo, memoryInfo, partitionInfo, osInfo);
    // fopen("index.html", "w");
    FILE* fp = fopen("index.html", "w");
    fprintf(fp, "%s", html);
    fclose(fp);
    printf("Saved to index.html\n");
    execlp("xdg-open", "xdg-open", "index.html", NULL);
    return 0;
}

char* makeHtml(int argCount, ...) {
    va_list args;
    va_start(args, argCount);
    char* html = strdup("<html>\n \
        <head>\n \
            <title>System Information</title>\n \
            <script> \
                let radioButtonOnClick = () => { \
                    let tabs = document.getElementsByClassName(\"tab\"); \
                    let tabContents = document.getElementsByClassName(\"tab_content\"); \
                    for(let i = 0; i < tabs.length; i++) { \
                        if(tabs[i].checked) { \
                            tabContents[i].style.display = \"block\"; \
                        } else { \
                            tabContents[i].style.display = \"none\"; \
                        } \
                    } \
                }\
            </script> \
        </head>\n \
        <body>\n \
        <h1>System Information</h1>\n \
        <div id=\"tab_select\">\n");
    char** infos = calloc(argCount, sizeof(char*));
    char* tabFormat = "<input type=\"radio\" onclick=\"radioButtonOnClick()\" class=\"tab\" name=\"tab\" id=\"%s\" />\n<label class=\"tab_label\" for=\"%s\">%s</label>\n";
    char* tab = calloc(1000, sizeof(char));
    for(int i = 0; i < argCount; i++) {
        Info info = va_arg(args, Info);
        info.info = replace(info.info, "\n", "<br>");
        sprintf(tab, tabFormat, info.infoName, info.infoName, info.infoName);
        html = (char*)realloc(html, (strlen(html) * sizeof(char)) + (strlen(tab) * sizeof(char)));
        strcat(html, tab);

        infos[i] = calloc(10000, sizeof(char));
        sprintf(infos[i], "<div id=\"%s\" class=\"tab_content\" style=\"display: none;\">\n<h1>%s</h1>\n<p>%s</p>\n</div>", info.infoName, info.infoName, info.info);
    }
    va_end(args);
    html = (char*)realloc(html, (strlen(html) * sizeof(char)) + 100);
    strcat(html, "</div>\n");
    for(int i = 0; i < argCount; i++) {
        html = (char*)realloc(html, sizeof(char) * (strlen(html) + strlen(infos[i])));
        strcat(html, infos[i]);
    }
    html = (char*)realloc(html, sizeof(char) * (strlen(html) + 100));
    strcat(html, "</div>\n</body>\n</html>");
    return html;
}

char* replace(char* str, char* oldStr, char* newStr) {
    char* result;
    int i, count = 0; // i: 반복 겸 length, count: oldStr 개수
    int newStrLen = strlen(newStr);
    int oldStrLen = strlen(oldStr);
    for(i = 0; str[i] != '\0'; i++) { // 문자열 크기 조절
        if(strstr(&str[i], oldStr) == &str[i]) { //strstr: indexOf와 유사, 찾으면 포인터 위치 반환, 못 찾으면 NULL 반환
            count++;
            i += oldStrLen - 1; //중복된 위치 검사 방지
        }
    }
    result = (char*)malloc(i + count * (newStrLen - oldStrLen) + 1);
    i = 0;
    while(*str) {
        if(strstr(str, oldStr) == str) {
            strcpy(&result[i], newStr);
            i += newStrLen;
            str += oldStrLen;
        } else {
            result[i++] = *str++;
        }
    }
    result[i] = '\0';
    return result;
}