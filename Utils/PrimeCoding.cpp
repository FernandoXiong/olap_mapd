//
//  main.cpp
//  JustDemo
//
//  Created by Victor on 2017/12/31.
//  Copyright © 2017年 Victor. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <ext/hash_map>
#include "stdio.h"
#include <string.h>
#include <stdlib.h>

#include "PrimeCoding.h"

using namespace std;

/*创建单词哈希链表节点 可删除*/

struct str_hash{
    size_t operator()(const string& str) const
    {
        unsigned long __h = 0;
        for (size_t i = 0 ; i < str.size() ; i ++)
            __h = 5*__h + str[i];
        return size_t(__h);
    }
};

//删除字符串中空格，制表符tab等无效字符
string Trim(string& str)
{
    //str.find_first_not_of(" \t\r\n"),在字符串str中从索引0开始，返回首次不匹配"\t\r\n"的位置
    str.erase(0,str.find_first_not_of(" \t\r\n"));
    str.erase(str.find_last_not_of(" \t\r\n") + 1);
    return str;
}


typedef __gnu_cxx::hash_map<string, long,str_hash> StrIntMap;

StrIntMap namemap;

long ToolManager :: getPrimeNumber(long lastNum){
    if (lastNum >= 5) {
        
        long divisor   = lastNum / 6;
        long remainder = lastNum % 6;
        if (remainder > 3) {
            if (this->isPrimeNum(lastNum+2)) {
                return lastNum + 2;
            }else{
                return this->getAdjacentPrimeNumber(divisor + 2);
            }
        }else{
            return this->getAdjacentPrimeNumber(divisor + 1);
        }
        
        
    }else{
        if (lastNum >= 3) {
            return 5;
        }
        return 3;
    }
};

string ToolManager::readFileIntoString(string filename){
    ifstream ifile(filename.c_str());
    //将文件读入到ostringstream对象buf中
    ostringstream buf;
    char ch;
    while(buf&&ifile.get(ch))
        buf.put(ch);
    //返回与流对象buf关联的字符串
    return buf.str();
}

string ToolManager::splitString(string file){
    file = this->replaceSymbolString(file);
    char str[file.length()+5];
    strcpy(str, file.c_str());
    const char * split = " ";
    char * p;
    p = strtok (str,split);
    string restrStr = "";
    int i = 0;
    while(p!=NULL) {
//        printf ("%s\n",p);
        if (i == 0) {
            restrStr = p;
        }else{
            restrStr += ",";
            restrStr += p;
        }
        p = strtok(NULL,split);
        i++;
    }
//    getchar();
    
    return restrStr;
}

long ToolManager::returnPrimeNumber(long long lastNum, long divisor){
    if (lastNum%divisor == 0) {
        return this->returnPrimeNumber(lastNum/divisor, divisor)+1;
    }
    return 0;
}

bool ToolManager::hasFileTool(char *filePath, char *data){
    FILE *fp=fopen(filePath, "w");
    fprintf(fp, "%s", data);
    fclose(fp);
    return true;
}

Product* ToolManager::hasMyFileTool(Product *Head, Product *p, Product *node,long lastPrimeNumber){
    DIR *dir;
    struct dirent *ptr;
    if ((dir=opendir("/Users/apple/Desktop/testFile")) == NULL)
    {
        perror("Open dir error...");
        exit(1);
    }
    string filePath;
    string newfilePath;
    while ((ptr=readdir(dir)) != NULL)
    {
        
        if (strstr(ptr->d_name, ".txt") != NULL) {
            node = new Product; //
            node->fileName = ptr->d_name;
            filePath = "/Users/apple/Desktop/testFile/";
            newfilePath = "/Users/apple/Desktop/testFile/textHasFile/";
            ///Users/apple/Desktop/testFile
            node->filePath = filePath + (ptr->d_name);
            node->fileBody = "12";
            string s = this->readFileIntoString(filePath + (ptr->d_name));
            string splitstr = this->splitString(s);
            //转码
            char allStr[splitstr.length()+5];
            strcpy(allStr, splitstr.c_str());
            const char * slplit = ",";
            char * pStr;
            pStr = strtok (allStr,slplit);
            int  i = 0;
            long long tempProduct = 1;
            string productStr = "";
            while(pStr!=NULL) {
                long tempPrimyNum = 1;
                string tempSStr = pStr;
                if (namemap.find(tempSStr) != namemap.end()) {
                    tempPrimyNum = namemap[tempSStr];
                }else{
                    lastPrimeNumber  = this->getPrimeNumber(lastPrimeNumber);
                    namemap[tempSStr]= lastPrimeNumber;
                    tempPrimyNum     = lastPrimeNumber;
                }
                long tempNum = (i + 1) / 4;
                if ((i + 1)%4 == 0) {
                    tempProduct *= tempPrimyNum;
                    if (tempNum == 1) {
                        productStr += to_string(tempProduct);
                    }else{
                        productStr += ",";
                        productStr += to_string(tempProduct);
                    }
                    tempProduct = 1;
                }else{
                    tempProduct *= tempPrimyNum;
                }
                pStr = strtok(NULL,slplit);
                if (pStr == NULL) {    //如果不存在加入哈希链表
                    if (tempProduct!= 1) {
                        if (tempNum == 0) {
                            productStr += to_string(tempProduct);
                        }else{
                            productStr += ",";
                            productStr += to_string(tempProduct);
                        }
                    }
                    
                }
                i++;
            }
            //cout<<productStr<<endl;
            node->fileBody = productStr;
            
            //存储文件到另外文件夹
            char * bbStr = (char*)(newfilePath + (ptr->d_name)).c_str();
            this->hasFileTool(bbStr, (char*)productStr.c_str());
            
            if (Head == NULL) {  //若Head为NULL，说明还未被设置，这是第一个节点。
                Head = node;     //Head现在为node的地址，即第一个节点的地址。
            }else{               //若不是第一次，p就是上一个node
                p->next = node;  //p中的指针指向node，p是前一个节点的地址，node是这个节点的地址
            }
            p = node;            //node变为p，p的地址变为这个node的地址
            //            cout<<s<<"\n";
        }
    }
    p->next = NULL;      //这个p中的指针指向end
    closedir(dir);
    return Head;
}

Product* ToolManager::hasMyFileTool(string filePath, Product *Head, Product *p, Product *node,long lastPrimeNumber){
	if (strstr(filePath.c_str(), ".txt") != NULL) {
		node = new Product; //
		node->fileName = filePath;
		///Users/apple/Desktop/testFile
		node->filePath = filePath;
		node->fileBody = "12";
		string s = this->readFileIntoString(filePath);
		string splitstr = this->splitString(s);
		//转码
		char allStr[splitstr.length()+5];
		strcpy(allStr, splitstr.c_str());
		const char * slplit = ",";
		char * pStr;
		pStr = strtok (allStr,slplit);
		int  i = 0;
		long long tempProduct = 1;
		string productStr = "";
		while(pStr!=NULL) {
			long tempPrimyNum = 1;
			string tempSStr = pStr;
			if (namemap.find(tempSStr) != namemap.end()) {
				tempPrimyNum = namemap[tempSStr];
			}else{
				lastPrimeNumber  = this->getPrimeNumber(lastPrimeNumber);
				namemap[tempSStr]= lastPrimeNumber;
				tempPrimyNum     = lastPrimeNumber;
			}
			long tempNum = (i + 1) / 4;
			if ((i + 1)%4 == 0) {
				tempProduct *= tempPrimyNum;
				if (tempNum == 1) {
					productStr += to_string(tempProduct);
				}else{
					productStr += ",";
					productStr += to_string(tempProduct);
				}
				tempProduct = 1;
			}else{
				tempProduct *= tempPrimyNum;
			}
			pStr = strtok(NULL,slplit);
			if (pStr == NULL) {    //如果不存在加入哈希链表
				if (tempProduct!= 1) {
					if (tempNum == 0) {
						productStr += to_string(tempProduct);
					}else{
						productStr += ",";
						productStr += to_string(tempProduct);
					}
				}

			}
			i++;
		}
		//cout<<productStr<<endl;
		node->fileBody = productStr;

		if (Head == NULL) {  //若Head为NULL，说明还未被设置，这是第一个节点。
			Head = node;     //Head现在为node的地址，即第一个节点的地址。
		}else{               //若不是第一次，p就是上一个node
			p->next = node;  //p中的指针指向node，p是前一个节点的地址，node是这个节点的地址
		}
		p = node;            //node变为p，p的地址变为这个node的地址
		//            cout<<s<<"\n";
	}
	p->next = NULL;      //这个p中的指针指向end
	return Head;
}

bool ToolManager::findHasWord(Product *Head, Product *p, Product *node,string inputWord){
    
    long inputWordHaXiNum = 0;
    if (namemap.find(inputWord) != namemap.end()) {
        //        string tempWordNumStr =
        inputWordHaXiNum =namemap[inputWord];
        
    }else{
        //cout<<"对不起，在当前单词库中未发现该单词，请录入!"<<endl;
		return false;
    }
    
    if (inputWordHaXiNum != 0) {
        p = Head;
        long inputWordNum = 0;
        int myCount = 0;
        while (p != NULL) {
            string wordData = p->fileBody;
            char allWordArr[wordData.length()+5];
            strcpy(allWordArr, wordData.c_str());
            const char * smplit = ",";
            char * wStr;
            wStr = strtok (allWordArr,smplit);
            inputWordNum = 0;
            while(wStr!=NULL) {
                long long  tempWordNum = atoll(wStr);
                inputWordNum += this->returnPrimeNumber(tempWordNum, inputWordHaXiNum);
                wStr = strtok(NULL,smplit);
            }
            if (inputWordNum>0) {
                //cout<<"出现单词"<<inputWord<<"的文件路径为："<<p->filePath<<" 文件名为："<<p->fileName<<" 出现的次数为："<<inputWordNum<<endl;
                myCount += 1;
            }
            p = p->next;
        }
        if (myCount == 0) {
            //cout<<"该文件目录下未出现单词"<<inputWord<<endl;
			return false;
        }
    }
    return true;
}


bool word_in_text(string word, string filePath) {
    /*初始化工具类*/
    ToolManager manager;
    /*初始化哈希单词表*/
    long lastPrimeNumber = 0;    //标识最后一个素数
    
    //哈希链表初始化
    ifstream fin("/home/olap/lx/data/word.csv"); //打开文件流操作
    string line;
    while (getline(fin, line))   //整行读取，换行符“\n”区分，遇到文件尾标志eof终止读取
    {
        string ms = Trim(line);
        lastPrimeNumber  = manager.getPrimeNumber(lastPrimeNumber);
        namemap[ms] = lastPrimeNumber;
    }
    
    Product *Head = NULL;
    Product *p    = NULL;
    Product *node = NULL;  //这些都是Product型的指针/Users/victor/Desktop/testFile

    /*数据哈希map化*/
    Head = manager.hasMyFileTool(filePath, Head, p, node, lastPrimeNumber);

    
    /*哈希文件匹配*/
    return manager.findHasWord(Head, p, node, word);//哈希查找并输出
}

/*
int main(int argc, const char * argv[]) {
	if(argc != 3) {
		cout << "params error!" << endl;
		exit(0);
	}
	string word(argv[1]);
	string filePath(argv[2]);
    cout << (word_in_text(word, filePath) ? "yes" : "no") << endl; 
    return 0;
}
*/
