#ifndef IMGINDEX_H
#define IMGINDEX_H

#include <string>
#include <vector>

void CreateOrInsertIndex(const std::string columnName,
		const std::string folderPath,
		const std::string str_val);

void ShowIndex(const std::string columnName,
		const std::string folderPath);
		
std::vector<int32_t> SearchIndex(const std::string columnName,
		const std::string folderPath,
		const std::string fileName,
		const size_t indexCount);
		
#endif
