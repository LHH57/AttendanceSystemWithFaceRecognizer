#ifndef FILTER_H
#define FILTER_H

#include "attendance.h"

class Filter {
 public:
    std::vector<Attendance::PersonWithData> persons_data;
 public:
    Filter();
    Filter(std::vector<Attendance::PersonWithData> datas);

    void load_data(std::vector<Attendance::PersonWithData> datas);
    void sort(int index);   //用归并排序或者快速排序
    void search(std::string target);    //字符串模式匹配算法（BM最好）

 private:
    std::string string_data;
};

#endif // FILTER_H
