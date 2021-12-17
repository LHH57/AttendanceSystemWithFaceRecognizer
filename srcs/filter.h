#ifndef FILTER_H
#define FILTER_H

#include "attendance.h"

class Filter {
 private:
    std::vector<Attendance::PersonWithData> persons_data;
    std::vector<Attendance::PersonWithData> filter_data;
    std::string str_data;
    bool str_need_refresh;

 public:
    Filter();
    explicit Filter(std::vector<Attendance::PersonWithData>&& datas);

    void load_data(std::vector<Attendance::PersonWithData> &&datas);

    const std::vector<Attendance::PersonWithData>& sort(int index);   //用归并排序或者快速排序
    Attendance::PersonWithData search(std::string target);    //字符串模式匹配算法（BM最好）
//    const std::vector<Attendance::PersonWithData>& filter()

    const std::vector<Attendance::PersonWithData>& getData();
    const std::vector<Attendance::PersonWithData>& getFilterData();

 private:
    static bool compare_lessequal(const Attendance::PersonWithData& a, const Attendance::PersonWithData& b, int index);
    void refresh_str();
    void quick_sort(Attendance::PersonWithData a[], unsigned long long int n, int index);
};

#endif // FILTER_H
