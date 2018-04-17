#ifndef PRIMECODING_H
#define PRIMECODING_H

#include <string>
#include <string.h>
#include <math.h>

/*创建文件实体*/
struct Product{
    std::string   fileName;
    std::string   filePath;
    std::string   fileBody;
    Product *next;
};

/*创建文件查找工具类*/
class ToolManager{
public:
    long   getPrimeNumber(long lastNum);            //获取素数
    std::string readFileIntoString(std::string filename);     //读取txt文件中的
    std::string splitString(std::string file);                //拆分字符串
    long   returnPrimeNumber(long long lastNum,long divisor);         //返回每个单词组中每个单词的个数
    bool hasFileTool(char* filePath,char *data);         //哈希化文件到另一文件夹
    
    Product* hasMyFileTool(std::string filePath, Product *Head,Product *p,Product *node,long lastPrimeNumber);  //哈希话文件
    Product* hasMyFileTool(Product *Head,Product *p,Product *node,long lastPrimeNumber);  //哈希话文件
    bool findHasWord(Product *Head,Product *p,Product *node,std::string inputWord);  //哈希查找文件
private:
    bool isPrimeNum(long m){   //判断是不是素数
        int count=0;
        for (int i=2;i<=sqrt(m);i++)
        {
            if(m%i==0)
                count++;
        }
        if (count==0)
            return 1;
        else
            return 0;
    };
    long getAdjacentPrimeNumber(long x){ //获取相邻素数
        long priNum = 6 * x-1;
        while (!(this->isPrimeNum(priNum) || this->isPrimeNum(priNum+ 2))) {
            x++;
            priNum = 6 * x-1;
        }
        if (this->isPrimeNum(priNum)) {
            return priNum;
        }else{
            return priNum + 2;
        }
    };
    std::string replaceSymbolString(std::string file){ //替换无用标点符号
        char s[file.length()+10];
        int i,j;
        strcpy(s, file.c_str());//strncpy(buf, str.c_str(), 10);
        for(i = 0,j = 0;s[i];++i)
        {
            if(ispunct(s[i])&&!(s[i] == '\'')) {
                s[j++] = ' ';
            }
            else {
                if (s[i] >= 'A' && s[i] <= 'Z') {
                    s[i] += 32;
                }
                s[j++] = s[i];
            }
        }
        s[j] = '\0';
        return s;
    };
    
};

bool word_in_text(std::string word, std::string filePath);

#endif
