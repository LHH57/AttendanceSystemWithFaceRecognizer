# 程序实现的功能

- 人脸识别
- 二叉排序树的插入、删除、查找、遍历
- 链表插入、删除、遍历
- 对顺序表的排序、查找
- 文件操作

---

# 程序结构规划

## 人员类（Person）

主要涉及链表操作和员工数据存储

### 属性

- 姓名
- 工号（或者学号等）
- 人脸信息
- 所属部门
- 。。。
- 考勤情况的链表

### 方法

- 添加考勤数据（打卡）
- 导出考勤数据（链表转化顺序表）
- 清空考勤数据（释放链表）

## 考勤类（Attendance）

主要涉及二叉树相关操作

### 属性

- 所有人员构成的二叉树
- 最少出勤要求（扩展）

### 方法

- 添加员工
- 删除员工
- 查找员工
- 将数据导出为顺序表
- 将数据存入文件
- 从文件中导入数据（扩展：可以选择只导入员工信息或者员工信息和旧考勤信息一同导入）

## 结果处理类（Filter）

主要是对于导出的顺序表进行排序、查找等。

### 属性

- 存储数据的顺序表（从Attendance类导出得到）

### 方法

- 按照指定字段排序
- 按照指定字段查找（筛选）