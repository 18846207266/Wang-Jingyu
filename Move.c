#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OK 1
#define ERROR 0
typedef int ElemType;
typedef int Status;
typedef struct NODE {  // 若自身调用，则需要用结构体的显示名称
    ElemType data;
    struct NODE *next;
} NODE;
// 创建链表
NODE *createNODE(int data) {
    NODE *newNODE = (NODE *)malloc(sizeof(NODE));
    if (newNODE == NULL) {
        printf("Error: memory allocation failed\n");
        exit(1);
    }
    newNODE->data = data;
    newNODE->next = NULL;
    return newNODE;
}
// 插入节点
void insertNODE(NODE **head, int data) {
    NODE *newNODE = createNODE(data);
    if (*head == NULL) {
        *head = newNODE;
    } else {
        NODE *temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNODE;
    }
}
// 打印链表
void Print_List(NODE **head) {
    NODE *temp = *head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}
void R_L_Move_Llist(NODE **head, int k) {
    int i = 0;  // k>0左移
    int l = 0;
    if (*head == NULL || (*head)->next == NULL) {
        return;  // 如果链表为空或只有一个元素，则不需要移动
    }
    NODE *temp = *head;
    NODE *run = *head;
    NODE *prev = NULL;
    while (run != NULL) {
        prev = run;
        run = run->next;
        l++;  // 获得链表长度
    }
    prev->next = temp;  // 成环（循环链表）注意不能写成run=temp
                        // 因为run已经从链表中掉下去了
    for (i = 0; i < (l + k) % l; i++) {
        prev = temp;
        temp = temp->next;
    }
    *head = temp;       // 改变头结点位置
    prev->next = NULL;  // 断开
}
// 释放整个链表的空间
void freeList(NODE *head) {
    NODE *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
int main() {
    NODE *head = NULL;

    int k = 0;
    int i = 0;
    for (k = 0; k < 6; k++) {
        insertNODE(&head, k);
    }

    Print_List(&head);

    R_L_Move_Llist(&head, -1);

    Print_List(&head);
    freeList(head);

    return 0;
}