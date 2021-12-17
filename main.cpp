#include "FaceRecognizer.h"

static void recognize(FaceRecognizer& recognizer, bool train_model = false)
{
    cv::Mat img;
    recognizer.loadDetectorFile("../models/lbpcascade_frontalface_improved.xml");
    if (train_model)
        recognizer.trainClassifier("../data", "../models/my_model.xml");
    recognizer.loadClassifierFile("../models/my_model.xml");
    if (recognizer.isReady())
    {
        while(true)
        {
            if(recognizer.takePhoto(img))
            {
                auto res = recognizer.recognize(img, true);
                cv::imshow("people", img);
                std::cout << "people = " << res << std::endl;
            }
            auto ch = cv::waitKey(1);
            if (ch == 'q')
            {
                break;
            }
        }
    }
    else
        std::cerr << "Recognizer is not ready" << std::endl;
}

static void take_photos(FaceRecognizer& recognizer, int face_id = 0)
{
    cv::Mat img;
    recognizer.loadDetectorFile("../models/lbpcascade_frontalface_improved.xml");
    if (recognizer.isReady())
    {
        while(true)
        {
            if(recognizer.takePhoto(img))
            {
                recognizer.saveFacePhotos(img, face_id);
                auto res = recognizer.recognize(img, true);
                cv::imshow("people", img);
                std::cout << "people = " << res << std::endl;
            }
            auto ch = cv::waitKey(0);
            if (ch == 'q')
            {
                break;
            }
        }
    }
    else
        std::cerr << "Recognizer is not ready" << std::endl;
}

int main()
{
    FaceRecognizer recognizer;

    return 0;
}
