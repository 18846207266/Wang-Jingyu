#include<iostream>
#include<stack>
#include <queue>
#include <string>
#include <cmath>
#include <vector>
using namespace std;
using ElemType = int;

typedef struct BiTNode{
	ElemType data;
	struct BiTNode *lchild,*rchild;
	
}BiTNode,*BiTree; 
//class BiTNode {
//public:
//    ElemType data; // 节点存储的数据
//    BiTNode* lchild; // 指向左子节点的指针
//    BiTNode* rchild; // 指向右子节点的指针
//
//    // 构造函数
//    BiTNode(ElemType val) : data(val), lchild(nullptr), rchild(nullptr) {}
//};

using BiTree = BiTNode*;

// 创造二叉树
vector<ElemType> CreatTree(BiTree *T, vector<ElemType> Q) {
    ElemType data;
    std::cout << "请输入根节点的值（输入0表示空树）: ";
    std::cin >> data;
    if (data == 0) {
        *T = NULL; // 创建空树
        return Q;
    }
    
    *T = new BiTNode; // 创建根节点
    (*T)->data = data;
    (*T)->lchild = (*T)->rchild = NULL;

    queue<BiTNode*> q; // 使用队列按层次顺序存储节点
    q.push(*T);
	Q.push_back(data);
    while (!q.empty()) {
        BiTNode *node = q.front();
        q.pop();
        
        cout << "请输入 " << node->data << " 的左子节点的值（输入0表示没有左子节点）: ";
        cin >> data;
        Q.push_back(data);
        if (data != 0) {
            node->lchild = new BiTNode;
            node->lchild->data = data;
            node->lchild->lchild = node->lchild->rchild = NULL;
            q.push(node->lchild);
            
        }

        cout << "请输入 " << node->data << " 的右子节点的值（输入0表示没有右子节点）: ";
        cin >> data;
        Q.push_back(data);
        if (data != 0) {
            node->rchild = new BiTNode;
            node->rchild->data = data;
            node->rchild->lchild = node->rchild->rchild = NULL;
            q.push(node->rchild);       
        }
    }
    ElemType s =Q.size();
	ElemType d= log2(s+1);//计算树的深度
	cout<<"画出该二叉树（0表示空）"<<endl; 
	int i = 0;
	int j =0 ;
	//cout<<"s="<<s<<" d="<<d-1<<"\n";
	for (i=0;i<s;){
		int now_p = log2(i+1);
		for(j=0;j<pow(2,now_p);j++){
			
			cout<<string(pow(2,d-now_p)-1,' ')<<Q[i]<<string(pow(2,d-now_p),' ');
			i++;
		}
		cout<<"\n";
	
		j=0;
		
	}
	return Q; 
}
// 访问 
void visit(BiTree T) {
    if (T != NULL) {
        std::cout << T->data << " ";
    }
}
//递归先序
void PreOrder1(BiTree T,queue<BiTNode*> &q){
	if(T!=NULL){
		visit(T);
		q.push(T); 
		PreOrder1(T->lchild,q);
		PreOrder1(T->rchild,q);
	}
} 
//递归中序
void InOrder1(BiTree T,queue<BiTNode*> &q){
	if(T!=NULL){
		InOrder1(T->lchild,q);
		visit(T);
		q.push(T);
		InOrder1(T->rchild,q);
	}
} 
//递归后序
void PostOrder1(BiTree T,queue<BiTNode*> &q){
	if(T!=NULL){
		PostOrder1(T->lchild,q);
		PostOrder1(T->rchild,q);
		visit(T);
		q.push(T);
	}
} 
//非递归先序
void PreOrder2(BiTree T,queue<BiTNode*> &q){
	stack<BiTree> S;
	BiTree p=T;//p是遍历指针 
	while(p||!S.empty()){
		if(p){
			visit(p);S.push(p);//访问并入栈 
			q.push(p);
			p=p->lchild;
		}
		else{
			p=S.top();
			S.pop();
			p=p->rchild;
		} 
	}
} 
//非递归中序
void InOrder2(BiTree T,queue<BiTNode*> &q){
	stack<BiTree> S;
	BiTree p=T;//p是遍历指针 
	while(p||!S.empty()){
		if(p){
			S.push(p);
			p=p->lchild;
		}
		else{
			visit(S.top());//访问并入栈 
			q.push(T);
			p=S.top();
			S.pop();
			p=p->rchild;
		} 
	}
} 
//非递归后序
void PostOrder2(BiTree T,queue<BiTNode*> &q){
	stack<BiTree> S;
	BiTree p =T;
	BiTree r = NULL;
	while(p||!S.empty()){
		if(p){
			S.push(p);
			p=p->lchild;
		} 
		else{
			p= S.top();
			if(p->rchild&&p->rchild!=r)
				p=p->rchild;
				else{
					p=S.top();
					S.pop();
					visit(p);
					q.push(T);
					r=p;//记录最近访问过的结果 
					p=NULL;
					
				}
		}
	}
} 
//打印储存队列 
void printQueue(queue<BiTNode*> &q) {
    while (!q.empty()) {
        BiTNode* elem = q.front();
        std::cout << elem->data << " ";
        q.pop(); // 移除队列前端的元素
    }
    std::cout << std::endl;
}

int main() {
    BiTree T;
    queue<BiTNode*> Record;
    vector<ElemType> Q;//层序动态数组 
	Q= CreatTree(&T,Q);
	for (size_t i = 0; i < Q.size(); ++i) {
	        cout<<Q[i] << " ";
	    } 
	cout<<"\n递归先序结果：\n"<<endl;
	  	PreOrder1(T,Record);
		cout<<"\n存储队列结果：\n"<<endl; 
		 	printQueue(Record); 
	cout<<"\n非递归先序结果：\n"<<endl; 
		PreOrder2(T,Record);
	cout<<"\n递归中序结果：\n"<<endl; 
		InOrder1(T,Record);
	cout<<"\n非递归中序结果：\n"<<endl; 
		InOrder2(T,Record);
	cout<<"\n递归后序结果：\n"<<endl; 
		PostOrder1(T,Record);					
	cout<<"\n递归后序结果：\n"<<endl; 
		PostOrder2(T,Record);

    return 0;
}
