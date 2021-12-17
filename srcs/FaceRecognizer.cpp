#include "FaceRecognizer.h"
#include <fstream>
#include <vector>
#include <cstdlib>
#include <direct.h>

FaceRecognizer::FaceRecognizer()
    : cap(0), ptr(FaceRecognizerType::create())
{

}

FaceRecognizer::FaceRecognizer(const std::string &detector_data_file, const std::string &classifier_file_file)
    : cap(0), ptr(FaceRecognizerType::create())
{
    cas.load(detector_data_file);
    ptr->read(classifier_file_file);
}

bool FaceRecognizer::loadDetectorFile(const std::string &detector_data_file)
{
    return cas.load(detector_data_file);
}

void FaceRecognizer::loadClassifierFile(const std::string &classifier_data_file)
{
    ptr->read(classifier_data_file);
}

bool FaceRecognizer::isReady()
{
    return cap.isOpened() && !cas.empty() && !ptr->empty();
}

cv::Mat FaceRecognizer::getFace(cv::Mat &img, bool draw_faces)
{
    cv::Mat gray_img, face;
    std::vector<cv::Rect> rects;
    cv::cvtColor(img, gray_img, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray_img, gray_img);
    cas.detectMultiScale(gray_img, rects, 1.1, 3, cv::CASCADE_DO_CANNY_PRUNING);
    if (rects.size() == 1)
    {
        constexpr double width_magnification = 0.25;
        constexpr double height_magnification = 0.45;
        cv::Rect rect{    //以矩形中心点为中心，将矩形的宽放大25%，高放大45%。
            rects.begin()->x - static_cast<int>(rects.begin()->width * width_magnification / 2),
            rects.begin()->y - static_cast<int>(rects.begin()->height * height_magnification / 2),
            static_cast<int>(rects.begin()->width * (1 + width_magnification)),
            static_cast<int>(rects.begin()->height * (1 + height_magnification))
        };
        // 判断矩形是否越界。
        if (0 <= rect.tl().x && 0 <= rect.tl().y
            && rect.br().x <= img.cols && rect.br().y <= img.rows)
        {
            if (draw_faces)
                cv::rectangle(img, rect, cv::Scalar(0, 255, 0), 2);
            cv::resize(gray_img(rect), face, cv::Size(Face_ROI_Region_Width, Face_ROI_Region_Height));
            return face;
        }
    }
    else
    {
        if (draw_faces)
            for (auto &&rect: rects)
                cv::rectangle(img, *rects.begin(), cv::Scalar(0, 0, 255), 2);
    }
    return {};
}

std::pair<int, double> FaceRecognizer::predictFace(const cv::Mat &roiImg)
{
    if (roiImg.empty())
        return {-1, 0};
    int label;
    double confidence;
    ptr->predict(roiImg, label, confidence);
    return {label, confidence};
}

int FaceRecognizer::recognize(cv::Mat &img, bool draw_res)
{
    auto face = getFace(img, draw_res);
    auto[pred, confid] = predictFace(face);
    return confid < Admissible_Confidence ? pred : -1;
}

void
FaceRecognizer::trainClassifier(std::string &&pics_csv_path, const std::string &classifier_data_file)
{
    if (pics_csv_path.back() == '/')
        pics_csv_path.pop_back();
    system(("python3 " + pics_csv_path + "/make_csv.py").c_str());
    std::vector<cv::Mat> imgs;
    std::vector<int> labels;
    read_csv_file(pics_csv_path + "/pics.csv", imgs, labels);
    ptr->train(imgs, labels);
    ptr->save(classifier_data_file);
}

void
FaceRecognizer::read_csv_file(const std::string &file_name, std::vector<cv::Mat> &imgs,
                              std::vector<int> &labels, char separator)
{
    std::ifstream file(file_name, std::ifstream::in);
    if (!file.is_open())
    {
        std::string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(cv::Error::StsError, error_message);
    }

    std::string line, path, classlabel;

    while (getline(file, line))//从文本文件中读取一行字符，未指定限定符默认限定符为“/n”
    {
        std::stringstream liness(line);//这里采用stringstream主要作用是做字符串的分割
        getline(liness, path, separator);//读入图片文件路径以分好作为限定符
        getline(liness, classlabel);//读入图片标签，默认限定符
        if (!path.empty() && !classlabel.empty())//如果读取成功，则将图片和对应标签压入对应容器中
        {
            imgs.push_back(cv::imread(path, 0));
            labels.push_back(std::stoi(classlabel));
        }
    }
}

void FaceRecognizer::saveFacePhotos(cv::Mat &img, int face_id)
{
    static int last_face_id = Inital_Face_Number, cnt = 0;
    if (face_id == 0)
    {
        cnt = 0;
        face_id = ++last_face_id;
    }
    else if (face_id != last_face_id)
    {
        cnt = 0;
        last_face_id = face_id;
    }
    auto path = Data_Folder_Path + std::to_string(face_id);
    if (0 != access(path.c_str(), 0) && 0 != mkdir(path.c_str()))
        throw std::invalid_argument("Data path has not existed but create failed! Please check whether face id is valid.");
    auto face = getFace(img);
    if(!face.empty())
    {
        auto file = path + "/" + std::to_string(cnt++) + Pic_File_Format;
        cv::imwrite(file, face);
        std::clog << "save file at" << file << std::endl;
    }
}

bool FaceRecognizer::takePhoto(cv::Mat &img)
{
    return cap.isOpened() && cap.read(img);
}
