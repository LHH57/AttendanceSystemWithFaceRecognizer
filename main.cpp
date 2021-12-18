#include "common.h"
#include "FaceRecognizer.h"
#include "attendance.h"
#include "filter.h"
#include <iostream>
using namespace std;

static void recognize(FaceRecognizer &recognizer, bool train_model = false)
{
    cv::Mat img;
    recognizer.loadDetectorFile("../models/lbpcascade_frontalface_improved.xml");
    if (train_model)
        recognizer.trainClassifier("../pics_data", "../models/my_model.xml");
    recognizer.loadClassifierFile("../models/my_model.xml");
    if (recognizer.isReady())
    {
        while (true)
        {
            if (recognizer.takePhoto(img))
            {
                auto res = recognizer.recognize(img, true);
                cv::imshow("people", img);
                if (res != -1) std::cout << "people = " << res << std::endl;
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

static void take_photos(FaceRecognizer &recognizer, int face_id = 0)
{
    cv::Mat img;
    recognizer.loadDetectorFile("../models/lbpcascade_frontalface_improved.xml");
    recognizer.loadClassifierFile("../models/my_model.xml");
    if (recognizer.isReady())
    {
        while (true)
        {
            if (recognizer.takePhoto(img))
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
    Attendance att;
    Person::PersonInfo info{"asdda", "1264842315643", 1, false};
    att.addPerson(info).commute();
    info = {"assdd", "984216841321654", 2, false};
    att.addPerson(info).commute();
    att.findPerson(1).commute();
    att.findPerson(2).commute();

    cout << "For Loop to see data:" << endl;
    auto pwds = att.derivePersonData();
    for (auto &pwd: pwds)
    {
        cout << "@name:" << pwd.name << "@id_number:" << pwd.id_number << "@identy:" << identities[pwd.identy] << "@state:" << pwd.state <<
                    "@commute_times:" << pwd.commute_times << "@commute_hours:" << pwd.commute_hours << "@avg_commute_hours:" << pwd.avg_commute_hours << endl;
    }
    att.save("../persons_data/data.dat");

    att.load("../persons_data/data.dat");
    cout << "For Loop to see data:" << endl;
    for (auto &pwd: pwds)
    {
        cout << "@name:" << pwd.name << "@id_number:" << pwd.id_number << "@identy:" << identities[pwd.identy] << "@state:" << pwd.state <<
             "@commute_times:" << pwd.commute_times << "@commute_hours:" << pwd.commute_hours << "@avg_commute_hours:" << pwd.avg_commute_hours << endl;
    }
    return 0;
}
