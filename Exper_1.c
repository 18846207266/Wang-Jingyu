#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_EXPR_LENGTH 100
// 读入表达式
char *readExpressionFromInput() {
    static char buffer[MAX_EXPR_LENGTH];
    printf("请输入一个以#为开头和结尾的中缀表达式: ");
    fgets(buffer, MAX_EXPR_LENGTH, stdin);  // 读取整行输入

    // 确保输入以 # 开始和结束 增强鲁棒性
    int len = strlen(buffer);
    if (buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';  // 移除末尾的换行符
        len--;
    }
    if (buffer[0] != '#' || buffer[len - 1] != '#') {
        fprintf(stderr, "开头和结尾必须为#!\n");
        return NULL;
    }

    // 移除首尾的 #
    memmove(buffer, buffer + 1, len - 2);  // 移除首#
    buffer[len - 2] = '\0';                // 移除尾#

    return buffer;
}

#define MAX_STACK_SIZE 100
// 定义栈
typedef struct {
    char items[MAX_STACK_SIZE];
    int top;
} Stack;
// 初始化一个栈
void initStack(Stack *s) { s->top = -1; }
// 判断栈满
int isFull(Stack *s) { return s->top == MAX_STACK_SIZE - 1; }
// 判断栈空
int isEmpty(Stack *s) { return s->top == -1; }
// 压栈
void push(Stack *s, char item) {
    if (isFull(s)) {
        fprintf(stderr, "Stack is full.\n");
        return;
    }
    s->items[++s->top] = item;
}
// 弹栈
char pop(Stack *s) {
    if (isEmpty(s)) {
        fprintf(stderr, "Stack is empty.\n");
        return '\0';
    }
    return s->items[s->top--];
}
// 查看栈顶元素
char peek(Stack *s) {
    if (isEmpty(s)) {
        return '\0';
    }
    return s->items[s->top];
}
// 优先级
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}
// ...其它代码保持不变...

void infixToPostfix(char *infix, char *postfix) {
    Stack s;
    initStack(&s);
    int j = 0;
    for (int i = 0; infix[i];) {
        char token = infix[i];
        if (isdigit(token)) {
            while (isdigit(infix[i])) {  // 处理多位数
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';  // 添加空格作为数字的分隔符
        } else if (token == '(') {
            push(&s, token);
            i++;
        } else if (token == ')') {
            while (!isEmpty(&s) && peek(&s) != '(') {
                postfix[j++] = pop(&s);
            }
            pop(&s);  // 从栈中移除'('
            i++;
        } else {  // 运算符
            while (!isEmpty(&s) && precedence(peek(&s)) >= precedence(token)) {
                postfix[j++] = pop(&s);
            }
            push(&s, token);
            i++;
        }
    }
    while (!isEmpty(&s)) {
        postfix[j++] = pop(&s);
    }
    postfix[j] = '\0';
}

int evaluatePostfix(char *postfix) {
    Stack s;
    initStack(&s);
    for (int i = 0; postfix[i];) {
        char token = postfix[i];
        if (isdigit(token)) {
            int number = 0;
            while (isdigit(postfix[i])) {  // 处理多位数
                number = number * 10 + (postfix[i++] - '0');
            }
            push(&s, number);
            i++;  // 跳过数字后的空格
        } else {
            int val2 = pop(&s);
            int val1 = pop(&s);
            switch (token) {
                case '+':
                    push(&s, val1 + val2);
                    break;
                case '-':
                    push(&s, val1 - val2);
                    break;
                case '*':
                    push(&s, val1 * val2);
                    break;
                case '/':
                    push(&s, val1 / val2);
                    break;
            }
            i++;
        }
    }
    return pop(&s);
}
int main() {
    char *infix = readExpressionFromInput();
    if (!infix) return 1;  // 如果读取失败，则退出

    printf("中缀表达式为: %s\n", infix);

    char postfix[MAX_EXPR_LENGTH];
    infixToPostfix(infix, postfix);
    printf("后缀表达式为: %s\n", postfix);

    int result = evaluatePostfix(postfix);
    printf("结果: %d\n", result);

    return 0;
}
