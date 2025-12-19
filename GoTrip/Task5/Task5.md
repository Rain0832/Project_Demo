非常好，这一关你**不只是“做完了”**，而是真的**理解到位了**。
我先给你一个**教练级点评**，然后正式进入 **Level 5**。

---

## 🟢 **Level 4 代码与理解点评（简短但关键）**

### ✅ 你的三点理解基本正确（我只补精度）

1. **struct 是值类型 → 安全性高、语义清晰**
   ✔️ 正确
   更准确地说：Go 倾向“显式拷贝”，避免你在不知情时修改共享数据。

2. **需要修改时用指针接收者**
   ✔️ 完全正确
   Go 官方建议：

   > 如果方法会修改接收者，或 struct 较大 → 用 `*T`

3. **map 遍历无序**
   ✔️ 正确
   补充一嘴：

   * Go 1.12+ 明确保证 **map iteration order 是随机的**
   * 这是为了防止你依赖顺序写 bug，也减少 hash 攻击面

---

### ⭐ 代码层面的加分点

* 正确使用 `delete`
* 正确理解 `score["A"]` 在不存在时返回零值
* 正确区分 `User` 和 `*User`
* 正确理解 `map[int]User` 中存的是 **拷贝**

你已经具备写真实业务数据结构的能力了。

---

# 🚀 **Level 5：JSON 编解码（Web 后端的“语言”）**

> **这一关是从“写 Go 程序” → “写 Web 后端”的分水岭**

预计 15–20 分钟。

---

## 🎯 **Level 5 的学习重点（5 个）**

1. JSON 与 struct 的映射关系
2. `encoding/json` 的 `Marshal / Unmarshal`
3. struct tag（反引号里的魔法）
4. slice / map 与 JSON 的关系
5. JSON 编解码失败的原因

---

## 🧠 **最白话解释**

### **1. Web 世界说的是 JSON，不是 Go**

前端 / HTTP / API 统一用 JSON：

```json
{
  "name": "Tom",
  "age": 20
}
```

Go 的 struct 是**内存结构**，JSON 是**字符串格式**，
`encoding/json` 负责翻译。

---

### **2. Marshal / Unmarshal 是“序列化 / 反序列化”**

* **Marshal**：Go → JSON
* **Unmarshal**：JSON → Go

就像：

* 打包 → 运输 → 拆包

---

### **3. struct tag 决定 JSON 长什么样**

这行是关键：

```go
Name string `json:"name"`
```

意思是：

> Go 里叫 Name，JSON 里叫 name

不写 tag：

* JSON 默认使用字段名（区分大小写）

---

### **4. JSON 天然支持 slice / map**

```go
[]User        → JSON array
map[string]T → JSON object
```

这正是 Web API 最常见的数据结构。

---

### **5. Unmarshal 必须传指针**

因为它要修改你的变量。

```go
json.Unmarshal(data, &u)
```

---

# 📚 **本关语法 / API 名字**

你只需知道这些名字：

* 包：`encoding/json`
* `json.Marshal`
* `json.Unmarshal`
* struct tag： `` `json:"xxx"` ``
* 错误处理：`error`

---

# 📝 **你的任务（15–20 分钟）**

创建目录：

```
GoTrip/Task5
```

编写 `main.go`。

---

## **任务 1：struct → JSON（Marshal）**

定义 struct：

```go
type User struct {
    Name string `json:"name"`
    Age  int    `json:"age"`
}
```

在 main 中：

1. 创建一个 User
2. 调用 `json.Marshal`
3. 打印 JSON 字符串

---

## **任务 2：JSON → struct（Unmarshal）**

给你一段 JSON 字符串（写死在代码里）：

```json
{"name":"Alice","age":22}
```

1. 定义一个空 User
2. Unmarshal 到 User
3. 打印 User 内容

---

## **任务 3：slice + JSON**

1. 创建 `[]User`，放 2–3 个用户
2. Marshal 成 JSON
3. 打印结果

---

## **任务 4（思考题，代码可选）**

如果 JSON 中缺少某个字段，会发生什么？

---

# 🔍 **自查问题**

1. 为什么 Unmarshal 一定要传指针？
2. struct tag 是给谁看的？Go 还是 JSON？
3. JSON 和 map[string]interface{} 的关系是什么？

---

完成后告诉我：

**“Level 5 OK，进入 Level 6”**

下一关将进入真正的后端世界：
👉 **Level 6：net/http —— 第一个 Go Web Server**
