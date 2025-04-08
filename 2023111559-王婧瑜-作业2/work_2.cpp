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
//    ElemType data; // �ڵ�洢������
//    BiTNode* lchild; // ָ�����ӽڵ��ָ��
//    BiTNode* rchild; // ָ�����ӽڵ��ָ��
//
//    // ���캯��
//    BiTNode(ElemType val) : data(val), lchild(nullptr), rchild(nullptr) {}
//};

using BiTree = BiTNode*;

// ���������
vector<ElemType> CreatTree(BiTree *T, vector<ElemType> Q) {
    ElemType data;
    std::cout << "��������ڵ��ֵ������0��ʾ������: ";
    std::cin >> data;
    if (data == 0) {
        *T = NULL; // ��������
        return Q;
    }
    
    *T = new BiTNode; // �������ڵ�
    (*T)->data = data;
    (*T)->lchild = (*T)->rchild = NULL;

    queue<BiTNode*> q; // ʹ�ö��а����˳��洢�ڵ�
    q.push(*T);
	Q.push_back(data);
    while (!q.empty()) {
        BiTNode *node = q.front();
        q.pop();
        
        cout << "������ " << node->data << " �����ӽڵ��ֵ������0��ʾû�����ӽڵ㣩: ";
        cin >> data;
        Q.push_back(data);
        if (data != 0) {
            node->lchild = new BiTNode;
            node->lchild->data = data;
            node->lchild->lchild = node->lchild->rchild = NULL;
            q.push(node->lchild);
            
        }

        cout << "������ " << node->data << " �����ӽڵ��ֵ������0��ʾû�����ӽڵ㣩: ";
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
	ElemType d= log2(s+1);//�����������
	cout<<"�����ö�������0��ʾ�գ�"<<endl; 
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
// ���� 
void visit(BiTree T) {
    if (T != NULL) {
        std::cout << T->data << " ";
    }
}
//�ݹ�����
void PreOrder1(BiTree T,queue<BiTNode*> &q){
	if(T!=NULL){
		visit(T);
		q.push(T); 
		PreOrder1(T->lchild,q);
		PreOrder1(T->rchild,q);
	}
} 
//�ݹ�����
void InOrder1(BiTree T,queue<BiTNode*> &q){
	if(T!=NULL){
		InOrder1(T->lchild,q);
		visit(T);
		q.push(T);
		InOrder1(T->rchild,q);
	}
} 
//�ݹ����
void PostOrder1(BiTree T,queue<BiTNode*> &q){
	if(T!=NULL){
		PostOrder1(T->lchild,q);
		PostOrder1(T->rchild,q);
		visit(T);
		q.push(T);
	}
} 
//�ǵݹ�����
void PreOrder2(BiTree T,queue<BiTNode*> &q){
	stack<BiTree> S;
	BiTree p=T;//p�Ǳ���ָ�� 
	while(p||!S.empty()){
		if(p){
			visit(p);S.push(p);//���ʲ���ջ 
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
//�ǵݹ�����
void InOrder2(BiTree T,queue<BiTNode*> &q){
	stack<BiTree> S;
	BiTree p=T;//p�Ǳ���ָ�� 
	while(p||!S.empty()){
		if(p){
			S.push(p);
			p=p->lchild;
		}
		else{
			visit(S.top());//���ʲ���ջ 
			q.push(T);
			p=S.top();
			S.pop();
			p=p->rchild;
		} 
	}
} 
//�ǵݹ����
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
					r=p;//��¼������ʹ��Ľ�� 
					p=NULL;
					
				}
		}
	}
} 
//��ӡ������� 
void printQueue(queue<BiTNode*> &q) {
    while (!q.empty()) {
        BiTNode* elem = q.front();
        std::cout << elem->data << " ";
        q.pop(); // �Ƴ�����ǰ�˵�Ԫ��
    }
    std::cout << std::endl;
}

int main() {
    BiTree T;
    queue<BiTNode*> Record;
    vector<ElemType> Q;//����̬���� 
	Q= CreatTree(&T,Q);
	for (size_t i = 0; i < Q.size(); ++i) {
	        cout<<Q[i] << " ";
	    } 
	cout<<"\n�ݹ���������\n"<<endl;
	  	PreOrder1(T,Record);
		cout<<"\n�洢���н����\n"<<endl; 
		 	printQueue(Record); 
	cout<<"\n�ǵݹ���������\n"<<endl; 
		PreOrder2(T,Record);
	cout<<"\n�ݹ���������\n"<<endl; 
		InOrder1(T,Record);
	cout<<"\n�ǵݹ���������\n"<<endl; 
		InOrder2(T,Record);
	cout<<"\n�ݹ��������\n"<<endl; 
		PostOrder1(T,Record);					
	cout<<"\n�ݹ��������\n"<<endl; 
		PostOrder2(T,Record);

    return 0;
}
