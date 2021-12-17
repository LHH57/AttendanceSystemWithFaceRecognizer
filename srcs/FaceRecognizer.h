#ifndef _FACERECOGNIZER_H_
#define _FACERECOGNIZER_H_

#include "opencv2/opencv.hpp"
#include "opencv2/face.hpp"
#include <string>

class FaceRecognizer {
 private:
    using FaceRecognizerType = cv::face::FisherFaceRecognizer;  // 分类方法
    static constexpr double Admissible_Confidence = 1800;       // 置信度（越小越好）
    static constexpr char Data_Folder_Path[] = "../data/";      // 照片路径
    static constexpr char Pic_File_Format[] = ".pgm";           // 照片格式
    static constexpr int Face_ROI_Region_Height = 231;          // 照片大小（高）
    static constexpr int Face_ROI_Region_Width = 195;           // 照片大小（宽）
    static constexpr int Inital_Face_Number = 16;               // 自带数据集大小，仅供测试使用。

 private:
    cv::CascadeClassifier cas;
    cv::VideoCapture cap;
    cv::Ptr<cv::face::FaceRecognizer> ptr;

 public:
    FaceRecognizer();
    FaceRecognizer(const std::string &detector_data_file, const std::string &classifier_file_file);

    int recognize(cv::Mat &img, bool draw_res = false);

    bool loadDetectorFile(const std::string &detector_data_file);
    void loadClassifierFile(const std::string &classifier_data_file);

    bool isReady();

    void trainClassifier(std::string&& pics_csv_path, const std::string &classifier_data_file);
    void saveFacePhotos(cv::Mat &img, int face_id);

    bool takePhoto(cv::Mat &img);

 private:
    cv::Mat getFace(cv::Mat &img, bool draw_faces = false);
    std::pair<int, double> predictFace(const cv::Mat &roiImg);

    static void
    read_csv_file(const std::string &file_name, std::vector<cv::Mat> &imgs,
                  std::vector<int> &labels, char separator = ';');
};

#endif //_FACERECOGNIZER_H_
