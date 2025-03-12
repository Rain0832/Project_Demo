#include <stdio.h>
#include "sqlite3/sqlite3.h"

// 学生结构体
typedef struct Student
{
    int id;
    char name[30];
    char dept[30];
    double chinese;
    double math;
    double english;
} Student;

// 插入数据
const char* student_toSql(Student* stu)
{
    static char buf[BUFSIZ] = { 0 };
    sqlite3_snprintf(BUFSIZ, buf, "INSERT INTO students(id, name, dept, chinese, math, english) VALUES(%d, '%s', '%s', %lf, %lf, %lf)", stu->id, stu->name, stu->dept, stu->chinese, stu->math, stu->english);
    return buf;
}

// 删除数据
const char* student_deleteSql(int id, sqlite3* db)
{
    static char sql[BUFSIZ] = { 0 };
    sqlite3_snprintf(BUFSIZ, sql, "DELETE FROM students WHERE id = %d", id);
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    return sql;
}


// 打印表头
const char* student_header()
{
    return "学号、姓名、学院、语文成绩、数学成绩、英语成绩";
}

// 函数指针
// int (*callback)(void*, int, char**, char**);
/*
* @param data: 用户自定义数据，也就是 sqlite3_exec 的第三个参数
* @param colums: 查询到的列数（字段数量）
* @param values: char* 一维数组，存储的是每个字段对应的值（一条记录）
* @param fileds: char* 一维数组，所有字段名
*/
int printTable(void* data, int columns, char** values, char** fields)
{
    static int isHeaderPrinted = 0;
    if (!isHeaderPrinted)
    {
        // 打印表头
        for (size_t i = 0; i < columns; i++)
        {
            printf("%s\t", fields[i]);
        }
        printf("\n");
        isHeaderPrinted = 1;
    }

    // 打印数据
    for (size_t i = 0; i < columns; i++)
    {
        printf("%s\t", values[i]);
    }
    printf("\n");
    return SQLITE_OK;
}

void showMenu()
{
    printf("学生管理系统\n");
    printf("1. 输入学生信息\n");
    printf("2. 显示学生信息\n");
    printf("3. 修改学生信息\n");
    printf("4. 删除学生信息\n");
    printf("5. 退出系统\n");
    printf("请输入选项：");
}

int main(void)
{
    int rc = SQLITE_ERROR;  // 记录返回值，判断操作是否成功

    // 1 打开数据库
    // 取数据库 传地址
    sqlite3* db = NULL;
    rc = sqlite3_open("student.db", &db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "打开数据库失败: %s\n", sqlite3_errmsg(db));
        return 1;
    }
    else
    {
        printf("打开数据库成功\n");
    }

    // 2 创建数据表
    /*
    *   |id|name|dept|chinese|math|english|    // 表头 - 字段构成
    *   |100|张三|计算机|80|90|85|            // 一条记录 - 字段值
    */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS students(id INT PRIMARY KEY, name VARCHAR(30), dept VARCHAR(30), chinese REAL, math REAL, english REAL)", NULL, NULL, NULL);
    if (rc == SQLITE_OK)
    {
        printf("创建 students 表成功\n");
    }
    else
    {
        printf("创建 students 表失败: %s\n", sqlite3_errmsg(db));
    }

    while (1)
    {
        int choice;
        showMenu();
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
        {
            //  输入数据
            printf("请输入学生信息（%s）\n", student_header());
            Student stu;
            scanf("%d %s %s %lf %lf %lf", &stu.id, stu.name, stu.dept, &stu.chinese, &stu.math, &stu.english);
            rc = sqlite3_exec(db, student_toSql(&stu), NULL, NULL, NULL);
            if (rc != SQLITE_OK)
            {
                printf("插入失败: %s\n", sqlite3_errmsg(db));
            }
            else
            {
                printf("插入成功\n");
            }
        }
        break;
        case 2:
        {
            //  输出数据
            char** result = NULL;
            int rowCount, columnCount;
            char* errMsg;
            rc = sqlite3_get_table(db, "SELECT * FROM students", &result, &rowCount, &columnCount, &errMsg);
            if (rc != SQLITE_OK)
            {
                printf("查询失败: %s\n", sqlite3_errmsg(db));
            }
            else
            {
                for (size_t r = 0; r <= rowCount; r++)
                {
                    for (size_t c = 0; c < columnCount; c++)
                    {
                        printf("%s\t", result[r * columnCount + c]);
                    }
                    printf("\n");
                }
                // 释放结果集
                sqlite3_free_table(result);
            }
        }
        break;
        case 3:
        {
            int id;
            printf("请输入要修改的学生学号: ");
            scanf("%d", &id);
            Student stu;
            printf("请输入新的学生信息（%s）\n", student_header());
            scanf("%d %s %s %lf %lf %lf", &stu.id, stu.name, stu.dept, &stu.chinese, &stu.math, &stu.english);
            char sql[BUFSIZ];
            sqlite3_snprintf(BUFSIZ, sql, "UPDATE students SET id = %d, name = '%s', dept = '%s', chinese = %lf, math = %lf, english = %lf WHERE id = %d", stu.id, stu.name, stu.dept, stu.chinese, stu.math, stu.english, id);
            rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
            if (rc != SQLITE_OK)
            {
                printf("修改失败: %s\n", sqlite3_errmsg(db));
            }
            else
            {
                printf("修改成功\n");
            }
        }
        break;
        case 4:
        {
            printf("请输入要删除的学生学号: ");
            int id;
            scanf("%d", &id);
            rc = sqlite3_exec(db, student_deleteSql(id, db), NULL, NULL, NULL);

            if (rc != SQLITE_OK)
            {
                printf("删除失败: %s\n", sqlite3_errmsg(db));
            }
            else
            {
                printf("删除成功\n");
            }
        }
        break;
        case 5:
            // 关闭数据库
            sqlite3_close(db);
            return 0;
        default:
            printf("输入错误，请重新输入！\n");
            break;
        }
        system("pause");
        system("cls");
    }
    return 0;
}