#include <chrono>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>

#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <dlib/image_processing.h>
// #include <dlib/image_processing/frontal_face_detector.h>
#include "dlib_sycl/image_processing/frontal_face_detector.h"
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/opencv/cv_image_abstract.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/photo.hpp>

#include <sycl/sycl.hpp>

#include "sycl/image_processing.h"
#include "sycl/onevpl_video_process.h"
#include "sycl/sycl_handler.h"
#include "util.h"

using lint = long long;

struct correspondens {
	std::vector<int> index;
};

int face_landmark_detection(dlib::array2d<unsigned char> &, dlib::shape_predictor, std::vector<cv::Point2f> &);
lint get_time();
lint calc_time_elapsed(lint);
void delaunay_triangulation(const std::vector<cv::Point2f> &, std::vector<correspondens> &, cv::Rect);
void warp_triangle(cv::Mat &, cv::Mat &, std::vector<cv::Point2f> &, std::vector<cv::Point2f> &);
void apply_affine_transform(cv::Mat &, cv::Mat &, std::vector<cv::Point2f> &, std::vector<cv::Point2f> &);

dlib::frontal_face_detector detector;

sycl::queue q;

int main(int argc, char *argv[]) {

	if (argc != 4) {
		util::print_usage();
	}

	mkdir("./tmp_work", 0777);
	mkdir("./tmp_work/input_frames", 0777);
	mkdir("./tmp_work/output_frames", 0777);

	std::cout << "Origin Video: " << argv[1] << std::endl;
	std::cout << "Pattern Image: " << argv[2] << std::endl;

	q = queue(default_selector_v);
	std::cout << "oneAPI Parallel Device: " << q.get_device().get_info<info::device::name>() << "\n";

	std::cout << "Initializing dlib Shape Detector" << std::endl;
	dlib::shape_predictor sp;
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> sp;
	detector = dlib::get_frontal_face_detector();
	std::cout << "dlib Shape Detector Initialized" << std::endl;

	std::cout << "Decoding Video" << std::endl;
	util::split_va(argv[1]);
	int frame_num = video_process::run(std::string("./tmp_work/input.h264"));
	std::cout << "Video Decoded" << std::endl;

	if (frame_num == -1) return -1;

	std::cout << "Spliting yuv video into frames" << std::endl;
	image_process::run("./tmp_work/out.yuv", frame_num, q);
	std::cout << "Frames split" << std::endl;

	dlib::array2d<unsigned char> img_dlib1;
	cv::Mat img_cv1;
	try {
		dlib::load_image(img_dlib1, argv[2]);
		img_cv1 = cv::imread(argv[2]);
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		exit(-1);
	}
	img_cv1.convertTo(img_cv1, CV_32F);
	std::vector<cv::Point2f> points1;
	face_landmark_detection(img_dlib1, sp, points1);

	for (int index = 1; index <= frame_num; ++index) {

		std::cout << "Now processing frame " << index << "." << std::endl;

		std::stringstream ss;
		std::string file_in, file_out;
		ss << "./tmp_work/input_frames/frame_" << index << ".png";
		ss >> file_in;
		ss.clear();
		ss << "./tmp_work/output_frames/frame_" << index << ".png";
		ss >> file_out;

		lint current_time = get_time();
		dlib::array2d<unsigned char> img_dlib2;
		cv::Mat img_cv2;
		try {
			dlib::load_image(img_dlib2, file_in);
			img_cv2 = cv::imread(file_in);
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
		if (!img_cv1.data || !img_cv2.data) {
			std::cerr << "No image data were loaded" << std::endl;
			return -1;
		}
		current_time = calc_time_elapsed(current_time);

		std::vector<cv::Point2f> points2;
		int status = 1;
		// status &= face_landmark_detection(img_dlib1, sp, points1);
		status &= face_landmark_detection(img_dlib2, sp, points2);
		if (!status) {
			try {
				cv::imwrite(file_out, img_cv2);
			} catch (const std::exception &e) {
				std::cerr << e.what() << std::endl;
				// cv::imwrite("/home/maxmilite/GitHub/dev4one/dist/samples/output.png", output);
			}
			current_time = calc_time_elapsed(current_time);
			continue;
		}
		current_time = calc_time_elapsed(current_time);

		cv::Mat img_cv1_warped = img_cv2.clone();



		img_cv1_warped.convertTo(img_cv1_warped, CV_32F);

		std::vector<cv::Point2f> hull1, hull2;
		std::vector<int> hull_index;

		cv::convexHull(points2, hull_index, false, false);
		for (const auto &i : hull_index) {
			hull1.push_back(points1[i]);
			hull2.push_back(points2[i]);
		}
		current_time = calc_time_elapsed(current_time);

		std::vector<correspondens> delaunary_tri;
		cv::Rect rect(0, 0, img_cv1_warped.cols, img_cv1_warped.rows);
		delaunay_triangulation(hull2, delaunary_tri, rect);

		cv::Mat img_cv1_t = img_cv1;

		for (const auto &corpd : delaunary_tri) {
			std::vector<cv::Point2f> t1, t2;
			for (int j = 0; j < 3; ++j) {
				t1.push_back(hull1[corpd.index[j]]);
				t2.push_back(hull2[corpd.index[j]]);
			}
			warp_triangle(img_cv1_t, img_cv1_warped, t1, t2);
		}
		current_time = calc_time_elapsed(current_time);

		std::vector<cv::Point> hull8U;
		for (const auto &i : hull2) {
			hull8U.push_back(cv::Point(i.x, i.y));
		}
		cv::Mat mask = cv::Mat::zeros(img_cv2.rows, img_cv2.cols, img_cv2.depth());
		cv::fillConvexPoly(mask, &hull8U[0], hull8U.size(), cv::Scalar(255, 255, 255));

		cv::Rect r = boundingRect(hull2);
		cv::Point center = (r.tl() + r.br()) / 2;
		current_time = calc_time_elapsed(current_time);

		cv::Mat output;
		img_cv1_warped.convertTo(img_cv1_warped, CV_8UC3);
		cv::seamlessClone(img_cv1_warped, img_cv2, mask, center, output, cv::NORMAL_CLONE);
		try {
			cv::imwrite(file_out, output);
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
			// cv::imwrite("/home/maxmilite/GitHub/dev4one/dist/samples/output.png", output);
		}
	}

	std::cout << "Merging frames" << std::endl;
	image_process_inv::run(argv[3]);
	std::cout << "Frames merged" << std::endl;

	std::cout << "Video now saved as " << argv[3] << std::endl;

	return 0;
}

int face_landmark_detection(dlib::array2d<unsigned char> &img, dlib::shape_predictor sp, std::vector<cv::Point2f> &landmark) {
	// std::cerr << std::fixed << std::setprecision(3) << "Detector start time: " << std::chrono::system_clock::now().time_since_epoch().count() * 1.0 * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den << std::endl;
	std::vector<dlib::rectangle> dets = detector(img);
	// std::cerr << std::fixed << std::setprecision(3) << "Detector end time: " << std::chrono::system_clock::now().time_since_epoch().count() * 1.0 * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den << std::endl;
	if (dets.empty())
		return 0;
	dlib::full_object_detection shape = sp(img, dets[0]);

	for (int i = 0; i < shape.num_parts(); ++i) {
		float x = shape.part(i).x();
		float y = shape.part(i).y();
		landmark.push_back(cv::Point2f(x, y));
	}
	return 1;
}

lint get_time() {
	return std::chrono::system_clock::now().time_since_epoch().count();
}

lint calc_time_elapsed(lint x) {
	lint v = get_time();
	std::cerr << std::fixed << std::setprecision(3) << "Time elapsed: " << (v - x) * 1.0 * std::chrono::system_clock::period::num / std::chrono::system_clock::period::den << std::endl;
	return v;
}

void delaunay_triangulation(const std::vector<cv::Point2f> &hull, std::vector<correspondens> &delaunay_tri, cv::Rect rect) {

	cv::Subdiv2D subdiv(rect);
	for (const auto &i : hull)
		subdiv.insert(i);
	std::vector<cv::Vec6f> triangle_list;
	subdiv.getTriangleList(triangle_list);

	for (const cv::Vec6f &t : triangle_list) {
		std::vector<cv::Point2f> pt;
		correspondens ind;
		pt.push_back(cv::Point2f(t[0], t[1]));
		pt.push_back(cv::Point2f(t[2], t[3]));
		pt.push_back(cv::Point2f(t[4], t[5]));

		if (rect.contains(pt[0]) && rect.contains(pt[1]) && rect.contains(pt[2])) {
			int count = 0;
			for (int j = 0; j < 3; ++j)
				for (size_t k = 0; k < hull.size(); k++)
					if (abs(pt[j].x - hull[k].x) < 1.0 && abs(pt[j].y - hull[k].y) < 1.0) {
						ind.index.push_back(k);
						count++;
					}
			if (count == 3)
				delaunay_tri.push_back(ind);
		}
	}
}

void warp_triangle(cv::Mat &img1, cv::Mat &img2, std::vector<cv::Point2f> &t1, std::vector<cv::Point2f> &t2) {
	cv::Rect r1 = cv::boundingRect(t1);
	cv::Rect r2 = cv::boundingRect(t2);
	std::vector<cv::Point2f> t1_rect, t2_rect;
	std::vector<cv::Point> t2_rect_i;

	for (int i = 0; i < 3; ++i) {
		t1_rect.push_back(cv::Point2f(t1[i].x - r1.x, t1[i].y - r1.y));
		t2_rect.push_back(cv::Point2f(t2[i].x - r2.x, t2[i].y - r2.y));
		t2_rect_i.push_back(cv::Point(t2[i].x - r2.x, t2[i].y - r2.y));
	}

	cv::Mat mask = cv::Mat::zeros(r2.height, r2.width, CV_32FC3);
	cv::fillConvexPoly(mask, t2_rect_i, cv::Scalar(1.0, 1.0, 1.0), 16, 0);

	cv::Mat img1_rect;
	img1(r1).copyTo(img1_rect);
	cv::Mat img2_rect = cv::Mat::zeros(r2.height, r2.width, img1_rect.type());

	apply_affine_transform(img2_rect, img1_rect, t1_rect, t2_rect);

	cv::multiply(img2_rect, mask, img2_rect);
	cv::multiply(img2(r2), cv::Scalar(1.0, 1.0, 1.0) - mask, img2(r2));
	img2(r2) = img2(r2) + img2_rect;
}

void apply_affine_transform(cv::Mat &warp_image, cv::Mat &src, std::vector<cv::Point2f> &src_tri, std::vector<cv::Point2f> &dst_tri) {
	cv::Mat warp_mat = cv::getAffineTransform(src_tri, dst_tri);
	cv::warpAffine(src, warp_image, warp_mat, warp_image.size(), cv::INTER_LINEAR, cv::BORDER_REFLECT_101);
}