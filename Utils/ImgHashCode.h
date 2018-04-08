#ifndef IMGHASHCODE_H
#define IMGHASHCODE_H

#include "../Shared/funcannotations.h"

#include <stdint.h>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#define CODE_SIZE	4
#define WINDOW_SIZE	8

typedef unsigned int uint;

/*
extern "C" DEVICE bool ImageCompare(const char* s1,
									const int32_t s1_len,
									const char* s2,
									const int32_t s2_len);

extern "C" DEVICE bool image_eq(const char *lhs,
								const int32_t lhs_len,
								const char* rhs,
								const int32_t rhs_len);

extern "C" DEVICE bool image_ne(const char *lhs,
								const int32_t lhs_len,
								const char* rhs,
								const int32_t rhs_len);
								*/

extern "C" bool ImageCompare(const char* s1,
		const int32_t s1_len,
		const char* s2,
		const int32_t s2_len);

extern "C" bool image_eq(const char *lhs,
		const int32_t lhs_len,
		const char* rhs,
		const int32_t rhs_len);

extern "C" bool image_ne(const char *lhs,
		const int32_t lhs_len,
		const char* rhs,
		const int32_t rhs_len);

extern "C" int8_t image_eq_nullable(const char *lhs,
		const int32_t lhs_len,
		const char* rhs,
		const int32_t rhs_len,
		const int8_t bool_null);

extern "C" int8_t image_ne_nullable(const char *lhs,
		const int32_t lhs_len,
		const char* rhs,
		const int32_t rhs_len,
		const int8_t bool_null);

class ImgHashCode {
public:
	ImgHashCode(std::string img_path);
	bool similarTo(ImgHashCode iCode);
	void print();

	uint hashcode[CODE_SIZE];

private:
	short GetRotation(const cv::Mat &img);
	void ComputeHashCode(cv::Mat img, uint *hashcode);
	int BitCount(uint num);
};

#endif
