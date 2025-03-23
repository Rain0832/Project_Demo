#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <./QtNetwork/QUdpSocket>     // 头文件路径加载
#include <QColor>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent, QString name);

    // 重写关闭事件
    void closeEvent(QCloseEvent *);

    enum Msgtype{Msg, UserEnter, UserLeft}; // 枚举 分别代表 普通信息 用户进入 用户离开
    void sndMsg(Msgtype type);  // 广播 UDP 信息
    QString getName();  // 获取名字
    QString getMsg();   // 获取聊天信息
    void userEnter(QString username);   // 处理用户输入
    void userLeft(QString username, QString time);  // 处理用户离开
    void ReciveMessage();   // 接受 UDP 信息

    ~Widget();

signals:
    void closeWidget();

private:
    Ui::Widget *ui;
    QString myname;
    quint16 port;   // 端口
    QUdpSocket *udpSocket;  // UDP 套接字
};
#endif // WIDGET_H
