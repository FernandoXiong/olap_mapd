#ifndef IMGINDEX_H
#define IMGINDEX_H

#include <string>

void CreateOrInsertIndex(const std::string columnName,
		const std::string folderPath,
		const std::string str_val);

void ShowIndex(const std::string columnName,
		const std::string folderPath,
		const std::string str_val);

#endif
