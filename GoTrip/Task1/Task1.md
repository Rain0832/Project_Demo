明白！从现在开始我就是你的 **Go 学习教练**。
整个训练会像打游戏一样“过关升级”，每关 10–20 分钟可完成。
你自己敲代码，我只告诉“你需要的语法/API”和“为什么 Go 要这么设计”。

下面正式开始。

---

# **Level 1：Go 环境验证 + 第一个 Go 程序**

## 🎯 **这一关的目标（3–5 个重点）**

1. **知道 Go 项目是怎样组织的**（main.go, module）
2. **理解 go run / go build 的区别**
3. **写出最小可运行的 main.go**
4. **理解为什么 Go 要有 package / module**

---

## 🧠 **最白话解释**

* **Go 的项目 = 文件夹 + go.mod + .go 文件**
  类似你书桌上放一本“说明书”（go.mod）+ 一堆“代码纸张”（.go）。

* **package main + func main()**
  就像 C/C++ 的入口，告诉 Go：“从这里开始跑”。

* **go run main.go** = 直接执行

* **go build** = 生成自带启动能力的二进制文件

Go 的核心理念：

> **“只要有 main.go，我就能跑。”**

不需要虚拟环境、不需要 makefile、不需要复杂依赖管理。

---

## 📚 **本关会用到的语法/API**

你只需了解名字，不需背：

* `package main`
* `import "fmt"`
* `func main()`
* `fmt.Println("...")`
* CLI：`go version`, `go mod init`, `go run`, `go build`

---

## 📝 **你的任务（10 分钟）**

❶ 打开 WSL 终端，输入：

```
go version
```

你应该看到类似：

```
go version go1.22 linux/amd64
```

---

❷ 为你的 Go 课程创建一个项目目录（名字随便）：

```
mkdir go-learning
cd go-learning
```

---

❸ 初始化 Go module（Go 项目的身份证）

```
go mod init example.com/learn
```

（域名随便写，无需真实存在）

---

❹ 创建文件：

**main.go**

你自己写，结构应当包含：

* `package main`
* 一个 `import fmt`
* 一个 `main()`
* 在 main 中打印一行 "Hello Go"

---

❺ 运行：

```
go run main.go
```

再试试：

```
go build
./learn
```

---

## 🔍 **自查问题（你不用回答我，但要能自己回答）**

1. 为什么 Go 项目要用 `go.mod`？它解决了什么问题？
2. 为什么 main.go 必须写在 `package main` 里？

---

## 🎉 **完成这一关后你将具备**

* 能创建一个 Go 项目
* 能跑一个 Go 程序
* 能解释 go run / go build 的区别

当你完成 Level 1，请告诉我：
**“Level 1 OK，进入 Level 2”**
我会继续带你进入 **变量 & 类型 & 函数基础（Level 2）**。
