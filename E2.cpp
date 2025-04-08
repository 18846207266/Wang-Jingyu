#include <string.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include<bitset>
#define MaxSize 18880

using namespace std; 

typedef struct Huffmantrue {
    int parent;
    int l_child;
    int r_child;
    int weight;
    char data;
} HTNode, *HuffmanTree;
typedef struct HCode {
    char data;
    char* str;
}* HuffmanCode;
/*整个字符串的字符构成*/
typedef struct NumCount {
    char c[MaxSize];
    int num[MaxSize];
    int length = 0;
} NumCount;
int Read_Sort_File(std::ifstream& file, NumCount* paraCnt);//读取统计字符 
bool cmp(const std::pair<char, int>& a, const std::pair<char, int>& b);//定义比较函数，用于按值排序
void CreateHuffmanTree(HuffmanTree &HT,int length,NumCount cntarray);  // 创建哈夫曼树 
void select(HuffmanTree HT,int top,int *s1,int *s2);  // 选择权重最小的两个节点 
void ReadData(char *source);  // 读入文件 
void CreateHuffmanCode(HuffmanTree HT,HuffmanCode &HC,int length);  // 创建哈夫曼编码 
void Encode(char *data,HuffmanCode HC,int length);  // 将读入的文件编码，写到txt文件
void compressBinaryFile(const std::string &inputFilename, const std::string &outputFilename);//压缩 
void decompressBinaryFile(const std::string &inputFilename, const std::string &outputFilename);//解压 
void Decode(HuffmanTree HT,int length);  //读入编码文件，解码 
double Cal_Compression_Ratio(std::ifstream& file_1, std::ifstream& file_2);//计算压缩率 

// 定义比较函数，用于按值排序
bool cmp(const std::pair<char, int>& a, const std::pair<char, int>& b) {
    return a.second < b.second;
}

int Read_Sort_File(std::ifstream& file, NumCount* paraCnt) {
    if (!file.is_open()) {
        std::cout << "Unable to open file\n";
        return 1;
    }

    std::map<char, int>
           counts;  // 键值对，字符为“键”，其在文章中的出现次数为值；
    char c;
    while (file.get(c)) {
        if (std::isalpha(c) || std::ispunct(c) || std::isspace(c)||std::isdigit(c)) {
            // 检查是否为字母或标点符号
            counts[c]++;
        }
    }

//    for (auto const& pair : counts) {
//        std::cout << pair.first << ": " << pair.second << '\n';
//    }
    // 创建一个 vector，存储 counts 中的元素
     std::vector<std::pair<char, int>> vec(counts.begin(), counts.end());

    // 使用自定义的比较函数，对 vector 进行排序
     std::sort(vec.begin(), vec.end(), cmp);

     // 打印排序后的键值对
     for (const auto& pair : vec) {
         std::cout << pair.first << ": " << pair.second << std::endl;
     }
    int i = 0;
    for (const auto& pair : counts) {
        paraCnt->c[i] = pair.first;
        paraCnt->num[i] = pair.second;
        i++;
    }
    paraCnt->length = i;
    return 0;
}
void select(HuffmanTree HT, int top, int* s1, int* s2) {
    int min = INT_MAX;
    for (int i = 1; i <= top; ++i)  // 选择没有双亲的节点中，权重最小的节点
    {
        if (HT[i].weight < min && HT[i].parent == 0) {
            min = HT[i].weight;
            *s1 = i;
        }
    }

    min = INT_MAX;
    for (int i = 1; i <= top; ++i)  // 选择没有双亲的节点中，权重次小的节点
    {
        if (HT[i].weight < min && i != *s1 && HT[i].parent == 0) {
            min = HT[i].weight;
            *s2 = i;
        }
    }
}
void CreateHuffmanTree(HuffmanTree& HT, int length, NumCount cntarray) {
    if (length <= 1) printf("ERROR!!!\r\n");

    int s1, s2;
    int m = length * 2 - 1;  // 没有度为1的节点，则总结点是2*叶子节点数-1个
    HT = new HTNode[m + 1];
    for (int i = 1; i <= m; ++i)  // 初始化
    {
        HT[i].parent = 0;
        HT[i].l_child = 0;
        HT[i].r_child = 0;
    }

    for (int i = 1; i <= length; ++i) {
        HT[i].data = cntarray.c[i];
        HT[i].weight = cntarray.num[i];
    }

    for (int i = length + 1; i <= m; ++i) {
        select(HT, i - 1, &s1, &s2);  // 从前面的范围里选择权重最小的两个节点
        HT[s1].parent = i;
        HT[s2].parent = i;
        HT[i].l_child = s1;
        HT[i].r_child = s2;
        HT[i].weight = HT[s1].weight + HT[s2].weight;  // 得到一个新节点
    }
}

void CreateHuffmanCode(HuffmanTree HT, HuffmanCode& HC, int length) {
    HC = new HCode[length + 1];
    char* cd = new char[length];  // 存储编码的临时空间
    cd[length - 1] = '\0';        // 方便之后调用strcpy函数
    int c, f, start;
    for (int i = 1; i <= length; ++i) {
        start =
            length -
            1;  // start表示编码在临时空间内的起始下标，由于是从叶子节点回溯，所以是从最后开始
        c = i;
        f = HT[c].parent;
        while (f != 0) {
            --start;  // 由于是回溯，所以从临时空间的最后往回计
            if (HT[f].l_child == c)
                cd[start] = '0';
            else
                cd[start] = '1';
            c = f;
            f = HT[c].parent;
        }
        HC[i].str =
            new char[length -
                     start];  // 最后，实际使用的编码空间大小是length-start
        HC[i].data = HT[i].data;
        strcpy(HC[i].str, &cd[start]);  // 从实际起始地址开始，拷贝到编码结构中
    }
    delete[] cd;
}
void ReadData(char *source)
{
	//打开文件读入数据 
	std::ifstream infile;
	infile.open("Huffman coding.txt");
	std::cout<<"Reading..."<<std::endl;
	std::cout<<"the  file is:"<<std::endl;
	std::string line;
	    std::string total;
	    while (std::getline(infile, line)) {
	        total += line + "\n";
	    }
	
	    strncpy(source, total.c_str(), MaxSize - 1);
	    source[MaxSize - 1] = '\0'; // Ensure null-terminated
	
	    //std::cout << source << std::endl;
	
	    infile.close();
	    std::cout << std::endl;
}
void Encode(char* data, HuffmanCode HC, int length) {
    std::ofstream outfile;
    outfile.open("code.txt");
    std::cout<<length<<std::endl;
    for (size_t i = 0; i < strlen(data);
         ++i)  // 依次读入数据，查找对应的编码，写入编码文件
    {   
    //std::cout<<data[i]<<std::endl;
        for (int j = 1; j <= length; ++j) {
            if (data[i] == HC[j].data) {
                outfile << HC[j].str;
            }
        }
    }
    outfile.close();
    std::cout << "the code txt has been written" << std::endl;
    std::cout << std::endl;
}
void Encode_table(char* data, HuffmanCode HC, int length) {
    std::ofstream outfile;
    outfile.open("code_table.txt");
    for (size_t i = 0; i < strlen(data);
         ++i)  // 依次读入数据，查找对应的编码，写入编码文件
    {
//    	 if (data[i] == '\n') {
//		        // 如果当前字符是一个换行符，那么使用一个特殊的编码
//		        outfile << "\n";
//		    } 
        for (int j = 1; j <= length; ++j) {
            if (data[i] == HC[j].data) {
            	outfile <<HC[j].data<<":"<< HC[j].str<<"\n";
            }
        }
    }
    outfile.close();
    std::cout << "the code_table.txt has been written" << std::endl;
    std::cout << std::endl;
}


void Decode(HuffmanTree HT,int length)
   {
   	char codetxt[MaxSize*length];
   	ifstream infile;
   	infile.open("decompressed.txt");
   	infile.getline(codetxt,MaxSize*length);
   	infile.close();
   	
   	ofstream outfile;
      	outfile.open("out.txt");
   	
   	int root = 2*length-1;  // 从根节点开始遍历 
   	for(size_t i = 0;i < strlen(codetxt);++i)
   	{
   		if(codetxt[i] == '0') root = HT[root].l_child;  //为0表示向左遍历 
   		else if(codetxt[i] == '1') root = HT[root].r_child; //为1表示向右遍历 
   		if(HT[root].l_child == 0 && HT[root].r_child == 0)  // 如果已经是叶子节点，输出到输出文件中，然后重新回到根节点 
   		{
   			outfile<<HT[root].data;
   			root = 2*length-1;
   		}
   	}
   	outfile.close();
   	std::cout<<"the out txt has been written"<<std::endl;
   	
   	std::cout<<std::endl;
   }
   
void compressBinaryFile(const std::string &inputFilename, const std::string &outputFilename) {
    std::ifstream inputFile(inputFilename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return;
    }

    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file!" << std::endl;
        return;
    }

    std::string buffer;
    char ch;
    while (inputFile.get(ch)) {
        buffer += ch;
        if (buffer.size() == 8) {
            std::bitset<8> bits(buffer);
            char byte = static_cast<char>(bits.to_ulong());
            outputFile.write(&byte, sizeof(char));
            buffer.clear();
        }
    }

    // 如果文件大小不是8的倍数，处理最后一组不足8位的数据
    if (!buffer.empty()) {
        std::bitset<8> bits(buffer);
        char byte = static_cast<char>(bits.to_ulong());
        outputFile.write(&byte, sizeof(char));
    }
	cout<<"compress done."<<endl;
    inputFile.close();
    outputFile.close();
}

void decompressBinaryFile(const std::string &inputFilename, const std::string &outputFilename) {
    std::ifstream inputFile(inputFilename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file!" << std::endl;
        return;
    }

    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file!" << std::endl;
        return;
    }

    char ch;
    while (inputFile.get(ch)) {
        std::bitset<8> bits(static_cast<unsigned char>(ch));
        outputFile << bits;
    }
	cout<<"decompress done."<<endl;
    inputFile.close();
    outputFile.close();
}


double Cal_Compression_Ratio(const std::string& filename1, const std::string& filename2) {
    std::ifstream file_1(filename1, std::ios::binary | std::ios::ate);
    std::ifstream file_2(filename2, std::ios::binary | std::ios::ate);

    if (!file_1.is_open() || !file_2.is_open()) {
        std::cerr << "Error opening files!" << std::endl;
        return 0.0;
    }

    // 获取文件大小
    std::streamsize size1 = file_1.tellg();
    std::streamsize size2 = file_2.tellg();

    file_1.close();
    file_2.close();

    if (size2 == 0) {  // 防止除以零
        std::cerr << "Compressed file size is zero, cannot calculate ratio." << std::endl;
        return 0.0;
    }

    double Ratio = (double)size1 / size2;
    return Ratio;
}

int main() {
    std::ifstream file("Huffman coding.txt");
    std::ifstream file_0("code.txt");
    std::ifstream file_2("result.txt");
    NumCount Cntarray;
    if (file.is_open()) {
        Read_Sort_File(file, &Cntarray);//统计 
        file.close();
    } else {
        std::cout << "Unable to open file";
    }
	//std::cout <<data<<std::endl;
    HuffmanTree tree;
    CreateHuffmanTree(tree, Cntarray.length, Cntarray);  // 建树
    HuffmanCode code;
    CreateHuffmanCode(tree, code, Cntarray.length);  // 创建编码
    char data[MaxSize];
    ReadData(data);
    Encode_table(Cntarray.c,code,Cntarray.length);
    Encode(data, code, Cntarray.length);             // 生成编码文件
    compressBinaryFile("code.txt", "result.txt");//压缩 
    decompressBinaryFile("result.txt", "decompressed.txt");//解压缩 
    Decode(tree, Cntarray.length);                   // 解码
    double ratio = Cal_Compression_Ratio("result.txt", "Huffman coding.txt")*100;
       std::cout << "Compression Ratio: " << ratio<<"%" << std::endl;
    return 0;
}
