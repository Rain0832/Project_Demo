非常好 👏
先给你一句**工程师级评价**，再正式进入 **Level 11**。

---

## ✅ 对你当前代码的最终评估（Level 10 结算）

你现在这版代码：

* ✅ **并发安全**：`users + users.json` 在同一把锁内
* ✅ **持久化语义正确**：写的是完整 `users`
* ✅ **启动加载**：`main()` 中 `loadUsersFromJSON()`
* ✅ **锁粒度合理**：POST 原子、GET 只读
* ⚠️ **可改进但不算错**：

  * `loadUsersFromJSON()` 的错误被忽略
  * handler 职责有点“太多”

👉 **结论**：
**Level 10 已经是“合格工程实现”**，不是练习代码了。

现在进入你点名的下一关。

---

# 🧱 Level 11：拆结构（从“能跑”到“像工程”）

> 这一关 **不引入新功能**
> 只做一件事：**把职责分清楚**

---

## 一、为什么要拆？（非常关键）

你现在的 `usersHandler` 在做 5 件事：

1. HTTP 方法判断
2. JSON 编解码
3. 业务逻辑（创建用户）
4. 并发控制（mutex）
5. 文件 IO（持久化）

📌 **这是“脚本式代码”**
📌 再加功能，就会爆炸

---

## 二、Level 11 的目标（一句话）

> **让 main.go 只关心“路由”，不关心“怎么做”**

---

## 三、我们要拆成哪三层？（极简后端骨架）

```text
main.go        → 路由 & 启动
handler.go     → HTTP / JSON
storage.go     → users + 文件 + 锁
```

不多，不少，**正好**

---

## 四、每一层“只允许做什么”

### 1️⃣ main.go —— 入口层

**只干三件事**：

```go
func main() {
    storage.Init()
    http.HandleFunc("/users", usersHandler)
    http.ListenAndServe(":8080", nil)
}
```

📌 **main.go 不允许出现**：

* mutex
* users
* json.Marshal
* os.ReadFile

---

### 2️⃣ handler.go —— HTTP 适配层

它的职责是：

* 解析请求
* 调用“业务函数”
* 返回 JSON / 状态码

📌 handler **不关心数据存哪**

---

### 3️⃣ storage.go —— 数据层（本关重点）

它负责：

* `users` 的内存状态
* mutex
* 文件读写
* 保证一致性

📌 handler **只能“调用它”，不能“碰 users”**

---

## 五、storage 层应该长什么样？（接口思维）

你现在已经有这些隐含能力：

```text
- 读取所有用户
- 创建新用户
```

所以 storage 层应该提供 **函数**：

```go
func GetUsers() ([]User, error)
func CreateUser(name string, age int) (User, error)
```

📌 handler 不知道：

* users 是 slice
* 是否有锁
* 是否写文件

---

## 六、storage.go 的核心思想（不写代码也能懂）

### storage.go 内部：

```go
var (
    mu    sync.Mutex
    users []User
)
```

### 对外暴露函数：

```go
func Init() error
func GetUsers() []User
func CreateUser(name string, age int) User
```

📌 **锁完全藏在 storage 层**

---

## 七、handler 会变成什么样？（这一步你会爽）

现在的 handler 很长，对吧？

### 拆完后，GET 会像这样：

```go
func usersHandler(w http.ResponseWriter, r *http.Request) {
    if r.Method != http.MethodGet {
        http.Error(...)
        return
    }

    users := storage.GetUsers()
    json.NewEncoder(w).Encode(users)
}
```

📌 handler **突然变得“很干净”**

---

## 八、Level 11 的任务（你要做什么）

👉 **不要求一次写完**

### 任务清单：

1. 新建 `storage.go`
2. 把：

   * `users`
   * `mutex`
   * `load/store`
     全部移进去
3. 提供：

   * `Init()`
   * `GetUsers()`
   * `CreateUser()`
4. handler 不再直接操作 `users`

---

## 九、Level 11 自查问题（非常重要）

做完后，回答这 3 个问题：

1. handler 是否还能直接访问 users？
2. 如果以后把 JSON 文件换成数据库，handler 要不要改？
3. storage 层是不是“唯一知道数据怎么存的地方”？

如果全是 **“不用 / 不用 / 是”**
👉 你就真正跨进工程门槛了。

---

## 🔜 Level 12 预告（你会明显感觉提升）

Level 12 我们会做一件**非常像真实项目的事**：

> **引入“错误语义”**

* storage 返回业务错误
* handler 决定 HTTP 状态码
* 不再到处 `http.Error`

📌 这是从“能写” → “写得专业”的一步。

---

你现在可以：

* 👉 **直接开始拆 Level 11**
* 👉 拆一半贴代码，我帮你 review
* 👉 或者说：**“storage.go 我不知道怎么下手”**

你现在走得非常稳，这条 Go 后端路线是对的。
