# TCP network communication mini-demo

### Server process

1. Create a `socket` socket

	- Similar to an interface, realize communication

3. Bind a port number to this `socket`

	- IP address, port number

5. Enable the listening property for this `socket`

	- `socket` can only accept connections

7. Wait for the client to connect

8. Start communication

9. Close the connection

### Client process

1. Create a `socket` socket

2. Connect to the server

3. Start communication

4. Close the socket

### Client and server communication

- Multiple communications: loop implementation
- Multi-client and single server communication: multi-threading problem

### Demo

1. First create a socket and turn on the listening property.

	- Server

	- Port number 268

2. A new client is connected.

	- Port number 276, 284

3. Received a message from the client, multi-threaded implementation,

	- 284: 123

	- 276: abc

4. A new client has connected.

	- Port number 300

5. Received a message from the client, multi-threaded implementation.

	- 300: 123

6. The client disconnected.

	- 284 disconnected

	- 276 disconnected

7. Received a message from the client, multi-threaded implementation.

	- 300: 123

8. The client disconnected.

	- 300 disconnected

![img](https://cdn.nlark.com/yuque/0/2025/bmp/44785005/1738334177029-acee9993-223c-489f-9710-362695281d61.bmp)

---

# TCP网络通讯小项目

### 服务端的流程

1. 创建`socket`套接字

1. - 类似一个接口、实现通信

2. 给这个`socket`绑定一个端口号

1. - IP地址、端口号

3. 给这个`socket`开启监听属性

1. - `socket`只能做接受连接

4. 等待客户端连接
5. 开始通讯
6. 关闭连接

### 客户端的流程

1. 创造`socket`套接字
2. 连接服务端
3. 开始通讯
4. 关闭套接字

### 客户端与服务端通讯

- 多次通讯：循环实现
- 多客户端与单服务端通讯：多线程问题

### 示例

1. 首先进行socket套接字创立，打开监听属性。

	- 服务端

	- 端口号268

3. 有新的客户端接入。

	- 端口号276、284

4. 收到来自客户端的消息，多线程实现，

	- 284：123

	- 276：abc

5. 有新的客户端接入。

	- 端口号300

6. 收到来自客户端的消息，多线程实现。

	- 300：123

7. 客户端断开连接。

	- 284断开连接

	- 276断开连接

8. 收到来自客户端的消息，多线程实现。

	- 300：123

9. 客户端断开连接。

	- 300断开连接

![img](https://cdn.nlark.com/yuque/0/2025/bmp/44785005/1738334177029-acee9993-223c-489f-9710-362695281d61.bmp)