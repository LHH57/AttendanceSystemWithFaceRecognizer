#include "filter.h"
#include "common.h"

#include <stdexcept>
#include <sstream>

Filter::Filter()
    :str_need_refresh(false)
{

}

Filter::Filter(std::vector<Attendance::PersonWithData>&& datas)
    :persons_data(datas), str_need_refresh(true)
{
}

void Filter::load_data(std::vector<Attendance::PersonWithData> &&datas)
{
    str_need_refresh = true;
    persons_data = datas;
}

inline const std::vector<Attendance::PersonWithData> &Filter::getData()
{
    return persons_data;
}

const std::vector<Attendance::PersonWithData> &Filter::getFilterData()
{
    return filter_data;
}

const std::vector<Attendance::PersonWithData> &Filter::sort(int index)
{
    quick_sort(persons_data.data(), persons_data.size(), index);
    return persons_data;
}

Attendance::PersonWithData Filter::search(std::string target)
{
    if (str_need_refresh)
    {
        refresh_str();
        str_need_refresh = true;
    }

    return {};
}

bool
Filter::compare_lessequal(const Attendance::PersonWithData &a, const Attendance::PersonWithData &b, int index)
{
    switch (index)
    {
        case 1:
            return a.name <= b.name;
        case 2:
            return a.id_number <= b.id_number;
        case 3:
            return a.identy <= b.identy;
        case 4:
            return a.commute_times <= b.commute_times;
        case 5:
            return a.commute_hours <= b.commute_hours;
        case 6:
            return a.avg_commute_hours <= b.avg_commute_hours;
        default:
            throw std::invalid_argument("index is not valid.");
    }
}
void Filter::refresh_str()
{
    std::stringstream ss;
    for (const auto& person: persons_data)
    {
        ss << "@name:" << person.name << "@id_number:" << person.id_number << "@identy:" << identities[person.identy] << "@state:" << person.state <<
        "@commute_times:" << person.commute_times << "@commute_hours:" << person.commute_hours << "@avg_commute_hours:" << person.avg_commute_hours;
    }
    str_data = ss.str();
}

void Filter::quick_sort(Attendance::PersonWithData *a, unsigned long long int n, int index)
{
    if (n <= 1)
        return;
    Attendance::PersonWithData key = a[0];
    int i = 0;
    auto j = n - 1;
    while (i != j)
    {
        while (!compare_lessequal(a[i], key, index) && i != j) --j;
        a[i] = a[j];
        while (compare_lessequal(a[i], key, index) && i != j) ++i;
        a[j] = a[i];
    }
    a[j] = key;
    quick_sort(a, j, index);
    quick_sort(a + j + 1, n - j - 1, index);
}
