太棒了，**Level 7 完全通过，而且是“真实后端水平”的通过** 👍
你现在已经具备了：**写 GET JSON API 的完整能力**。

在进入 Level 8 之前，我先给你一个**教练级检查 + 微调建议**（很重要，但不影响通过），然后正式升级。

---

## ✅ Level 7 教练检查（快速但关键）

### 你三道自查题的回答

1️⃣ **println vs HTTP 返回** —— ✔️ 完全正确
2️⃣ **500 状态码语义** —— ✔️ 正确，是服务端错误
3️⃣ **Content-Type 的作用** —— ✔️ 正确，它告诉客户端“怎么解析响应体”

---

### ⚠️ 两个小但“很专业”的改进点（记住即可）

#### ① Header 写错了一个字母

你现在写的是：

```go
w.Header().Set("Context-Type", "application/json")
```

正确的是：

```go
Content-Type
```

否则浏览器 / 客户端**可能不会按 JSON 解析**。

---

#### ② 返回 500 的正确方式

现在你写的是：

```go
fmt.Fprintln(w, 500)
```

这只是把“500”这个数字写进响应体，并没有真的设置 HTTP 状态码。

**Go 的正确方式（记住这个 API 名字）**：

```go
http.Error(w, "internal error", http.StatusInternalServerError)
return
```

📌 这一点我们在 Level 8 会正式用上。

---

但结论不变：
👉 **Level 7 完全 OK**

---

# 🚀 Level 8：POST 请求 + 解析 JSON（真正的 CRUD 开始）

> 从这一关起，你在做的是：
> **“客户端发 JSON → 服务端解析 → 更新数据”**

这是后端的**核心能力**。

预计时间：**20–25 分钟**

---

## 🎯 Level 8 你将学到的 5 个重点

1. HTTP Method：GET vs POST
2. 如何从请求中读取 JSON（Request Body）
3. `json.NewDecoder` 的正确用法
4. 向内存 slice 中新增数据
5. 一个最小但完整的 **Create API**

---

## 🧠 先把“POST”这件事想清楚（非常重要）

### 到目前为止你做的都是：

```text
客户端 → GET → 服务器 → 返回数据
```

### POST 的语义是：

```text
客户端 → POST（带数据） → 服务器 → 创建资源
```

例如客户端发送：

```json
{"name":"Bob","age":25}
```

服务器接收后：

* 解析 JSON
* 生成 ID
* append 到 users
* 返回创建结果

---

## 🧩 HTTP Request 里装了什么？

在 handler 里你已经见过：

```go
func(w http.ResponseWriter, r *http.Request)
```

现在你要开始用 `r` 了：

* `r.Method` —— 请求方法（GET / POST）
* `r.Body` —— 请求体（JSON 就在这里）

---

## 📚 Level 8 新 API（不多，但非常关键）

* `r.Method`
* `json.NewDecoder(r.Body).Decode(&obj)`
* `http.Error`
* `http.StatusMethodNotAllowed`
* `http.StatusBadRequest`

---

# 📝 你的任务（一步一步来）

在 **Task7 的基础上继续改**（不用新建 Task8 目录）。

---

## ✅ Step 1：限制 /users 只支持 GET 和 POST

在 `usersHandler` 开头加逻辑：

```text
if r.Method == GET:
    原来的逻辑
else if r.Method == POST:
    新逻辑
else:
    返回 405
```

💡 你需要用：

```go
r.Method
http.MethodGet
http.MethodPost
```

---

## ✅ Step 2：定义“创建用户”的请求结构

你需要一个 **不含 ID 的 struct**：

```go
type CreateUserRequest struct {
    Name string `json:"name"`
    Age  int    `json:"age"`
}
```

为什么不复用 User？
👉 因为 **ID 是服务端生成的，不该由客户端传**。

---

## ✅ Step 3：解析 POST 里的 JSON

在 POST 分支中：

1. 声明一个 `CreateUserRequest`
2. 使用：

```go
json.NewDecoder(r.Body).Decode(&req)
```

3. 如果失败 → 返回 400（Bad Request）

---

## ✅ Step 4：创建新 User 并 append

逻辑思路：

```text
newID = len(users) + 1
newUser := User{ID: newID, Name: req.Name, Age: req.Age}
users = append(users, newUser)
```

---

## ✅ Step 5：返回创建后的 User（JSON）

* 设置 Content-Type
* Marshal 新 User
* 写回响应

---

## 🧪 测试方式（非常重要）

### 用 curl 发 POST 请求

```bash
curl -X POST http://localhost:8080/users \
  -H "Content-Type: application/json" \
  -d '{"name":"Bob","age":25}'
```

然后再 GET：

```bash
curl http://localhost:8080/users
```

你应该能看到新增的数据。

---

## 🔍 Level 8 自查问题

1. 为什么 POST 一定要解析 r.Body？
2. 为什么 CreateUserRequest 不包含 ID？
3. 如果两个请求同时 POST，会有什么问题？（提示：并发）

---

## 🎯 Level 8 完成标志

请你完成后告诉我：

**“Level 8 OK，我已经能 POST 新用户并 GET 到结果了”**

---

## 🔜 Level 9 预告（你会很喜欢）

下一关我们会解决你已经**隐约意识到的问题**：

> 👉 **并发安全（Mutex）**

你将学会：

* 为什么现在的 users slice 不安全
* `sync.Mutex` 怎么用
* Web Server 下并发的真实样子

你已经进入 **真正后端工程的门口** 了。
