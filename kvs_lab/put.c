#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kvs.h"

#define MAX_LEVEL 16
#define P 0.5  // 레벨을 결정할 확률

// 무작위 레벨을 생성하는 함수
int random_level() {
    int level = 0;
    while ((rand() / (double)RAND_MAX) < P && level < MAX_LEVEL - 1) {
        level++;
    }
    return level;
}

int put(kvs_t* kvs, const char* key, const char* value) {
    printf("put: %s, %s\n", key, value);

    skiplist_node_t *update[MAX_LEVEL];
    skiplist_node_t *current = kvs->header;

    // 삽입할 위치를 찾습니다.
    for (int i = kvs->level; i >= 0; i--) {
        while (current->forward[i] != NULL && strcmp(current->forward[i]->key, key) < 0) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];

    // 키가 이미 존재하는 경우 값을 업데이트합니다.
    if (current != NULL && strcmp(current->key, key) == 0) {
        free(current->value);  // 기존 값을 해제합니다.
        current->value = strdup(value);  // 새로운 값으로 대체합니다.
        return 1;
    }

    // 키가 존재하지 않는 경우, 새로운 노드를 삽입합니다.
    int level = random_level();
    if (level > kvs->level) {
        for (int i = kvs->level + 1; i <= level; i++) {
            update[i] = kvs->header;
        }
        kvs->level = level;
    }

    // 새로운 노드를 생성합니다.
    skiplist_node_t *new_node = create_node(level, key, value);

    // 노드를 삽입합니다.
    for (int i = 0; i <= level; i++) {
        new_node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = new_node;
    }

    kvs->items++;
    return 0;
}
