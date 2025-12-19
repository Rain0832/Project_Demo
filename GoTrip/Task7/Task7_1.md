好，**Level 7 正式开始**。
从这一关起，你已经在做**真正的 Go 后端**了，而不是“示例代码”。

我会 **一步一步、每一步都能跑**，而且会**主动帮你复习前面学过的内容**，不会让你“学了就忘”。

---

# 🚀 Level 7：返回 JSON 的 Web API（真正的 Go 后端起点）

> 目标：
> **浏览器 / curl 访问接口 → 拿到 JSON 数据**

预计时间：**20 分钟左右**

---

## 🎯 Level 7 你将学到的 5 个重点

1. 一个 Web API 的完整链路（请求 → handler → JSON → 响应）
2. 在 handler 中返回 **JSON 而不是字符串**
3. 多路由设计（`/hello`、`/users`）
4. 把 **struct + slice + json + http** 串成一条线
5. 为什么 Web API 一般“只返回数据，不打印东西”

---

## 🧠 先建立「API 心智模型」（非常重要）

### 到现在为止你已经会了什么？

* struct
* slice
* json.Marshal
* http handler
* 浏览器访问 localhost

### 那一个 **Web API** 是什么？

一句话：

> **Web API = 浏览器/客户端请求一个 URL，服务器返回 JSON 数据**

不是 HTML，不是 fmt.Println。

---

## 🧩 一个典型 Go API 的执行流程

```
浏览器访问 /users
      ↓
net/http 接收到请求
      ↓
匹配路由 /users
      ↓
调用 usersHandler
      ↓
构造 Go struct / slice
      ↓
json.Marshal
      ↓
写入 ResponseWriter
      ↓
浏览器看到 JSON
```

---

## 📚 本关会用到的 API（你已全部学过）

* `net/http`
* `encoding/json`
* `http.HandleFunc`
* `http.ResponseWriter`
* `json.Marshal`
* struct + slice

**没有新语法，只有“组合使用”**

---

# 📝 你的任务（分 3 步，稳扎稳打）

创建目录：

```
GoTrip/Task7
```

---

## ✅ Step 1：准备数据模型（复习 Level 4 + 5）

你需要定义一个 struct（你来写）：

```go
type User struct {
    ID   int    `json:"id"`
    Name string `json:"name"`
    Age  int    `json:"age"`
}
```

然后在**全局作用域**准备一个 slice，当作“内存数据库”：

```go
var users = []User{
    {ID: 1, Name: "Tom", Age: 20},
    {ID: 2, Name: "Amy", Age: 22},
}
```

🧠 为什么放在全局？

> 因为 handler 每次都会被调用，需要共享数据。

---

## ✅ Step 2：写第一个 JSON API（/users）

### 目标行为

访问：

```
http://localhost:8080/users
```

浏览器看到类似：

```json
[
  {"id":1,"name":"Tom","age":20},
  {"id":2,"name":"Amy","age":22}
]
```

---

### usersHandler 的逻辑（你来写）

你需要在 handler 中完成：

1. `json.Marshal(users)`
2. 如果 err != nil → 返回 500
3. 设置响应头：

```go
w.Header().Set("Content-Type", "application/json")
```

4. 把 JSON 写给浏览器：

```go
w.Write(jsonBytes)
```

---

## ✅ Step 3：再加一个简单路由（/hello）

路径：

```
/hello
```

返回：

```json
{"message":"hello go"}
```

你可以：

* 定义一个匿名 struct
* 或 map[string]string

---

## ▶️ main 函数你需要做的事

在 `main()` 中：

```go
http.HandleFunc("/users", usersHandler)
http.HandleFunc("/hello", helloHandler)
http.ListenAndServe(":8080", nil)
```

---

## 🧪 测试方式（非常重要）

### 方式 1：浏览器

直接访问：

```
http://localhost:8080/users
http://localhost:8080/hello
```

### 方式 2：curl（推荐，后端常用）

```bash
curl http://localhost:8080/users
```

---

## 🔍 本关自查问题（一定要想）

1. 为什么 handler 里**不应该用 fmt.Println 返回数据**？
2. 如果 json.Marshal 出错，为什么要返回 500？
3. 为什么要设置 `Content-Type: application/json`？

---

## ⚠️ 非常重要的一点（提前说）

你现在这个 API：

* ❌ 不是并发安全的
* ❌ 没有持久化
* ❌ 没有参数解析

**这是刻意的。**

我们是在：

> **先把“最小可用 Web API”跑通**

---

## 🎯 Level 7 完成标志

请你在完成后告诉我：

**“Level 7 OK，我已经能在浏览器/curl 看到 JSON 了”**

---

## 🔜 Level 8 预告（你一定会感兴趣）

下一关我们会做：

> **POST + 解析请求 JSON → 新增用户（真正 CRUD）**

会学到：

* HTTP method（GET / POST）
* `http.Request.Body`
* `json.NewDecoder`
* 简易 REST 风格设计

等你 Level 7 完成，我们马上继续。