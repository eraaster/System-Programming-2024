#include <stdlib.h>
#include "kvs.h"

// 노드를 해제하는 함수
void free_node(skiplist_node_t *node) {
    if (node->key) free((char*)node->key);  // strdup을 사용했을 경우를 대비해 free
    if (node->value) free((char*)node->value);
    free(node->forward);
    free(node);
}

int close(kvs_t* kvs) {
    if (!kvs) return -1;

    // 헤더부터 시작하여 모든 노드를 해제
    skiplist_node_t *current = kvs->header;
    while (current != NULL) {
        skiplist_node_t *next = current->forward[0];
        free_node(current);  // 현재 노드를 해제
        current = next;      // 다음 노드로 이동
    }

    // kvs_t 구조체 해제
    free(kvs);
    printf("Close: kvs has been successfully freed\n");
    
    return 0;
}
