#include <string.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#define MaxSize 1888
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
// 定义比较函数，用于按值排序
bool cmp(const std::pair<char, int>& a, const std::pair<char, int>& b) {
    return a.second < b.second;
}

int Read_Sort_File(std::ifstream& file, NumCount* paraCnt, char* total) {
    if (!file.is_open()) {
        std::cout << "Unable to open file\n";
        return 1;
    }

    std::map<char, int>
        counts;  // 键值对，字符为“键”，其在文章中的出现次数为值；
    std::cout << "the input file is:" << std::endl;
    file.getline(total, MaxSize);
    std::cout << total << std::endl;

    std::cout << std::endl;
    char c;
    while (file.get(c)) {
        if (std::isalpha(c) || std::ispunct(c) || std::isspace(c) ||
            c == '\n') {
            // 检查是否为字母或标点符号
            counts[c]++;
        }
    }

    for (auto const& pair : counts) {
        std::cout << pair.first << ": " << pair.second << '\n';
    }
    // // 创建一个 vector，存储 counts 中的元素
    // std::vector<std::pair<int, int>> vec(counts.begin(), counts.end());

    // // 使用自定义的比较函数，对 vector 进行排序
    // std::sort(vec.begin(), vec.end(), cmp);

    // // 打印排序后的键值对
    // for (const auto& pair : vec) {
    //     std::cout << pair.first << ": " << pair.second << std::endl;
    // }
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

void Encode(char* data, HuffmanCode HC, int length) {
    std::ofstream outfile;
    outfile.open("code.txt");
    for (size_t i = 0; i < strlen(data);
         ++i)  // 依次读入数据，查找对应的编码，写入编码文件
    {
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

void Decode(HuffmanTree HT, int length) {
    char codetxt[MaxSize * length];
    std::ifstream infile;
    infile.open("code.txt");
    infile.getline(codetxt, MaxSize * length);
    infile.close();

    std::ofstream outfile;
    outfile.open("out.txt");

    int root = 2 * length - 1;  // 从根节点开始遍历
    for (size_t i = 0; i < strlen(codetxt); ++i) {
        if (codetxt[i] == '0')
            root = HT[root].l_child;  // 为0表示向左遍历
        else if (codetxt[i] == '1')
            root = HT[root].r_child;  // 为1表示向右遍历
        if (HT[root].l_child == 0 &&
            HT[root].r_child ==
                0)  // 如果已经是叶子节点，输出到输出文件中，然后重新回到根节点
        {
            outfile << HT[root].data;
            root = 2 * length - 1;
        }
    }
    outfile.close();
    std::cout << "the output txt has been written" << std::endl;
    std::cout << std::endl;
}
double Cal_Compression_Ratio(std::ifstream& file_1, std::ifstream& file_2) {
    file_1.seekg(0, std::ios::end);
    std::streamsize size1 = file_1.tellg();
    file_1.seekg(0,
                 std::ios::beg);  // 重置读取位置到beginning

    file_2.seekg(0, std::ios::end);
    std::streamsize size2 = file_2.tellg();
    file_2.seekg(0,
                 std::ios::beg);  // 重置读取位置到beginning

    double Ratio = (double)size1 / size2;
    return Ratio;
}
int main() {
    std::ifstream file("Huffman coding.txt");
    std::ifstream file_0("code.txt");
    char data[MaxSize];
    NumCount Cntarray;
    if (file.is_open()) {
        Read_Sort_File(file, &Cntarray, data);
        file.close();
    } else {
        std::cout << "Unable to open file";
    }

    HuffmanTree tree;
    CreateHuffmanTree(tree, Cntarray.length, Cntarray);  // 建树
    HuffmanCode code;
    CreateHuffmanCode(tree, code, Cntarray.length);  // 创建编码
    Encode(data, code, Cntarray.length);             // 生成编码文件
    Decode(tree, Cntarray.length);                   // 解码
    double rate = Cal_Compression_Ratio(file_0, file) * 100;
    std::cout << "The calculation compression ratio is" << rate << "%"
              << std::endl;
    std::cout << "Please view the generated TXT file to check the result"
              << std::endl;

    return 0;
}
