#include <stdio.h>
#include <stdlib.h>
#include "kvs.h"

#define MAX_LEVEL 16  // Skip List의 최대 레벨 정의

// Skip List의 노드 초기화 함수
skiplist_node_t* create_node(int level, const char *key, const char *value) {
    skiplist_node_t *node = (skiplist_node_t*) malloc(sizeof(skiplist_node_t));
    node->key = key ? strdup(key) : NULL;
    node->value = value ? strdup(value) : NULL;
    node->forward = (skiplist_node_t**) malloc(sizeof(skiplist_node_t*) * (level + 1));

    for (int i = 0; i <= level; i++) {
        node->forward[i] = NULL;
    }
    return node;
}

// Skip List 초기화 함수
void init_skiplist(kvs_t *kvs) {
    kvs->level = 0;
    kvs->header = create_node(MAX_LEVEL, NULL, NULL);  // 헤더 노드를 초기화
    kvs->items = 0;
}

// open 함수 구현
kvs_t* open() {
    kvs_t* kvs = (kvs_t*) malloc(sizeof(kvs_t));

    if (kvs) {
        init_skiplist(kvs);  // Skip List 초기화
        printf("Open: kvs has %d items\n", kvs->items);
    }

    return kvs;
}
