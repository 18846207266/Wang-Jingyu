#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ERROR 0
#define OK 1
#define STACK_INT_SIZE 10 /*存储空间初始分配量*/
#define STACKINCREMENT 5  /*存储空间分配增量*/
#define M 50
#define MAX_EXPR_LENGTH 100
typedef char ElemType;    /*定义字符数据类型*/
typedef double ElemType2; /*定义运算数数据类型*/
/*字符栈*/
typedef struct {
    ElemType *base;//为了方便后续操作，加一个栈低的指针，正常没有也成 
    ElemType *top;
    int stacksize;
} SqStack;

/*运算数栈*/
typedef struct {
    ElemType2 *base;
    ElemType2 *top;
    int stacksize;
} NStack;

char *readFile(const char* filename);       /*文件读入*/ 
//char *readExpressionFromInput()          /*键盘input读入*/
int InitStack(SqStack *S);                /*构造空栈*/
int push(SqStack *S, ElemType e);         /*入栈*/
int Pop(SqStack *S, ElemType *e);         /*出栈*/
int StackEmpty(SqStack *s);               /*栈空判断*/
void in_to_post(ElemType *str, ElemType *p); /*中缀表达式转后缀表达式*/
double CAL_post(char *str);               /*计算后缀表达式*/

// 通过文件形式读入表达式
char *readFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "无法打开文件 %s\n", filename);
        return NULL;
    }

    static char buffer[MAX_EXPR_LENGTH];
    int i = 0;
    int foundHash = 0;
    while (1) {
        char ch = fgetc(file);
        if (ch == EOF) {
            break;
        } else if (ch == '#') {
            if (foundHash) {
                buffer[i++] = '\0';
                break;
            } else {
                foundHash = 1;
            }
        } else if (foundHash) {
            buffer[i++] = ch;
        }
    }

    fclose(file);
    return buffer;
}

//// 通过键盘直接读入表达式
//char *readExpressionFromInput() {
//    static char buffer[MAX_EXPR_LENGTH];
//    printf("请输入一个以#为开头和结尾的中缀表达式: ");
//    fgets(buffer, MAX_EXPR_LENGTH, stdin);  // 读取整行输入
//
//    // 确保输入以 # 开始和结束 增强鲁棒性
//    int len = strlen(buffer);
//    if (buffer[len - 1] == '\n') {
//        buffer[len - 1] = '\0';  // 移除末尾的换行符
//        len--;
//    }
//    if (buffer[0] != '#' || buffer[len - 1] != '#') {
//        fprintf(stderr, "开头和结尾必须为#!\n");
//        return NULL;
//    }
//
//    // 移除首尾的 #
//    memmove(buffer, buffer + 1, len - 2);  // 移除首#
//    buffer[len - 2] = '\0';                // 移除尾#
//
//    return buffer;
//}


/*字符栈初始化*/
int InitStack(SqStack *S) {
    S->base = (ElemType *)malloc(STACK_INT_SIZE * sizeof(ElemType));
    if (!S->base) return ERROR;  // 分配失败
    S->top = S->base;
    S->stacksize = STACK_INT_SIZE;
    return OK;
} /*InitStack*/

/*运算数栈初始化*/
int InitStack_N(NStack *S) {
    S->base = (ElemType2 *)malloc(STACK_INT_SIZE * sizeof(ElemType2));
    if (!S->base) return ERROR;
    S->top = S->base;
    S->stacksize = STACK_INT_SIZE;
    return OK;
}

/*字符栈入栈*/
int Push(SqStack *S, ElemType e) {
    // 判断栈满
    if (S->top - S->base >= S->stacksize) {
        S->base = (ElemType *)realloc(
            S->base, (S->stacksize + STACKINCREMENT) * sizeof(ElemType));
        if (NULL == S->base)  // 分配失败
            return ERROR;
        S->top = S->base + S->stacksize;
        S->stacksize = S->stacksize + STACKINCREMENT;
    }
    *S->top = e;
    S->top++;
    return OK;
}

/*运算数栈入栈*/
int Push_num(NStack *S, ElemType2 e) {
    if (S->top - S->base >= S->stacksize) {
        S->base = (ElemType2 *)realloc(
            S->base, (S->stacksize + STACKINCREMENT) * sizeof(ElemType2));
        if (NULL == S->base) return ERROR;
        S->top = S->base + S->stacksize;
        S->stacksize = S->stacksize + STACKINCREMENT;
    }
    *S->top = e;
    S->top++;
    return OK;
}

/*字符栈出栈*/
int Pop(SqStack *S, ElemType *e) {
    // 判断栈空
    if (S->top == S->base) return ERROR;
    S->top--;
    *e = *S->top;
    return OK;
} /*Pop*/

/*运算数栈出栈*/
int Pop_num(NStack *S, ElemType2 *e) {
    if (S->top == S->base) return ERROR;
    S->top--;
    *e = *S->top;
    return OK;
}

/*判断栈空*/
int StackEmpty(SqStack *s) {
    if (s->top == s->base) return OK;
    return ERROR;
} /*StackEmpty*/
//中缀表达式转后缀表达式 
void in_to_post(ElemType *str, ElemType *p) { /*infix to postfix*/
    // 初始化一个空栈
    SqStack s;
    InitStack(&s);
    ElemType e;

    size_t i;
    int j = 0;
    for (i = 0; i < strlen(str); i++)  // 遍历中缀表达式
    {
        if ('-' == str[i])  // 负数情况判断
        {
            // 负数在首位
            if (0 == i) p[j++] = str[i++];
            // 负数在后面'-'前面是'(',则一定是作为负数符号
            else if ('(' == str[i - 1])
                p[j++] = str[i++];
        }
//判断数or"." 
        while (isdigit(str[i]) || '.' == str[i]) {
            p[j++] = str[i++];
            if (!isdigit(str[i]) && '.' != str[i])
                p[j++] =
                    ' ';  // 一个数字完整输出后使用空格与其它运算符或数字分隔开
        }

        // 遇到左括号直接入栈
        if ('(' == str[i]) Push(&s, str[i]);

        // 遇到右括号直接出栈，直到左括号出栈(左括号不输出)
        if (')' == str[i]) {
            while ('(' != *(s.top - 1)) {
                Pop(&s, &e);
                p[j++] = e;
                p[j++] = ' ';
            }
            Pop(&s, &e);  // 左括号出栈但不输出
        }
//区分运算符优先级 
        // 遇到+或—
        // 1.栈空/栈顶为左括号:直接入栈
        // 2.否则一直出栈,直到栈空/栈顶为左括号,再入栈
        if ('+' == str[i] || '-' == str[i]) {
            while (!StackEmpty(&s) &&
                   '(' != *(s.top - 1))  // 栈非空 且 栈顶非左括号
            {
                Pop(&s, &e);
                p[j++] = e;
                p[j++] = ' ';
            }
            Push(&s, str[i]);
        }

        // 遇到*或/
        // 1.栈空/栈顶为左括号/栈顶操作符为+ or -:直接入栈
        // 2.否则一直出栈,直到满足1,再入栈
        if ('*' == str[i] || '/' == str[i] || '%' == str[i]) {
            while (!StackEmpty(&s) && '(' != *(s.top - 1) &&
                   '+' != *(s.top - 1) && '-' != *(s.top - 1)) {
                Pop(&s, &e);
                p[j++] = e;
                p[j++] = ' ';
            }
            Push(&s, str[i]);
        }
    }
    // 中缀表达式遍历完成,还需检查栈中是否有未输出字符
    // 判断栈空,非空则直接出栈并输出(左括号不用输出)
    while (!StackEmpty(&s)) {
        Pop(&s, &e);
        if ('(' != e) {
            p[j++] = e;
            p[j++] = ' ';
        }
    }
    p[--j] = '\0';
}
//计算后缀表达式 
double CAL_post(char *str) {
    ElemType2 e, a, b;
    char d[M];
    // 初始化一个运算数栈保存运算数
    NStack n;
    InitStack_N(&n);
    int i = 0;
    int j = 0;
    while (str[i])  // 遍历后缀表达式
    {
        switch (str[i]) {
            case '-':
                if (isdigit(str[i + 1]))  // 判断'-'是作为负数符号or运算符
                {
                    d[j++] = str[i++];  // 将负号加入运算数字符串
                    d[j] = '\0';
                    break;  // 注:这里的break只是跳出switch循环
                } else {
                    Pop_num(&n, &b);
                    Pop_num(&n, &a);
                    Push_num(&n, a - b);
                    i++;
                    break;
                }
            case '+':
                Pop_num(&n, &b);
                Pop_num(&n, &a);
                Push_num(&n, a + b);
                i++;
                break;
            case '*':
                Pop_num(&n, &b);
                Pop_num(&n, &a);
                Push_num(&n, a * b);
                i++;
                break;
            case '/':
                Pop_num(&n, &b);
                Pop_num(&n, &a);
                Push_num(&n, a / b);
                i++;
                break;
            case ' ':
                i++;
        }

        // 遇到运算数直接入栈(先转换double类型)
        // d保存后缀表达式中的字符串形式的运算数
        // 使用atof将字符串转换为double类型
        while (isdigit(str[i]) || '.' == str[i]) {
            d[j++] = str[i++];
            d[j] = '\0';
            if (' ' == str[i]) {
                e = atof(d);  // 此时分析出的就是完整的运算数
                Push_num(&n, e);
                i++;
                j = 0;
            }
        }
    }
    Pop_num(&n, &e);
    return e;
}
int main() {
    const char* filename = "expression.txt";  // 文件名
	    char* infix = readFile(filename);
	    if (!infix) return 1;  // 如果读取失败，则退出
	    printf("中缀表达式为: %s\n", infix);
	    char post[MAX_EXPR_LENGTH];
	    in_to_post(infix, post);
	    printf("对应的后缀表达式: %s\n", post);
	    printf("计算后缀表达式: %.2f\n", CAL_post(post));
	    return 0;
}
