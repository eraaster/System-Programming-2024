#ifndef KVS_H
#define KVS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEVEL 16
#define P 0.5  // 레벨을 결정할 확률

// Skip List 노드 구조체
struct skiplist_node {
    char *key;
    char *value;
    struct skiplist_node **forward;  // 레벨별 다음 노드를 가리키는 포인터 배열
};
typedef struct skiplist_node skiplist_node_t;

// Skip List 구조체
struct kvs {
    skiplist_node_t *header;  // Skip List의 헤더 노드
    int level;                // Skip List의 현재 레벨
    int items;                // 저장된 데이터 수
};
typedef struct kvs kvs_t;

// 함수 선언
kvs_t* open();                          // Skip List 초기화
int close(kvs_t* kvs);                  // 모든 메모리 해제
int put(kvs_t* kvs, const char* key, const char* value);  // 데이터 삽입, 실패 시 -1 반환
char* get(kvs_t* kvs, const char* key); // 데이터 검색, 없으면 NULL 반환

// 노드 생성 함수 선언 (put.c에서 사용할 수 있도록 선언)
skiplist_node_t* create_node(int level, const char *key, const char *value);

#endif // KVS_H
