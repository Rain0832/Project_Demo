太好了，你 **Level 5 的理解和代码都是合格且偏优秀的**。
你对 JSON 缺字段、默认值、Unmarshal 必须用指针、`map[string]interface{}` 的定位，全都说对了 👍
而且你这句 **“repeat is practice”** 非常 Go——后面 Web 里会自然复习。

你也提前说明了一个**非常重要的信息**：

> 👉 **我没有 Web Server 基础，需要讲清楚“监听端口 / 浏览器访问 / Go 代码怎么写”**

这一点我会在 **Level 6** 用「**白话 + 拆层**」的方式讲清楚，
**不默认任何 Web 背景**。

---

# 🚀 Level 6：第一个 Go Web Server（net/http 入门）

> **这是从“本地程序” → “网络服务”的质变点**

预计 **20 分钟**（略长一点，但值）。

---

## 🎯 Level 6 你将学会什么（5 个重点）

1. Web Server 到底在干什么（非常白话）
2. Go 中如何“监听端口”
3. 什么是 Handler（处理函数）
4. 浏览器访问 `http://localhost:8080` 时发生了什么
5. 用 Go 返回字符串给浏览器

---

## 🧠 先别写代码，先把 Web 这件事想清楚

### 🌍 1️⃣ Web Server 本质在干嘛？

一句话版本：

> **Web Server = 一直在等别人敲门，然后按规则回一句话**

更展开一点：

* 浏览器：
  👉「我想访问 `/`」
* 服务器：
  👉「好，我给你一个响应」

---

### 🔌 2️⃣ 什么叫“监听端口 :8080”？

你可以把电脑想象成一栋楼：

* IP = 楼地址（localhost = 自己）
* 端口 = 门牌号（8080 号门）

```text
http://localhost:8080
```

意思是：

> 去“我自己这台机器”的 **8080 号门** 问点东西

---

## 🧠 Go 是如何做 Web Server 的？

### 核心包：`net/http`

你只要记住三样东西（先别深究）：

1. **http.HandleFunc** —— 注册规则
2. **handler 函数** —— 怎么回应
3. **http.ListenAndServe** —— 开门营业

---

## 🧩 拆开来看（非常重要）

### ① Handler 是什么？

Handler 就是：

> **“有人访问某个路径时，Go 调用的函数”**

它的固定签名是：

```go
func(w http.ResponseWriter, r *http.Request)
```

你现在只需知道：

* `w`：你用它 **往浏览器写内容**
* `r`：浏览器发来的请求（现在先不用）

---

### ② 注册一个路由（路径）

```go
http.HandleFunc("/", handler)
```

意思是：

> **当有人访问 `/`，就用 handler 来处理**

---

### ③ 开始监听端口（最关键的一行）

```go
http.ListenAndServe(":8080", nil)
```

意思是：

> **打开 8080 端口，一直等请求（会阻塞）**

---

## 📚 本关你会用到的 API 名字（不背，认识即可）

* 包：`net/http`
* `http.HandleFunc`
* `http.ListenAndServe`
* `http.ResponseWriter`
* `http.Request`
* `fmt.Fprintln`

---

# 📝 你的任务（我会非常明确）

创建目录：

```
GoTrip/Task6
```

新建 `main.go`。

---

## ✅ 任务 1：写一个最小 Web Server

你需要完成这些逻辑（不是让我帮你写）：

1. `package main`
2. `import net/http`
3. 定义一个 handler 函数

   * 参数是 `(w http.ResponseWriter, r *http.Request)`
   * 用 `fmt.Fprintln(w, "Hello Go Web")`
4. 在 main 中：

   * 注册 `/`
   * 监听 `:8080`

---

## ▶️ 运行方式（非常重要）

```bash
go run main.go
```

你会看到：**程序卡住了**（这是对的）

然后：

* 打开浏览器
* 输入：

```
http://localhost:8080
```

你应该能看到：

```
Hello Go Web
```

---

## 🔍 自查问题（一定要想）

1. 为什么 Web Server 程序不会自己退出？
2. handler 函数是“谁”调用的？
3. 浏览器访问 `/`，Go 是怎么知道该用哪个函数？

---

## ⚠️ 先别急着扩展

现在我们 **只做一件事：能跑起来 + 能返回内容**

下一关我们才会：

* 多个路由
* 返回 JSON
* 用 struct + slice
* 做一个 mini API

---

完成后请你告诉我：

**“Level 6 OK，我已经在浏览器看到内容了”**

👉 **Level 7 将正式开始：返回 JSON 的 Web API（Todo/Notes 的第一步）**
