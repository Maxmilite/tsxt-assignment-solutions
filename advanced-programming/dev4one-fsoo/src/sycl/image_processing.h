#ifndef __IMAGE_PROCESSING_H
#define __IMAGE_PROCESSING_H

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include <sycl/sycl.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/opencv.hpp>
#include <opencv2/photo.hpp>

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <zconf.h>

#include "../util.h"

using namespace sycl;

const int WIDTH = 1920;
const int HEIGHT = 1080;

#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define MIN2(a, b) ((a) > (b) ? (b) : (a))
#define CLIP3(a, low, high) MIN2(MAX2(a, low), high)

namespace sycl_accelerator {

	void buf_add_1(uint8_t *tmp, uint8_t *src, int uv_height, int uv_stride, queue& q) {
		// for (int y = 0; y < uv_height; y++) {
		// 	for (int x = 0; x < uv_stride; x++) {
		// 		buf_tmp[y * uv_stride + x] = src[y * uv_stride + x];
		// 	}
		// }
		auto R = range<1>(uv_height * uv_stride);
		buffer buf_tmp(tmp, R);
		buffer buf_src(src, R);
		q.submit([&](handler &h) {
			accessor TMP(buf_tmp, h, write_only);
			accessor SRC(buf_src, h, read_only);
			h.parallel_for(range<1>(uv_height), [=](id<1> y) {
				for (int x = 0; x < uv_stride; x++) {
					TMP[y * uv_stride + x] = SRC[y * uv_stride + x];
				}
			});
		});
		q.wait();
	}

}

namespace image_process {

	void resize_4x(unsigned char *src, unsigned char *dst, int width, int height, queue& q) {
		int width_half = width >> 1;
		int height_half = height >> 1;

		unsigned char *src_line0 = src;
		unsigned char *src_line1 = src_line0 + width_half;
		unsigned char *dst_line0 = dst;
		unsigned char *dst_line1;

		// first line
		*(dst_line0++) = src_line0[0];
		for (int x = 1; x < width_half; x++) {
			*(dst_line0++) = (src_line0[x - 1] * 3 + src_line0[x] * 1) >> 2;
			*(dst_line0++) = (src_line0[x - 1] * 1 + src_line0[x] * 3) >> 2;
		}

		*(dst_line0++) = src_line0[width_half - 1];

		// middle bilinear interpolate
		dst_line1 = dst_line0 + width;
		for (int y = 1; y < height_half; y++) {
			*(dst_line0++) = (src_line0[0] * 3 + src_line1[1] * 1) >> 2;
			*(dst_line1++) = (src_line0[0] * 1 + src_line1[1] * 3) >> 2;

			for (int x = 1; x < width_half; x++) {
				*(dst_line0++) = (src_line0[x - 1] * 9 + src_line0[x] * 3 + src_line1[x - 1] * 3 + src_line1[x] * 1) >> 4;
				*(dst_line0++) = (src_line0[x - 1] * 3 + src_line0[x] * 9 + src_line1[x - 1] * 1 + src_line1[x] * 3) >> 4;
				*(dst_line1++) = (src_line0[x - 1] * 3 + src_line0[x] * 1 + src_line1[x - 1] * 9 + src_line1[x] * 3) >> 4;
				*(dst_line1++) = (src_line0[x - 1] * 1 + src_line0[x] * 3 + src_line1[x - 1] * 3 + src_line1[x] * 9) >> 4;
			}

			*(dst_line0++) = (src_line0[width_half - 1] * 3 + src_line1[width_half - 1] * 1) >> 2;
			*(dst_line1++) = (src_line0[width_half - 1] * 1 + src_line1[width_half - 1] * 3) >> 2;

			dst_line0 += width;
			dst_line1 += width;
			src_line0 += width_half;
			src_line1 += width_half;
		}

		// last line
		*(dst_line0++) = src_line0[0];
		for (int x = 1; x < width_half; x++) {
			*(dst_line0++) = (src_line0[x - 1] * 3 + src_line0[x] * 1) >> 2;
			*(dst_line0++) = (src_line0[x - 1] * 1 + src_line0[x] * 3) >> 2;
		}

		*(dst_line0++) = src_line0[width_half - 1];
	}

	int file_size(int fd) {
		struct stat st;
		fstat(fd, &st);
		return st.st_size;
	}

	int frame_size_of_cif() {
		int width = WIDTH;
		int heigh = HEIGHT;
		int Y_SIZE = width * heigh;
		int U_SIZE = Y_SIZE / 4;
		int V_SIZE = Y_SIZE / 4;
		int Frame_SIZE = Y_SIZE + U_SIZE + V_SIZE;
		return Frame_SIZE;
	}

	int frames_of_cif_file(int fd) {
		if (fd < 0) {
			printf("Invalid FD!");
			return -1;
		}
		int Frame_SIZE = frame_size_of_cif();
		int fd_size = file_size(fd);
		return fd_size / Frame_SIZE;
	}

	void yuv2rgb(unsigned char *yp, unsigned char *up, unsigned char *vp, unsigned char *rp, unsigned char *gp,
				 unsigned char *bp, int width, int height, int stride, queue& q) {
		/*
		R'= Y' + 0.000*U' + 1.403*V'
		G'= Y' - 0.344*U' - 0.714*V'
		B'= Y' + 1.773*U' + 0.000*V'
		*/
		float mat[9] = {1.0f, 0.0f, 1.403f, 1.0f, -0.344f, -0.714f, 1.0f, 1.773f, 0.0f};
		int uv_offset = 128;

		auto R = range<1>(width * height);
		buffer buf_mat(mat, range<1>(9));
		buffer buf_yp(yp, R);
		buffer buf_up(up, R);
		buffer buf_vp(vp, R);
		buffer buf_rp(rp, R);
		buffer buf_gp(gp, R);
		buffer buf_bp(bp, R);
		
		q.submit([&](handler &h) {
			accessor acc_mat(buf_mat, h, read_only);
			accessor acc_yp(buf_yp, h, read_only);
			accessor acc_up(buf_up, h, read_only);
			accessor acc_vp(buf_vp, h, read_only);
			accessor acc_rp(buf_rp, h, write_only);
			accessor acc_gp(buf_gp, h, write_only);
			accessor acc_bp(buf_bp, h, write_only);
			h.parallel_for(range<1>(height), [=](id<1> y) {
				for (int x = 0; x < width; x++) {
					int lum = acc_yp[y * stride + x];
					int u = acc_up[y * stride + x];
					int v = acc_vp[y * stride + x];

					int r = (int)(acc_mat[0] * lum + acc_mat[1] * (u - uv_offset) + acc_mat[2] * (v - uv_offset));
					int g = (int)(acc_mat[3] * lum + acc_mat[4] * (u - uv_offset) + acc_mat[5] * (v - uv_offset));
					int b = (int)(acc_mat[6] * lum + acc_mat[7] * (u - uv_offset) + acc_mat[8] * (v - uv_offset));

					acc_rp[y * width + x] = CLIP3(r, 0, 255);
					acc_gp[y * width + x] = CLIP3(g, 0, 255);
					acc_bp[y * width + x] = CLIP3(b, 0, 255);
				}
			});

		});
		q.wait();
		// for (int y = 0; y < height; y++) {
		// 	for (int x = 0; x < width; x++) {
		// 		int lum = yp[y * stride + x];
		// 		int u = up[y * stride + x];
		// 		int v = vp[y * stride + x];

		// 		int r = (int)(mat[0] * lum + mat[1] * (u - uv_offset) + mat[2] * (v - uv_offset));
		// 		int g = (int)(mat[3] * lum + mat[4] * (u - uv_offset) + mat[5] * (v - uv_offset));
		// 		int b = (int)(mat[6] * lum + mat[7] * (u - uv_offset) + mat[8] * (v - uv_offset));

		// 		rp[y * width + x] = CLIP3(r, 0, 255);
		// 		gp[y * width + x] = CLIP3(g, 0, 255);
		// 		bp[y * width + x] = CLIP3(b, 0, 255);
		// 	}
		// }
	}

	void abstract_frame_from_CIF_file(int fd, char *Path_And_Prefix_Img, int Len, queue& q) {

		int Frame_SIZE = frame_size_of_cif();
		char file[128];
		memset(file, 0, 128);
		memcpy(file, Path_And_Prefix_Img, Len);
		int ret = -1;
		int frames = 0;
		uint8_t *buf = (uint8_t *)malloc(Frame_SIZE);
		uint8_t *yp = (uint8_t *)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
		uint8_t *up = (uint8_t *)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
		uint8_t *vp = (uint8_t *)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
		uint8_t *rp = (uint8_t *)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
		uint8_t *gp = (uint8_t *)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
		uint8_t *bp = (uint8_t *)malloc(WIDTH * HEIGHT * sizeof(uint8_t));
		if (buf == NULL || yp == NULL || up == NULL || vp == NULL || rp == NULL || gp == NULL || bp == NULL) {
			if (buf) free(buf);
			if (yp) free(yp);
			if (up) free(up);
			if (vp) free(vp);
			if (rp) free(rp);
			if (gp) free(gp);
			if (bp) free(bp);
			util::call_exception("Out of memory");
		}

		while ((ret = read(fd, buf, Frame_SIZE))) {
			frames += 1;
			uint64_t len = strlen(file);
			sprintf(file + len, "%d", frames);
			len = strlen(file);
			sprintf(file + len, "%s", ".png");

			uint8_t *src = buf + WIDTH * HEIGHT;
			uint8_t *buf_tmp = buf;
			int uv_height = HEIGHT / 2;
			int uv_stride = WIDTH / 2;

			memcpy(yp, buf, WIDTH * HEIGHT * sizeof(unsigned char));
			sycl_accelerator::buf_add_1(buf_tmp, src, uv_height, uv_stride, q);

			resize_4x(buf_tmp, up, WIDTH, HEIGHT, q);

			src = buf + WIDTH * HEIGHT + uv_stride * uv_height;
			sycl_accelerator::buf_add_1(buf_tmp, src, uv_height, uv_stride, q);

			resize_4x(buf_tmp, vp, WIDTH, HEIGHT, q);

			yuv2rgb(yp, up, vp, rp, gp, bp, WIDTH, HEIGHT, WIDTH, q);

			cv::Mat img(cvSize(WIDTH, HEIGHT), CV_8UC3);
			{
				buffer buf_img(img.data, range<1>(WIDTH * HEIGHT));
				buffer buf_bp(bp, range<1>(WIDTH * HEIGHT));
				buffer buf_gp(gp, range<1>(WIDTH * HEIGHT));
				buffer buf_rp(rp, range<1>(WIDTH * HEIGHT));
				q.submit([&](handler &h) {
					accessor acc_img(buf_img, h, write_only);
					accessor acc_bp(buf_bp, h, read_only);
					accessor acc_gp(buf_gp, h, read_only);
					accessor acc_rp(buf_rp, h, read_only);
					h.parallel_for(range<1>(HEIGHT), [=](id<1> y) {
						for (int x = 0; x < WIDTH; x++) {
							acc_img[(y * WIDTH + x) * 3 + 0] = acc_bp[y * WIDTH + x];
							acc_img[(y * WIDTH + x) * 3 + 1] = acc_gp[y * WIDTH + x];
							acc_img[(y * WIDTH + x) * 3 + 2] = acc_rp[y * WIDTH + x];
						}
					});
				});
				q.wait();
			}

			imwrite(file, img);

			// int fdw = open(file, O_RDWR | O_CREAT, 0777);
			// write(fdw, buf, ret);
			// close(fdw);
			memset(file, 0, 128);
			memcpy(file, Path_And_Prefix_Img, Len);
			std::cerr << "YUV2 to PNG: Running on frame " << frames << ".\n";
			// memset(file, 0, 128);
		}

		if (buf) free(buf);
		if (yp) free(yp);
		if (up) free(up);
		if (vp) free(vp);
		if (rp) free(rp);
		if (gp) free(gp);
		if (bp) free(bp);
	}

	void run(std::string file_name, int frame_num, queue& q) {
		int fd = open(file_name.c_str(), O_RDONLY);
		abstract_frame_from_CIF_file(fd, "./tmp_work/input_frames/frame_", strlen("./tmp_work/input_frames/frame_"), q);
		close(fd);
	}
}

namespace image_process_inv {

	void run(std::string name) {
		// TODO
		// Just a substitute here

		std::stringstream ss;
		ss << "ffmpeg -framerate 30 -y -i ./tmp_work/output_frames/frame_%d.png -i ./tmp_work/audio.m4a -c:a copy -crf 20 -c:v libx264 -pix_fmt yuv420p ";
		ss << name;
		std::string s;
		getline(ss, s);
		std::cerr << s << std::endl;
		system(s.c_str());

	}
}

#endif