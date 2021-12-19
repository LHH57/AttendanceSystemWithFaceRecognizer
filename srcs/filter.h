#ifndef FILTER_H
#define FILTER_H

#include "attendance.h"

class Filter {
 private:
    using bm_size_type = long long int;
    static constexpr int CharSetSize = 256;

 private:
    std::vector<Attendance::PersonWithData> persons_data;
    std::vector<Attendance::PersonWithData> filter_data;
    std::vector<Attendance::PersonWithData> search_data;
    std::string str_data;
    bool str_need_refresh;

 public:
    Filter();
    explicit Filter(std::vector<Attendance::PersonWithData> &&datas);

    void load_data(std::vector<Attendance::PersonWithData> &&datas);

    const std::vector<Attendance::PersonWithData> &sort(int index);   //用归并排序或者快速排序
    const std::vector<Attendance::PersonWithData> &search(const std::string& target);    //字符串模式匹配算法（BM最好）
//    const std::vector<Attendance::PersonWithData>& filter()

    inline const std::vector<Attendance::PersonWithData> &getData()
    { return persons_data; }

    inline const std::vector<Attendance::PersonWithData> &getFilterData()
    { return filter_data; }

    inline const std::vector<Attendance::PersonWithData> &getSearchData()
    {return search_data;}

 private:
    static bool
    compare_less(const Attendance::PersonWithData &a, const Attendance::PersonWithData &b, int index, bool can_equal);
    void refresh_str();
    void quick_sort(Attendance::PersonWithData a[], unsigned long long int n, int index);


 private: //BM 算法的实现函数。
    static std::vector<Filter::bm_size_type> preBmBc(const std::string &x);
    static std::vector<Filter::bm_size_type> suffixs(const std::string &x);
    static std::vector<Filter::bm_size_type> preBmGs(const std::string &x);
    static std::vector<Filter::bm_size_type> BM(const std::string& sub_str, const std::string& str);
};

#endif // FILTER_H
