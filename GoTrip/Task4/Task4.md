你的理解 **非常准确**，尤其是这两点：

* **切片≈C++ vector（但更轻、更引用语义）**
* **append 触发扩容 → 可能换底层数组 → 原切片不再共享**

你现在已经拥有后续 Web 项目数据结构所需要的全部切片基础。
优秀进度，继续保持！

---

# **Level 4：map 与 struct（Go 数据建模的核心）**

预计本关 15 分钟内完成。
这一关结束后，你将能写出：

* Web 返回 JSON 的数据模型
* 内存中的“小数据库”
* REST API 里常见的 request/response 结构体

这也是做 Web 项目的基础。

---

# 🎯 **Level 4 的学习重点（5 个）**

1. map 的声明、增删查改
2. struct 的定义与初始化方式
3. struct 的“方法”（Go 没有 class）
4. 值类型 vs 引用类型（struct 是值类型）
5. map + struct 组合成小型数据存储

---

# 🧠 **最白话解释**

## **1. map = Go 的哈希表（字典）**

和 Python dict、C++ unordered_map 类似。

声明方式有两种：

```go
m := map[string]int{}
m := make(map[string]int)
```

map 是**引用类型**：
复制 map 变量不会拷贝数据。

---

## **2. struct = Go 的“对象模板”**

但 Go 没有 class，没有继承。

结构体声明：

```go
type User struct {
    Name string
    Age  int
}
```

初始化有三种方式：

```go
u := User{"Tom", 20}
u := User{Name: "Tom", Age: 20}
u := &User{Name: "Tom"}   // 返回指针
```

---

## **3. struct 方法 = 类似 C++ 的成员函数**

但 Go 把“类型”和“方法”分开写。

比如：

```go
func (u *User) SayHi() {
    fmt.Println("Hi, I'm", u.Name)
}
```

注意：要修改 struct 字段，接收者必须是 `*User`（指针）。

---

## **4. struct 是“值类型”**

当你写：

```go
u2 := u1
```

这是深拷贝（字段值被复制一份）。

但 map 是引用类型：

```go
m2 := m1 // 引用复制，不拷贝数据
```

---

## **5. map + struct = 你第一版“数据库模型”**

Web 项目里常见用法：

```go
var users = map[int]User{}
```

这会成为你下一关写的 mini-API 的基础。

---

# 📚 **本关语法/API 名字**

你只需知道：

* map 声明：`map[K]V`
* make：`make(map[K]V)`
* delete：`delete(map, key)`
* struct 定义：`type X struct{}`
* struct 方法：`func (x *X) Foo() {}`
* “点号访问”：`u.Name`

---

# 📝 **你的任务（10–15 分钟）**

创建目录：

```
GoTrip/Task4
```

编写 `main.go`。

---

## **任务 1：练习 map 的 CRUD**

1. 创建一个 map：

```go
scores := make(map[string]int)
```

2. 添加三条记录（随便写名字和分数）
3. 读取一个 key
4. 修改一个 value
5. 删除一个 key
6. 用 for range 遍历整个 map（顺序不保证）

---

## **任务 2：定义 struct 并初始化**

创建一个 struct：

```go
type User struct {
    Name string
    Age  int
}
```

在 main 中：

* 用三种方式各创建一个 User
* 打印它们

---

## **任务 3：为 struct 定义一个方法**

写一个方法：

```go
func (u User) SayHi()
```

功能：打印用户的打招呼信息。

然后在 main 中调用。

---

## **任务 4：把 struct 装进 map**

创建：

```go
users := make(map[int]User)
```

放入若干 User（key 用 int ID，例如 1,2,3）

遍历打印所有用户。

---

# 🔍 **自查问题**

1. 为什么 struct 是值类型，而 map 是引用类型？
2. 给 struct 写方法时，什么时候用值接收者？什么时候用指针接收者？
3. map 的遍历顺序为什么不保证？

---

完成后告诉我：

**“Level 4 OK，进入 Level 5”**

Level 5 将是：
**JSON 编解码（encoding/json）—— Web 后端最核心的能力之一。**
