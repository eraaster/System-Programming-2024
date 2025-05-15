#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kvs.h"

int main()
{
    kvs_t* kvs = open();

    if (!kvs) {
        printf("Failed to open kvs\n");
        return -1;
    }

    // query.dat 파일 읽기
    FILE *queryFile = fopen("query.dat", "r");
    FILE *answerFile = fopen("answer.dat", "w");

    if (!queryFile || !answerFile) {
        printf("Failed to open query.dat or answer.dat\n");
        close(kvs);
        if (queryFile) fclose(queryFile);
        if (answerFile) fclose(answerFile);
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), queryFile)) {
        // 줄에서 공백과 개행 문자 제거
        line[strcspn(line, "\n")] = 0;

        // 명령어와 인자를 구분하기 위해 토큰화
        char *command = strtok(line, ",");
        char *key = strtok(NULL, ",");
        char *value = strtok(NULL, ",");

        if (strcmp(command, "set") == 0 && key && value) {
            // put 명령어: key-value 쌍을 kvs에 삽입
            put(kvs, key, value);
        } else if (strcmp(command, "get") == 0 && key) {
            // get 명령어: key에 해당하는 값을 검색
            char *result = get(kvs, key);
            if (result) {
                fprintf(answerFile, "%s\n", result);
            } else {
                fprintf(answerFile, "-1\n");  // key가 없는 경우 -1 출력
            }
        }
    }

    // 파일 및 kvs 닫기
    fclose(queryFile);
    fclose(answerFile);
    close(kvs);

    return 0;
}