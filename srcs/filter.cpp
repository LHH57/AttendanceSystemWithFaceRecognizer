#include "common.h"
#include "filter.h"

#include <stdexcept>
#include <sstream>


Filter::Filter()
    : str_need_refresh(false)
{

}

Filter::Filter(std::vector<Attendance::PersonWithData> &&datas)
    : persons_data(datas), str_need_refresh(true)
{
}

void Filter::load_data(std::vector<Attendance::PersonWithData> &&datas)
{
    str_need_refresh = true;
    persons_data = datas;
    search_data = {};
    filter_data = {};
}

const std::vector<Attendance::PersonWithData> &Filter::sort(int index)
{
    quick_sort(persons_data.data(), persons_data.size(), index);
    return persons_data;
}

const std::vector<Attendance::PersonWithData> &Filter::search(const std::string &target)
{
    if (str_need_refresh)
    {
        refresh_str();
        str_need_refresh = true;
    }
    auto target_indexes = BM(target, str_data);
    search_data.clear();
    search_data.reserve(target_indexes.size());
    for (auto& index: target_indexes)
    {
        auto pos1 = str_data.find("@index:", index) + strlen("@index:");
        auto pos2 = str_data.find('@', pos1);
        auto str_index = str_data.substr(pos1, pos2 - pos1);
        search_data.emplace_back(persons_data.at(stoi(str_index)));
    }
    return search_data;
}

bool
Filter::compare_less(const Attendance::PersonWithData &a, const Attendance::PersonWithData &b, int index, bool can_equal)
{
    switch (index)
    {
        case 1:return a.name < b.name || (can_equal && a.name == b.name);
        case 2:return a.id_number < b.id_number || (can_equal && a.id_number == b.id_number);
        case 3:return a.identy < b.identy || (can_equal && a.identy == b.identy);
        case 4:
            return a.commute_times < b.commute_times
                   || (can_equal && a.commute_times == b.commute_times);
        case 5:
            return a.commute_hours < b.commute_hours
                   || (can_equal && a.commute_hours == b.commute_hours);
        case 6:
            return a.avg_commute_hours < b.avg_commute_hours
                   || (can_equal && a.avg_commute_hours == b.avg_commute_hours);
        default:throw std::invalid_argument("index is not valid.");
    }
}
void Filter::refresh_str()
{
    std::stringstream ss;
    int index = -1;
    for (const auto &person: persons_data)
    {
        ss << "@name:" << person.name << "@id_number:" << person.id_number << "@identy:"
           << identities[person.identy] << "@state:" << person.state <<
           "@commute_times:" << person.commute_times << "@commute_hours:" << person.commute_hours
           << "@avg_commute_hours:" << person.avg_commute_hours <<
           "@index:" << std::to_string(++index);
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
        while (!compare_less(a[j], key, index, false) && i != j) --j;
        a[i] = a[j];
        while (compare_less(a[i], key, index, true) && i != j) ++i;
        a[j] = a[i];
    }
    a[j] = key;
    quick_sort(a, j, index);
    quick_sort(a + j + 1, n - j - 1, index);
}

std::vector<Filter::bm_size_type> Filter::preBmBc(const std::string &x)
{
    auto m = static_cast<Filter::bm_size_type>(x.size());
    std::vector<Filter::bm_size_type> bad_char_vec(CharSetSize, m);
    for (Filter::bm_size_type i = 0; i < m - 1; ++i)
    {
        bad_char_vec.at(x.at(i)) = m - i - 1;
    }
    return bad_char_vec;
}

std::vector<Filter::bm_size_type> Filter::suffixs(const std::string &x)
{
    auto m = static_cast<Filter::bm_size_type>(x.size());
    std::vector<Filter::bm_size_type> suff(m);
    suff.back() = m; //模式串末尾的前缀是整个模式串
    for (Filter::bm_size_type i = m - 2; i >= 0; --i)
    {
        auto q = i;
        while (q >= 0 && x[q] == x[m - 1 - i + q]) --q;
        suff[i] = i - q;
    }
    return suff;
}

std::vector<Filter::bm_size_type> Filter::preBmGs(const std::string &x)
{
    auto m = static_cast<Filter::bm_size_type>(x.size());
    auto suff = suffixs(x);
    std::vector<Filter::bm_size_type> good_string_vec(m, m);
    int j = 0;
    for (Filter::bm_size_type i = m - 1; i >= 0; --i)
        if (suff[i] == i + 1)
            for (; j < m - 1 - i; ++j)
                if (good_string_vec[j] == m)
                    good_string_vec[j] = m - 1 - i;
    for (Filter::bm_size_type i = 0; i <= m - 2; ++i)
        good_string_vec[m - 1 - suff[i]] = m - 1 - i;
    return good_string_vec;
}

std::vector<Filter::bm_size_type> Filter::BM(const std::string &sub_str, const std::string &str)
{
    auto m = static_cast<Filter::bm_size_type>(sub_str.size());
    auto n = static_cast<Filter::bm_size_type>(str.size());
    auto bmGs = preBmGs(sub_str);
    auto bmBc = preBmBc(sub_str);
    std::vector<Filter::bm_size_type> ans;
    Filter::bm_size_type i, j = 0;
    while (j <= n - m)
    {
        for (i = m - 1; i >= 0 && sub_str[i] == str[i + j]; --i);
        if (i < 0)
        {
            ans.push_back(j);
            j += bmGs[0];
        }
        else
            j += std::max(bmGs[i], bmBc[str[i + j]] - m + 1 + i);
    }
    return ans;
}
