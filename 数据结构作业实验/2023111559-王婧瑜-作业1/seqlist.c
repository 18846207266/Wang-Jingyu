#include <stdio.h>
#include <string.h>
#define Mix_size 100
#define OK 1
#define ERROR 0

typedef int ElemType;
typedef int Status;
typedef struct {
    ElemType data[Mix_size];
    int length;
} Slist;
// 打印数组
void Print_Data(Slist s) {
    for (int i = 0; i < s.length; i++) {
        printf("%d ", s.data[i]);
    }
    printf("\n");
}
// 1.1删除给定元素
Status Delete_Slist(Slist *s, int Data) {
    int i = 0;
    int flag = 1;
    // 检查线性表是否为空
    if (s->length == 0) return ERROR;

    for (i = 0; i < s->length; i++) {
        if (s->data[i] == Data) {
            flag = 0;
            for (int j = i; j < s->length; j++) {
                s->data[j] = s->data[j + 1];
            }
            s->length--;
            i--;  // 回退一步 这样可以删除数组内所有的Data
        }
    }
    if (flag == 1) return ERROR;
    return OK;
}
// 1.2删除指定位置的元素
Status Delete_Slist_index(Slist *s, int index) {
    int i = 0;
    int flag = 1;
    // 检查线性表是否为空
    if (s->length == 0) return ERROR;

    for (i = 0; i < s->length; i++) {
        if (i == index - 1) {  // 这次是输入index来删数据
            flag = 0;
            for (int j = i; j < s->length; j++) {
                s->data[j] = s->data[j + 1];
            }
            s->length--;
            break;
        }
    }

    if (flag == 1) return ERROR;

    return OK;
}
// 2.对于已经排好的线性表，删除所有重复元素
Status Delete_Slist_repeat(Slist *s) {
    int i = 0;
    int j = 0;
    int flag = 1;
    // 检查线性表是否为空
    if (s->length == 0) return ERROR;
    // 查找重复元素
    for (i = 0; i < s->length; i++) {
        j = i + 1;
        while (s->data[i] == s->data[j]) {
            flag = 0;
            for (int k = j; k < s->length; k++) {
                s->data[k] = s->data[k + 1];
            }

            s->length--;
        }
    }
    if (flag == 1) {
        printf("未找到重复元素\n");
        return ERROR;
    } else {
        printf("已删除重复元素\n");
    }
    return OK;
}
// 3.逆置
void Inversion_Slist(Slist *s) {
    int i = 0;
    int l = s->length;
    int temp;  // 用于交换的临时变量
    for (i = 0; i < l / 2; i++) {
        temp = s->data[i];
        s->data[i] = s->data[l - i - 1];
        s->data[l - i - 1] = temp;
    }
}
// 4.左移右移
void R_L_Move_Slist(Slist *s, int k) {
    // k>0,右移；k<0,左移
    int l = s->length;
    int temp[l];        // 用于储存结果
    k = k % l;          // 防止k大于数组长度
    if (k < 0) k += l;  // 将负数转换为正数，简化处理
    for (int i = 0; i < l; i++) {
        temp[(i + k) % l] = s->data[i];  // 计算每个元素移动后的新位置
    }
    for (int i = 0; i < l; i++) {
        s->data[i] = temp[i];  // 将临时数组的内容复制回原数组
    }
}
// 5.合并两个线性表
Slist Combination_Slist(Slist s, Slist t) {  // 默认为s在前面，t在后面
    int l = s.length;
    Slist new;
    for (int i = 0; i < l; i++) {
        new.data[i] = s.data[i];
    }
    for (int j = 0; j < t.length; j++) {
        new.data[l + j] = t.data[j];
    }
    new.length = l + t.length;
    return new;
}

int main() {
    Slist s1 = {{0, 1, 2, 3, 4, 5, 6}, 7};
    Slist s2 = {{7, 8, 9}, 3};
    Print_Data(s1);
    Print_Data(s2);
    // Print_Data(s1);
    // if (Delete_Slist(&s1, 3) == OK) {
    //     Print_Data(s1);
    //     printf("成功删除元素3\n");
    // } else {
    //     printf("未找到相应元素");
    // }
    // if (Delete_Slist_index(&s1, 8) == OK) {
    //     Print_Data(s1);
    //     printf("成功删除8号元素\n");
    // } else {
    //     printf("未找到相应元素");
    // }
    // if (Delete_Slist_repeat(&s1) == OK) Print_Data(s1);
    // Inversion_Slist(&s1);
    // R_L_Move_Slist(&s1, 9);
    Slist new = Combination_Slist(s1, s2);
    Print_Data(new);
    return 0;
}