# 学生管理系统 C + SQLite

# 1. 什么是 SQLite？

SQLite 是一个软件库，实现了自给自足的，无服务器的、零配置的、事务性的 SQL 数据库引擎。

SQL 源码不受版权控制。

不需要在系统中配置。可以通过应用程式需求进行静态或动态链接。

### 1.1. 为什么要用 SQLite？

- 无需配置，轻量级...
- 提供 ANSI-C 编写的简单和易用的 API。

### 1.2. 预备 SQLite

下载、安装、配置 Path...而且如果想在 C 中调用包，需要再下载一个 Source Code 文件。

# 2. 学生管理系统设计

利用 SQLite 进行简单的数据交互啊

提供简单的增删查改的功能

做完了发现其实一些语法和 MySQL 是差不多的，主要学习如何与 C/Cpp 进行结合。

### 2.1. 准备

首先要明确一般的命令行工具：MySQL之类的指令是只能在命令行运行...

但是这里我们需要在 C 源文件中调包的，所以这里可以提前下载好对应的包，放到项目文件夹中。

引入`#include "sqlite3/sqlite3.h"`

### 2.2. 数据设计

准备好我们需要的数据，包括 DataBase 的字段以及数据存储结构体

```c
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
```

数据库表头和示例记录

```plain
*   |id|name|dept|chinese|math|english|    // 表头 - 字段构成
*   |100|张三|计算机|80|90|85|            // 一条记录 - 字段值
```

### 2.3. 打开数据库

首先定义一个返回值的数据

```
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
```

如果没有数据库的话会先创建一个数据库

### 2.4. 创建数据表

```c
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
```

### 2.5. 进入选项界面

这里不加演示了，有点小丑...需要完整代码可以查看 Github

### 2.6. 插入数据

```c
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
```

其中的封装函数如下...

```c
// 插入数据
const char* student_toSql(Student* stu)
{
    static char buf[BUFSIZ] = { 0 };
    sqlite3_snprintf(BUFSIZ, buf, "INSERT INTO students(id, name, dept, chinese, math, english) VALUES(%d, '%s', '%s', %lf, %lf, %lf)", stu->id, stu->name, stu->dept, stu->chinese, stu->math, stu->english);
    return buf;
}
```

### 2.7. 删除数据

```c
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
```

下面还是封装的函数

```c
// 删除数据
const char* student_deleteSql(int id, sqlite3* db)
{
    static char sql[BUFSIZ] = { 0 };
    sqlite3_snprintf(BUFSIZ, sql, "DELETE FROM students WHERE id = %d", id);
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    return sql;
}
```

### 2.8. 修改数据

```c
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
```

### 2.9. 查询数据

```c
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
```

封装函数

```c
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
```

### 2.10. 关闭数据库

```c
// 关闭数据库
sqlite3_close(db);
return 0;
```

# 3. 总结

SQLite 和 MySQL 本质上都是 SQL 语言吧...和这学期的数据库技术要学的内容也大差不差，只不过可能一个偏理论一个偏应用之类的。

总的来说通过这个联系还是复习了一下吧，虽然寒假学过一些基础，但是半个月一个月过去还是忘得差不多了...

还有就是其实大部分的内容都是面向过程的编写，但以后实际上大部分都是需要封装的...这一部分我还是要多加练习的。关于传入哪些参数，传出哪些参数，不会改变代码的逻辑功能，还是需要综合考量...