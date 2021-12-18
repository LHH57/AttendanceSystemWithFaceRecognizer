#include "person.h"

#include <fstream>
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
    records = new DataNode(false);
    records_num = 0;
}

Person::~Person()
{
    if (records != nullptr)
    {
        clearCommuteData();
        delete records;
    }
}

void Person::PersonInfo::save(ofstream& f)
{
    auto size = name.size();
    f.write((char*)&size, sizeof(size));
    f.write(name.c_str(), size * sizeof(name.front()));
    size = id_number.size();
    f.write((char*)&size, sizeof(size));
    f.write(id_number.c_str(), size * sizeof(id_number.front()));
    f.write((char*)&identy, sizeof(identy));
    f.write((char*)&state, sizeof(state));
    f.write((char*)&face_id, sizeof(face_id));
}

void Person::PersonInfo::load(ifstream& f)
{
    auto size = name.size();

    f.read((char*)&size, sizeof(size));
    name.resize(size);
    f.read(name.data(), size * sizeof(name.front()));
    f.read((char*)&size, sizeof(size));
    id_number.resize(size);
    f.read(id_number.data(), size * sizeof(id_number.front()));
    f.read((char*)&identy, sizeof(identy));
    f.read((char*)&state, sizeof(state));
    f.read((char*)&face_id, sizeof(face_id));
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
    vector<Person::CommuteData> datas;
    datas.reserve(records_num);
    commute_record p = records->next;
    while (p != nullptr)
    {
        datas.emplace_back(p->data);
        p = p->next;
    }
    return datas;
}

void Person::clearCommuteData()
{
    commute_record p;
    while (records->next != nullptr)
    {
        p = records->next->next;
        delete records->next;
        records->next = p;
    }
    records_num = 0;
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
