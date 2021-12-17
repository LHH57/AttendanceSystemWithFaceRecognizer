#include "person.h"

#include <utility>
using namespace std;

Person::Person()
{
    info.identy = -1;
    records = nullptr;
    records_num = 0;
    info.state = false;
    info.face_id = -1;
}

Person::Person(Person::PersonInfo info)
    : info(std::move(info))
{
    records = nullptr;
    records_num = 0;
}

void Person::commute()
{
    commute_record p = records->next;
    records->next = new DataNode(info.state);
    records->next->next = p;
    ++records_num;
    info.state = !info.state;
}

vector<Person::CommuteData> Person::getCommuteData()
{
    vector<Person::CommuteData> datas(records_num);
    commute_record p = records->next;
    while (p != nullptr)
    {
        datas.push_back(p->data);
        p = p->next;
    }
    return datas;
}

void Person::clearCommuteData()
{
    commute_record q, p = records->next;
    while (p != nullptr)
    {
        q = p->next;
        delete p;
        p = q;
    }
    records_num = 0;
}
Person::~Person()
{
    clearCommuteData();
    delete records;
}
void Person::addCommuteData(CommuteData data)
{
    commute_record p = records->next;
    records->next = new DataNode(data);
    records->next->next = p;
    ++records_num;
    info.state = !info.state;
}

Person::DataNode::DataNode(bool now_state)
{
    GetSystemTime(&data.time);
    data.on_duty = !now_state;
    next = nullptr;
}

Person::DataNode::DataNode(Person::CommuteData data)
    : data(data)
{
    next = nullptr;
}
