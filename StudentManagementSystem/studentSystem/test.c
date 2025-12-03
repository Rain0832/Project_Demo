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
}Student;

// 插入数据函数
const char* student_toSql(Student* stu)
{
	static char buf[BUFSIZ] = {0};
	sqlite3_snprintf(BUFSIZ, buf, "INSERT INTO students(id, name, dept, chinese, math, english)\
		VALUES(%d, '%s', '%s', %lf, %lf, %lf)", stu->id, stu->name, stu->dept, stu->chinese, stu->math, stu->english);
	return buf;
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
int printTable(void* data, int colums, char** values, char** fileds)
{
	// 自定义数据
	printf("data: %d\n", *(int*)data);

	// 打印表头
	for (size_t i = 0; i < colums; i++)
	{
		printf("%s\t", fileds[i]);
	}
	printf("\n");

	// 打印数据
	for (size_t i = 0; i < colums; i++)
	{
		printf("%s\t", values[i]);
	}

	printf("\n");
	return SQLITE_OK;
}


int main()
{
	int rc = SQLITE_ERROR;		// 记录返回值，判断操作是否成功

//----------------------------------------------------------------------------------------------
	// 1 打开数据库
	// 取数据库 传地址
	sqlite3* db = NULL;
	rc = sqlite3_open("student.db", &db);
	if (rc != SQLITE_OK)
	{
		sqlite3_log(sqlite3_errmsg(db), "open failed\n");
	}
	else
	{
		printf("打开数据库成功\n");
	}
//----------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------
	// 2 创建数据表
	/*
	*	|id|name|dept|chinese|math|english|	// 表头 - 字段构成
	*	|100|张三|计算机|80|90|85|			// 一条记录 - 字段值
	*/
	rc = sqlite3_exec(db, "CREATE TABLE students(id INT PRIMARY KEY, name VARCHAR(30), \
dept VARCHAR(30), chinese REAL, math REAL, english REAL)", NULL, NULL, NULL);
	if (rc == SQLITE_OK)
	{
		printf("创建 students 表成功\n");
	}
	else
	{
		printf("创建 students 表失败: %s\n", sqlite3_errmsg(db));
	}
//----------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------
	// 3 插入数据

	/*
	// 3.0 插入一条测试数据
	rc = sqlite3_exec(db, "INSERT INTO students(id, name, dept, chinese, math, english) VALUES\
						(100, '张三', '计算机', 80, 90, 85)\
						,(101, '李四', '汉语言', 70, 80, 75)", NULL, NULL, NULL);
	//if (rc == SQLITE_ERROR)
	if (rc != SQLITE_OK)
	{
		printf("插入失败: %s\n", sqlite3_errmsg(db));
	}
	*/

	// 3.1 让用户输入之后插入数据
	printf("请输入学生信息（%s）\n", student_header());
	Student stu;
	scanf("%d %s %s %lf %lf %lf", &stu.id, stu.name, stu.dept, &stu.chinese, &stu.math, &stu.english);
	student_toSql(&stu);
	rc = sqlite3_exec(db, student_toSql(&stu), NULL, NULL, NULL);
	if (rc != SQLITE_OK)
	{
		printf("插入失败: %s\n", sqlite3_errmsg(db));
	}
	else
	{
		printf("插入成功\n");
	}

	// 4 查询数据
	// 每查询到一条记录，调用回调函数 printTable
	/*
	int number = 222; // 测试代码
	rc = sqlite3_exec(db, "SELECT * FROM students", printTable, &number, NULL);

	//rc = sqlite3_exec(db, "SELECT * FROM students", printTable, NULL, NULL);
	if (rc != SQLITE_OK)
	{
		printf("查询失败: %s\n", sqlite3_errmsg(db));
	}
	else
	{
		printf("查询成功\n");
	}
	*/


	// 5 修改数据
	/*
	rc = sqlite3_exec(db, "UPDATE students SET name = '王五' WHERE id = 101",NULL, NULL, NULL);
	if (rc != SQLITE_OK)
	{
		printf("修改失败: %s\n", sqlite3_errmsg(db));
	}
	else
	{
		printf("修改成功\n");
	}
	*/



	// 6 删除数据
	/*
	rc = sqlite3_exec(db, "DELETE FROM students WHERE id = 101", NULL, NULL, NULL);
	if (rc != SQLITE_OK)
	{
		printf("删除失败: %s\n", sqlite3_errmsg(db));
	}
	else
	{
		printf("删除成功\n");
	}
	*/
	

	// 7 再次输出数据
	char** result = NULL;
	int rowCount, columnCount;
	char* errMsg;
	rc = sqlite3_get_table(db,				// 连接句柄
		"SELECT * FROM students",		// SQL语句
		&result,						// 结果集（包含字段和值）
		&rowCount,							//  &column, &errmsg);
		&columnCount,
		&errMsg
	);

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
				printf("%s\t", result[r*columnCount + c]);
			}
			printf("\n");
		}
	}

	// 关闭数据库
	sqlite3_close(db);

	system("pause");
	return 0;
}