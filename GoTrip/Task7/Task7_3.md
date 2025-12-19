非常好 👍
你这份 **after optimization** 的代码已经是 **“新手 → 合格 Go Web 后端”** 的标准形态了。
现在我们正式进入你点名的这一关：

---

# 🚦 Level 9：并发安全（为什么你的代码“现在是有 bug 的”）

> **这一关非常关键**
> 不是语法，而是「Go 服务器是怎么跑的」

---

## 一、先给结论（不拐弯）

👉 **你的代码在并发情况下是有问题的**
👉 **问题出在：全局变量 `users`**
👉 **解决方法：`sync.Mutex`**

---

## 二、最核心的一句话（一定要记住）

> **Go 的 http server：每一个请求，都是一个 goroutine**

也就是说：

```text
用户 A 发 POST /users  → goroutine #1
用户 B 发 POST /users  → goroutine #2
用户 C 发 GET  /users  → goroutine #3
```

**这些 goroutine 会同时运行**。

---

## 三、现在的代码哪里不安全？

### 你现在的共享数据

```go
var users = []User{ ... }
```

### 在 POST 里你做了什么？

```go
newID := len(users) + 1
users = append(users, newUser)
```

---

### ❗ 并发场景下的真实问题

假设 **两个 POST 同时到达**：

#### goroutine A

```go
len(users) == 2
newID = 3
```

#### goroutine B（几乎同时）

```go
len(users) == 2
newID = 3
```

然后：

```text
users = append(users, {ID:3})
users = append(users, {ID:3})
```

🚨 **ID 冲突 + slice 并发写 = 未定义行为**

---

## 四、这不是“可能有问题”，而是「一定会出问题」

* 并发写 slice → **数据竞争**
* 数据竞争 → Go 运行时未定义行为
* 开 `-race` 会直接报错

📌 **这在面试、真实项目里是“致命错误”**

---

## 五、Level 9 的唯一新知识：`sync.Mutex`

### Mutex 是什么？

一句人话：

> **一把锁：同一时间，只允许一个 goroutine 访问数据**

---

## 六、最小正确用法（新手必背）

```go
var mu sync.Mutex

mu.Lock()
defer mu.Unlock()

// 访问共享数据
```

---

## 七、把 Mutex 用到你的代码里（最小改动版）

### 1️⃣ 引入 mutex

```go
import "sync"

var mu sync.Mutex
```

---

### 2️⃣ 在 GET / POST 里保护 users

### ✅ 改后的 usersHandler（只加锁，不改结构）

```go
func usersHandler(w http.ResponseWriter, r *http.Request) {
	switch r.Method {
	case http.MethodGet:
		mu.Lock()
		jsonBytes, err := json.Marshal(users)
		mu.Unlock()

		if err != nil {
			http.Error(w, "internal error", http.StatusInternalServerError)
			return
		}
		w.Header().Set("Content-Type", "application/json")
		w.Write(jsonBytes)

	case http.MethodPost:
		var req CreateUserRequest
		if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
			http.Error(w, "bad request", http.StatusBadRequest)
			return
		}

		mu.Lock()
		newID := len(users) + 1
		newUser := User{
			ID:   newID,
			Name: req.Name,
			Age:  req.Age,
		}
		users = append(users, newUser)
		mu.Unlock()

		w.Header().Set("Content-Type", "application/json")
		jsonBytes, err := json.Marshal(newUser)
		if err != nil {
			http.Error(w, "internal error", http.StatusInternalServerError)
			return
		}
		w.Write(jsonBytes)

	default:
		http.Error(w, "method not allowed", http.StatusMethodNotAllowed)
	}
}
```

📌 **这就是 Level 9 的全部内容**

---

## 八、几个你现在不用管、但我先点名的东西

你现在 **不需要学**：

* RWMutex
* atomic
* channel 做 server
* 数据库事务

但你已经 **站在这些东西的门口了**。

---

## 九、你现在处在什么水平？

说实话：

* 你已经不是“学语法”的人
* 你已经在写 **真正的后端代码**
* 你已经开始考虑：

  * 并发
  * 幂等性
  * 状态一致性

这已经是 **实习生 / 初级后端的核心能力**

---

## 🔜 Level 10 预告（下一关）

下一关我们解决一个**非常真实的问题**：

> **服务器重启，数据全没了，怎么办？**

Level 10 内容（只选一个）：

1. 用文件存 users（最简单持久化）
2. 拆 handler / service（结构化）
3. 用 curl 构造更真实的 API 行为

你选一个，或者直接说：

> **Level 10 开始（你来安排）**

我会继续带你，一步一步把这条 Go Web 后端路走扎实 💪

## 十、问题
##### 1. 为什么 GET 里 json.Marshal(users) 要加锁，而 POST 里 json.Marshal(newUser) 不用？

Get 中是 `json.Marshal(users)` 。
- users 是 全局共享变量
- 可能正在被其他 goroutine append
- 你这里是在 读共享 slice

Post 中是 `json.Marshal(newUser)` 。
- newUser 是 当前请求的局部变量
- 只存在于当前 goroutine
- 没有任何其他 goroutine 能碰到它

##### 2. 为什么 w.Write(jsonBytes) 不用加锁？map 的地方呢？

`w.Write(jsonBytes)` 中
- w 是 当前请求专属的 ResponseWriter
- 每个 HTTP 请求都有自己的 w
- 不会被其他 goroutine 共享

> 所以理论上是一个 HTTP 一个 goroutine 这样？
每个请求 = 独立 response 管道

另外 map 的地方，这里是局部变量的 map，如果逃逸成全局变量，会触发 panic，同样需要在读写的地方进行上锁。

##### 3. 为什么这里不用 defer mu.Unlock()？defer 是什么原理？

defer 是函数返回前，一定会执行的语句。
```go
mu.Lock()
defer mu.Unlock()
// 后面随便 return，都不会忘记解锁
```

✔️ 好处：
- 防止忘记 Unlock
- 中间有 return 也安全
- 大型函数更稳妥

⚠️ 什么时候不建议 defer？
- 临界区极小、在热点循环中
- defer 有微小性能开销（现在你不用关心）