# GoUserAPI 🚀

A production-grade Go backend project for user management, following **level-based progressive engineering practices**.  
从基础语法到生产级雏形，循序渐进构建可扩展、高可靠的用户管理 API。

## 🌟 核心特性（Level 1-15 积累）
| 特性                | 说明                                                                 |
|---------------------|----------------------------------------------------------------------|
| 工程化项目结构      | 分层设计（`model/handler/storage/config`），解耦清晰                   |
| 存储抽象与可替换    | 基于 `UserStorage` 接口，当前实现文件存储（`FileUserStorage`），可无缝切换 MySQL/Redis |
| 并发安全保障        | `sync.Mutex` 保护临界区，避免数据竞争，解决死锁风险                   |
| 统一错误处理        | 自定义 `AppError` 结构体，区分业务错误码与 HTTP 状态码，前端友好响应   |
| 请求参数校验        | 基于 `validator/v10` 实现输入合法性校验（用户名、年龄范围等）          |
| 基础日志系统        | 请求日志中间件 + 业务操作日志，支持调试与排障                         |
| 优雅退出与资源清理  | 捕获系统信号（Ctrl+C/kill），安全释放存储资源                         |
| 配置管理            | 读取 `config.json` 配置文件，支持端口、存储路径等环境适配               |
| 自动 ID 自增        | 服务端生成唯一用户 ID，避免客户端篡改，保证数据一致性                 |
| 数据持久化          | 基于文件存储（`users.json`），支持新增、查询用户操作                   |

## 📋 技术栈
- 核心语言：Go 1.24+
- 依赖包：
  - 参数校验：`github.com/go-playground/validator/v10`
  - 标准库：`net/http`、`encoding/json`、`sync`、`log`、`os/signal` 等

## 🚀 快速开始

### 1. 环境准备
- Go 1.24+ 安装：[官方文档](https://go.dev/doc/install)
- 克隆项目：
  ```bash
  git clone https://github.com/your-username/GoUserAPI.git
  cd GoUserAPI
  ```

### 2. 安装依赖
```bash
go mod tidy
```

### 3. 配置文件（config.json）
默认配置无需修改，支持自定义端口和存储路径：
```json
{
  "server_port": ":8080",
  "storage_path": "users.json"
}
```

### 4. 启动服务
```bash
go run .
# 输出：2025/12/19 16:00:00 Server start in: 8080
```

### 5. 接口测试（curl 示例）
#### 新增用户（POST）
```bash
curl -X POST http://localhost:8080/users \
-H "Content-Type: application/json" \
-d '{"name":"张三","age":28}'
```
响应（带自动生成的 ID）：
```json
{"code":0,"message":"创建成功","user":{"id":5,"name":"张三","age":28}}
```

#### 查询所有用户（GET）
```bash
curl -X GET http://localhost:8080/users
```
响应：
```json
[{"id":1,"name":"Tom","age":20},{"id":2,"name":"Amy","age":22},{"id":5,"name":"张三","age":28}]
```

## 📂 项目结构
```
GoUserAPI/
├── config.go        # 配置加载逻辑
├── config.json      # 配置文件
├── errors.go        # 统一错误处理定义
├── handler.go       # API 处理器（请求响应逻辑）
├── main.go          # 程序入口（服务启动、路由注册）
├── middleware.go    # 中间件（日志记录）
├── model.go         # 数据模型（User 结构体）
├── storage.go       # 存储层（接口定义 + 文件实现）
├── users.json       # 数据持久化文件
├── go.mod           # 依赖管理
└── go.sum           # 依赖校验
```

### 文件职责说明
| 文件           | 核心职责                                                                 |
|----------------|--------------------------------------------------------------------------|
| `model.go`     | 定义业务实体（如 `User` 结构体），统一数据模型                           |
| `handler.go`   | 接收 HTTP 请求，解码参数、调用存储层、返回响应，衔接请求与业务逻辑       |
| `storage.go`   | 定义 `UserStorage` 接口及文件实现，负责数据的增删改查与持久化             |
| `config.go`    | 读取配置文件，提供统一的配置访问入口，适配不同环境                       |
| `errors.go`    | 自定义错误类型，统一错误响应格式，区分业务错误与系统错误                 |
| `middleware.go`| 扩展 HTTP 处理逻辑（如日志记录、认证授权，当前实现请求日志）             |

## 🔍 关键设计决策
### 1. 接口抽象（UserStorage）
```go
type UserStorage interface {
    AddUser(user model.User) (model.User, error)
    ListUser() ([]model.User, error)
}
```
- 解耦业务逻辑与存储实现，后续可无缝替换为 MySQL/Redis 存储
- 便于单元测试（可模拟 `UserStorage` 接口）

### 2. 并发安全设计
- 所有对共享数据（`s.users`）的操作都在 `sync.Mutex` 锁保护下
- 避免“查找-修改”拆分导致的原子性问题，确保操作完整性

### 3. 数据一致性保障
- 新增用户采用“先写文件，再更内存”策略，避免崩溃导致的数据丢失
- 服务端自动生成 ID，避免客户端传入非法 ID 导致冲突

## 📈 扩展方向（Level 16+ 规划）
1. 存储扩展：实现 `MySQLUserStorage`，替换文件存储
2. 功能扩展：新增用户更新（PUT）、删除（DELETE）、单用户查询（GET /users/:id）接口
3. 性能优化：添加 Redis 缓存热点用户数据
4. 安全增强：实现 JWT 认证、接口限流、HTTPS 支持
5. 监控告警：接入 Prometheus + Grafana，监控接口 QPS、错误率
6. 日志增强：使用 `zap` 替代标准库日志，支持日志分级、滚动存储

## 🎯 学习价值
本项目适合 Go 新手从“语法入门”到“工程实践”进阶，重点掌握：
- 分层架构设计与接口抽象
- 并发安全与数据一致性保障
- 错误处理与日志系统的工程化实现
- 配置管理与优雅退出的生产级实践
- 从“能跑起来”到“稳定可靠”的迭代思路

## 🤝 贡献指南
1. Fork 本仓库
2. 创建特性分支（`git checkout -b feature/xxx`）
3. 提交代码（`git commit -m "add xxx feature"`）
4. 推送分支（`git push origin feature/xxx`）
5. 提交 Pull Request