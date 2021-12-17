#include "attendance.h"
#include <fstream>
#include <utility>
using namespace std;

Attendance::TreeNode::TreeNode(Person::PersonInfo _person)
    : person(std::move(_person))
{
    left_child = nullptr;
    right_child = nullptr;
    flag = false;
}

Attendance::PersonWithData::PersonWithData(const Person::PersonInfo &info, std::vector<Person::CommuteData> data)
    : commute_data(std::move(data))
{
    name = info.name;
    id_number = info.id_number;
    identy = info.identy;
    face_id = info.face_id;
    state = info.state;
}

Person &Attendance::addPerson(const Person::PersonInfo &person)
{
    if (pTree == nullptr)
    {
        pTree = new TreeNode(person);
        return pTree->person;
    }
    else
    {
        node_ptr p = pTree;
        while (true)
        {
            if (person.face_id > p->person.info.face_id)
            {
                if (p->right_child != nullptr)
                    p = p->right_child;
                else
                {
                    p->right_child = new TreeNode(person);
                    p = p->right_child;
                    ++person_num;
                    break;
                }
            }
            else
            {
                if (p->left_child != nullptr)
                    p = p->left_child;
                else
                {
                    p->left_child = new TreeNode(person);
                    ++person_num;
                    p = p->left_child;
                    break;
                }
            }
        }
        return p->person;
    }
}

Person &Attendance::findPerson(int face_id)
{
    node_ptr p = pTree;
    while (p != nullptr)
    {
        if (face_id > p->person.info.face_id)
            p = p->right_child;
        else if (face_id < p->person.info.face_id)
            p = p->left_child;
        else
            return p->person;
    }
    return no_person;
}

void Attendance::destoryPersons()
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
            if (!p->flag)
            {
                stack[b++] = p;
                p->flag = true;
                p = p->right_child;
            }
            else
            {
                delete p;
                p = nullptr;
            }
        }
    }
    person_num = 0;
}

vector<Attendance::PersonWithData> &&Attendance::derivePersonData()
{
    vector<PersonWithData> data(person_num);
    inorderVisit([&](node_ptr &p)
                 {
                     data.emplace_back(p->person.info, p->person.getCommuteData());
                 });
    return std::move(data);
}

bool Attendance::save(string path)
{
    ofstream f(path, ofstream::out | ofstream::binary);
    if (!f.is_open()) return false;
    inorderVisit([&](node_ptr &p)
                 {
                     auto commute_data = p->person.getCommuteData();
                     auto size = commute_data.size();
                     f.write((char *) &p->person.info, sizeof(p->person.info));
                     f.write((char *) &size, sizeof(size));
                     f.write((char *) commute_data.data(), sizeof(Person::CommuteData) * size);
                 });
    f.close();
    return true;
}

bool Attendance::load(string path)
{
    ifstream f(path, ifstream::in | ifstream::binary);
    if (!f.is_open()) return false;
    f.peek();
    Person::PersonInfo info;
    Person::CommuteData tmp{};
    unsigned long long int size;
    while (!f.eof())
    {
        f.read((char *) &info, sizeof(info));
        auto &p = addPerson(info);
        f.read((char *) &size, sizeof(size));
        for (int i = 0; i < size; ++i)
        {
            f.read((char *) &tmp, sizeof(tmp));
            p.addCommuteData(tmp);
        }
        f.peek();
    }
    return true;
}

void Attendance::clearPersonData()
{
    inorderVisit([=](node_ptr &p)
                 {
                     p->person.clearCommuteData();
                 });
}

Attendance::~Attendance()
{
    destoryPersons();
}
