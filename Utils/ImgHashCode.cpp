#include <iostream>
#include <bitset>

#include "ImgHashCode.h"

using namespace cv;

extern "C" bool img_test(int64_t a) {return true;}

extern "C" bool ImageCompare(const char* s1,
		const int32_t s1_len,
		const char* s2,
		const int32_t s2_len) {
	std::string img_path1(s1, s1_len);
	std::string img_path2(s2, s2_len);
	return ImgHashCode(img_path1).similarTo(ImgHashCode(img_path2));
}

extern "C" bool image_eq(const char *lhs,
		const int32_t lhs_len,
		const char* rhs,
		const int32_t rhs_len) {
	return ImageCompare(lhs, lhs_len, rhs, rhs_len);
}

extern "C" bool image_ne(const char *lhs,
		const int32_t lhs_len,
		const char* rhs,
		const int32_t rhs_len) {
	return !ImageCompare(lhs, lhs_len, rhs, rhs_len);
}

#define IMG_CMP_NULLABLE(base_func)																				    \
	extern "C" int8_t base_func##_nullable(																	        \
		const char* lhs, const int32_t lhs_len, const char* rhs, const int32_t rhs_len, const int8_t bool_null) {	\
		if(!lhs || !rhs) {																							\
			return bool_null;																						\
		}																											\
		return base_func(lhs, lhs_len, rhs, rhs_len) ? 1 : 0;														\
	}

IMG_CMP_NULLABLE(image_eq)
IMG_CMP_NULLABLE(image_ne)

#undef IMG_CMP_NULLABLE

ImgHashCode::ImgHashCode(std::string img_path) {
	Mat img = imread(img_path, 0); 
	if(img.empty()) {
		std::cout << "read image failed!" << std::endl;
		return;
	}   
	for(int i = 0; i < CODE_SIZE; ++i)
		hashcode[i] = 0;
	Mat img_blur;
	GaussianBlur(img, img_blur, Size(3, 3), 0, 0); 

	Mat img_resize;
	int r_height = 201, r_width = 201;
	resize(img_blur, img_resize, Size(r_width, r_height), (0, 0), (0, 0), cv::INTER_LINEAR);
	short rotation = GetRotation(img_resize);
	Point2f center((float)(r_width - 1) / 2, (float)(r_height - 1) / 2); 
	Mat rot_mat, img_rotate;
	switch(rotation) {
		case 0:
			return; 
		case 1:
			ComputeHashCode(img_resize, hashcode);
			return;
		case 2:
			rot_mat = getRotationMatrix2D(center, 90, 1.0);
			warpAffine(img_resize, img_rotate, rot_mat, img_resize.size());
			ComputeHashCode(img_rotate, hashcode);
			return;
		case 3:
			rot_mat = getRotationMatrix2D(center, 180, 1.0);
			warpAffine(img_resize, img_rotate, rot_mat, img_resize.size());
			ComputeHashCode(img_rotate, hashcode);
			return;
		case 4:
			rot_mat = getRotationMatrix2D(center, 270, 1.0);
			warpAffine(img_resize, img_rotate, rot_mat, img_resize.size());
			ComputeHashCode(img_rotate, hashcode);
			return;
		default:
			std::cout << "error" << std::endl;
			return;
	}   
}

bool ImgHashCode::similarTo(ImgHashCode iCode) {
	int count = 0;
	uint *compareHashCode = iCode.hashcode;
	for(int i = 0; i < CODE_SIZE; ++i) {
		count += BitCount(hashcode[i] ^ compareHashCode[i]); 
	}

/*
	float similarity = 1.0 - (float)count / (CODE_SIZE * 32);
	if(similarity >= 0.9)
		return true;
	else
		return false;
*/
	return count <= 15;
}

void ImgHashCode::print() {
	for(int i = CODE_SIZE - 1; i >= 0; --i)
		std::cout << std::bitset<32>(hashcode[i]);
	std::cout << std::endl;
}

short ImgHashCode::GetRotation(const Mat &img) {
	int m_10 = 0, m_01 = 0, m_00 = 0;

	int iWidth = img.cols;
	int iHeight = img.rows;
	for(int i = 0; i < iHeight; ++i) {
		int point = i * iWidth;
		for(int j = 0; j < iWidth; ++j) {
			int light = img.data[point + j];
			m_00 += light;
			m_01 += (i + 1) * light;
			m_10 += (j + 1) * light;
		}
	}

	if(m_00 == 0)
		return 0;
	else {
		float x = (float)m_10 / m_00 - 1;
		float y = (float)m_01 / m_00 - 1;
		float center_x = (float)(iWidth - 1) / 2;
		float center_y = (float)(iHeight - 1) / 2;
		//std::cout << "(x, y): (" << x << ", " << y << ")" << std::endl;
		//std::cout << "(center_x, center_y): (" << center_x << ", " << center_y << ")" << std::endl;

		if(x >= center_x && y < center_y)
			return 2;
		else if(x > center_x && y >= center_y)
			return 3;
		else if(x <= center_x && y > center_y)
			return 4;
		else
			return 1;

	}
}

void ImgHashCode::ComputeHashCode(Mat img, uint *hashcode) {
	int iWidth = img.cols;
	int iHeight = img.rows;
	int hashValue[CODE_SIZE * 32];
	for(int i = 0; i < CODE_SIZE * 32; ++i)
		hashValue[i] = 0;

	for(int y = WINDOW_SIZE - 1; y < iHeight - WINDOW_SIZE; ++y) {
		for(int x = WINDOW_SIZE - 1; x < iWidth - WINDOW_SIZE; ++x) {
			int count = 0;
			for(int i = -WINDOW_SIZE + 1; i <= 0; ++i) {
				for(int j = -WINDOW_SIZE + 1; j <= WINDOW_SIZE; ++j) {
					uchar light = img.data[(y + i) * iWidth + x + j];
					uchar com_light = img.data[(y - i + 1) * iWidth + x - j + 1];
					if(light > com_light)
						++hashValue[count];
					else if(light < com_light)
						--hashValue[count];
					++count;
				}
			}
			/*
			   for(int i = 0; i < CODE_SIZE * 32; ++i)
			   cout << hashValue[i] << ", ";
			   cout << endl;
			   */
		}
	}

	for(int i = 0; i < CODE_SIZE * 32; ++i) {
		//cout << hashValue[i] << ", ";
		for(int j = CODE_SIZE - 1; j > 0; --j) {
			hashcode[j] <<= 1;
			hashcode[j] += ((hashcode[j - 1] & (1 << 31)) != 0);
		}
		hashcode[0] <<= 1;
		if(hashValue[i] > 0) {
			++hashcode[0];
		}
		/*
		   for(int i = CODE_SIZE - 1; i >= 0; --i)
		   cout << bitset<32>(hashcode[i]);
		   cout << endl;
		   */
	}
}

int ImgHashCode::BitCount(uint num) {
	int count = 0;

	while(num) {
		num &= num - 1;
		++count;
	}
	return count;
}
