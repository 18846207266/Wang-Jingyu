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
// 1.1删除给定元素
void Delete_Llist(NODE **head, int Data) {
    NODE *temp = *head;
    NODE *prev = NULL;
    int flag = 0;
    if (temp == NULL) printf("该链表为空\n");

    // 如果头节点本身就需要被删除
    if (temp != NULL && temp->data == Data) {
        *head = temp->next;  // 改变头指针
        free(temp);          // 释放旧的头节点
        flag = 1;
        return;
    }
    while (temp != NULL) {
        while (temp != NULL && temp->data != Data) {
            prev = temp;
            temp = temp->next;
        }
        if (temp == NULL) {
            if (flag == 0) printf("未找到该节点\n");
            return;
        }

        prev->next = temp->next;  // 删除该节点
        NODE *toDelete = temp;
        temp = temp->next;  // 更新临时节点,无需回退
        free(toDelete);     // 释放被删除节点的内存
        flag = 1;
    }
}
// 1.2删除指定位置的元素
void Delete_Llist_index(NODE **head, int index) {
    NODE *temp = *head;
    NODE *prev = NULL;
    int i = 0;

    if (temp == NULL) printf("该链表为空\n");
    if (index == 1) {
        *head = temp->next;
        free(temp);
        return;
    }
    for (i = 2; temp != NULL; i++) {
        prev = temp;
        temp = temp->next;
        if (i == index) {
            prev->next = temp->next;
            printf("You deleted the NO.%d node successfully.\n", index);
            free(temp);
            return;
        }
    }
    printf("该index超出链表长度。");
    return;
}
// 2.对于已经排好的线性表，删除所有重复元素
void Delete_Llist_repeat(NODE **head) {
    NODE *prev = *head;
    NODE *temp = NULL;
    while (prev != NULL) {
        for (temp = prev->next; temp != NULL;) {
            if (prev->data == temp->data) {
                NODE *ToDelete = temp;    // 临时保存重复节点
                prev->next = temp->next;  // 删除重复节点
                temp = temp->next;        // 向后移一个
                free(ToDelete);           // 释放内存
            } else
                break;
        }
        prev = prev->next;
    }
}
// 3.逆置
void Inversion_Llist(NODE **head) {
    NODE *temp = *head;
    NODE *prev = NULL;
    NODE *behind = *head;

    while (temp != NULL) {
        behind = temp->next;
        temp->next = prev;  // temp连到前一个的前面
        prev = temp;        // 往后移一个
        temp = behind;      // 往后移一个
    }
    *head = prev;  // 改变头结点位置
}
// 4.左移右移
void R_L_Move_Llist(NODE **head, int k) {
    int i = 0;  // k>0左移,k<0右移
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
// 5.合并两个线性表
NODE *Combination_Llist(NODE **head, NODE **Head) {
    NODE *new = NULL;
    NODE *temp = *head;
    while (temp != NULL) {
        insertNODE(&new, temp->data);
        temp = temp->next;
    }
    temp = *Head;
    while (temp != NULL) {
        insertNODE(&new, temp->data);
        temp = temp->next;
    }
    return new;
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
    NODE *Head = NULL;
    int k = 0;
    int i = 0;
    for (k = 0; k < 6; k++) {
        insertNODE(&head, k);
    }
    // insertNODE(&head, 5);
    // insertNODE(&head, 6);
    // insertNODE(&head, 6);
    // insertNODE(&head, 6);
    // insertNODE(&head, 7);
    for (i = 0; i < 3; i++) {
        insertNODE(&Head, i);
    }

    // insertNODE(&head, 3);
    Print_List(&head);
    // 1.2Delete_Llist(&head, 3);
    //  1.2_Delete_Llist_index(&head, 3);
    Delete_Llist_repeat(&head);
    // 3_Inversion_Llist(&head);
    // R_L_Move_Llist(&head, -2);
    // 5_NODE *new = Combination_Llist(&head, &Head);
    Print_List(&head);
    freeList(head);
    freeList(Head);
    return 0;
}