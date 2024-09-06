#pragma once
#include "utils.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#include <vector>
#include <fstream>

class Img {
private:
	std::vector<int8> img_data;
	std::vector<int8> gimg_data; //GRAY IMAGE
	int8*** sample;
	int8* data = 0;
public:
	int width, height, channels;
	Img(const char* filename, int channel_default = 0) {
		data = stbi_load(filename, &width, &height, &channels, channel_default);
		img_data.assign(data, data + width * height * channels);
		if (data == nullptr) {
			std::cerr << "Failed to load image\n";
		}

		sample = new int8 * *[height];
		for (int i = 0; i < height; ++i) {
			sample[i] = new int8 * [width];
			for (int j = 0; j < width; ++j) {
				sample[i][j] = new int8[channels];
			}
		}

		// Copy pixel data to the 3D array
		size_t index = 0;
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				for (int k = 0; k < channels; ++k) {
					sample[i][j][k] = img_data[index++];
				}
			}
		}
	}
	~Img() {
		stbi_image_free(data);
	}
	void convertToGrayscale() {
		for (int i = 0; i < width * height; ++i) {
			uint8_t r = img_data[i * 3];
			uint8_t g = img_data[i * 3 + 1];
			uint8_t b = img_data[i * 3 + 2];
			uint8_t gray = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);
			img_data[i * 3] = img_data[i * 3 + 1] = img_data[i * 3 + 2] = gimg_data[i] = gray;
		}
		channels = 1;
	}
	bool save(const std::string& file_path) const {
		return stbi_write_png(file_path.c_str(), width, height, channels, gimg_data.data(), width*channels) != 0;
	}
	void write_to_file(const char* filename) {
		std::ofstream outfile(filename);
		if (!outfile.is_open()) {
			std::cerr << "Failed to open output file\n";
		}

		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				outfile << "[ ";
				for (int k = 0; k < channels; ++k) {
					outfile << static_cast<int>(sample[i][j][k]) << " ";
				}
				outfile << "] "; // Newline for each pixel row
			}
			outfile << "\n"; // Newline for each image row
		}

		outfile.close();
		std::cout << "Pixel data written to pixel_data.txt\n";

	}
	void free_sample() {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				delete[] sample[i][j];
			}
			delete[] sample[i];
		}
		delete[] sample;
	}
};