#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include "person.h"

class Attendance {
 public:
    struct PersonWithData {
        std::string name;
        std::string id_number; //员工的工号
        int identy = 0; //表示员工所属部门
        bool state = false; //表示员工当前状态是上班还是下班

        int face_id = -1;
        std::vector<Person::CommuteData> commute_data;
        int commute_times = 0;
        double commute_hours = 0;
        double avg_commute_hours = 0.0;

        PersonWithData() = default;
        PersonWithData(const Person::PersonInfo &info, std::vector<Person::CommuteData> data);
        static double getDiffHours(SYSTEMTIME a, SYSTEMTIME b);
    };

 private:
    struct TreeNode {
        Person person;
        struct TreeNode *left_child, *right_child;
        bool flag;
        explicit TreeNode(const Person::PersonInfo &_person);
        ~TreeNode() = default;
    };
    using node_ptr = TreeNode *;

 private:
    node_ptr pTree = nullptr;
    int person_num = 0;

 public:
    Person no_person{};

 public:
    Attendance() = default;
    ~Attendance();

    // Attendance 类不允许复制
    Attendance(const Attendance &attendance) = delete;
    Attendance &operator=(const Attendance &attendance) = delete;
 public:
    // 返回引用是load时为了添加info同时添加CommuteData。
    Person &addPerson(Person::PersonInfo person);
    Person &findPerson(int face_id);
    void destoryPersons();    // 后序遍历
    void clearPersonData();
    std::vector<PersonWithData> derivePersonData();    //中序遍历

    bool load(const std::string& path);    //用先序遍历构造树
    bool save(const std::string& path);    //先序遍历

 private:
    template<class VISIT_FUNC>
    void inorderVisit(VISIT_FUNC func)
    {
        auto stack = new node_ptr[person_num + 5];
        int b = 0;
        node_ptr p = pTree;
        while (b > 0 || p != nullptr)
        {
            if (p != nullptr)
            {
                stack[b++] = p;
                p = p->left_child;
            }
            else
            {
                p = stack[--b];
                func(p);
                p = p->right_child;
            }
        }
    }
};

#endif // ATTENDANCE_H
