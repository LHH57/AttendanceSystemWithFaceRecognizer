#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <vector>
#include "windows.h"

class Person {
 public:
    struct PersonInfo {
        std::string name;
        std::string id_number; //员工的工号
        int identy; //表示员工所属部门
        bool state = false; //表示员工当前状态是上班还是下班

        int face_id; //记录员工的face编号用于人脸识别
    } info;
    struct CommuteData {
        SYSTEMTIME time;
        bool on_duty;   //用于记录是上班打卡还是下班打卡，true=上班
    };

 private:
    struct DataNode {
        CommuteData data;
        struct DataNode *next;
        explicit DataNode(bool now_state);
        explicit DataNode(CommuteData data);
    };
    using commute_record = DataNode *;

 private:
    commute_record records;
    int records_num;
 public:
    // 默认构造函数所返回的person对象只有一种含义：此人不存在。
    Person();
    Person(PersonInfo info);
    ~Person();

    // Person 类不允许复制
    Person(const Person &person) = delete;
    Person &operator=(const Person &person) = delete;

 public:
    void commute();
    void addCommuteData(CommuteData data);
    std::vector<CommuteData> getCommuteData();
    void clearCommuteData();
};

#endif // PERSON_H
