#include <fstream>
#include <iostream>
#include <bitset>

#include "ImgIndex.h"
#include "ImgHashCode.h"

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
			   const std::string folderPath,
			   const std::string str_val) {
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
