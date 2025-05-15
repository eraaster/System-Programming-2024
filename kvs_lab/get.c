#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kvs.h"

char* get(kvs_t* kvs, const char* key) {
    skiplist_node_t *current = kvs->header;

    // 최상위 레벨부터 시작하여 각 레벨을 내려가면서 탐색
    for (int i = kvs->level; i >= 0; i--) {
        while (current->forward[i] != NULL && strcmp(current->forward[i]->key, key) < 0) {
            current = current->forward[i];
        }
    }

    // 최하위 레벨에서 다음 노드를 확인
    current = current->forward[0];
    if (current != NULL && strcmp(current->key, key) == 0) {
        // 키가 존재하면 값을 반환
        char* value = (char*)malloc(strlen(current->value) + 1);
        if (!value) {
            printf("Failed to malloc\n");
            return NULL;
        }
        strcpy(value, current->value);
        return value;
    }

    // 키가 존재하지 않으면 NULL 반환
    return NULL;
}
