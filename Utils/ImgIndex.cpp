#include <fstream>
#include <iostream>
#include <bitset>
#include <memory>

#include "ImgIndex.h"
#include "ImgHashCode.h"

static ushort bitcount(int n) {
	n = (n & 0x55555555) + ((n >> 1) & 0x55555555);
	n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
	n = (n & 0x0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f);
	n = (n & 0x00ff00ff) + ((n >> 8) & 0x00ff00ff);
	n = (n & 0x0000ffff) + ((n >> 16) & 0x0000ffff);
	return (ushort)n;
}

static void index_filter_cpu(uint *index, ushort *result, int index_num, uint hashcode[4]) {
	for(int i = 0; i < index_num; ++i) {
		result[i] = 0;
		for(int j = 0; j < 4; ++j) {
			result[i] += bitcount(index[4 * i + j] ^ hashcode[j]);
		}   
	}   
}

void CreateOrInsertIndex(const std::string columnName,
						 const std::string folderPath,
						 const std::string str_val) {						 
	std::string filename(folderPath + "/" + columnName + ".index");
	std::ofstream findex(filename, std::ofstream::app | std::ofstream::binary);
	ImgHashCode imgcode(str_val);

	uint *hashcode = imgcode.hashcode;
	findex.write((char *)(hashcode), sizeof(uint) * CODE_SIZE);
	findex.close();
}

void ShowIndex(const std::string columnName,
			   const std::string folderPath) {
	std::string filename(folderPath + "/" + columnName + ".index");
	std::ifstream findex(filename, std::ifstream::binary);
	std::cout << "show index" << std::endl;
	uint hashcode[CODE_SIZE];
	findex.read((char *)(hashcode), sizeof(hashcode));
	while(!findex.eof()) {
		for(int i = 0; i < CODE_SIZE; ++i) {
			std::cout << std::bitset<32>(hashcode[i]);
		}
		std::cout << std::endl;
		findex.read((char *)(hashcode), sizeof(hashcode));
	}
	findex.close();
}

std::vector<int32_t> SearchIndex(const std::string columnName,
								 const std::string folderPath,
								 const std::string fileName,
								 const size_t indexCount) {
	std::vector<int32_t> result;
	std::string filename(folderPath + "/" + columnName + ".index");
	std::ifstream findex(filename, std::ifstream::binary);
	uint *hashcode = new uint[CODE_SIZE * indexCount]; 
	ImgHashCode imgCode(fileName);
	uint *cmp_hashcode = imgCode.hashcode;
	findex.read((char *)(hashcode), sizeof(uint) * CODE_SIZE * indexCount);
	for(int id = 0; id < indexCount; ++id) {
		int count = 0;
		for(int i = 0; i < 4; ++i) {
			count += bitcount(hashcode[i] ^ cmp_hashcode[i]);
		}
		if(count <= 15) {
			result.push_back(id);
		}
		hashcode += CODE_SIZE;
	}
	findex.close();
	return result;
}
