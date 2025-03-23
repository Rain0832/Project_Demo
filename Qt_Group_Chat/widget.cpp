#include <QDataStream>
#include <QMessageBox>
#include <QDateTime>
#include <QFont>
#include <QColorDialog>
#include <QFileDialog>

#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent, QString name)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    myname = name;

    this->port = 9999;  // 初始化端口
    this->udpSocket = new QUdpSocket(this);     // new 套接字 创建


    // 绑定端口
    /*
     * 采用ShareAddress模式
     * 允许其他的服务器连接到相同的 地址 和 端口
     * 特别在 多客户端 监听 一个服务端 时特别有效
     *
     * ReuseAddressHint模式
     * 重新连接服务器
     */
    udpSocket->bind(port, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);

    // 监听信号
    connect(udpSocket, &QUdpSocket::readyRead, this, &Widget::ReciveMessage);

    // 连接发送按钮 - 信号 与 槽
    connect(ui->sendBtn, &QPushButton::clicked, this, [=](){
        sndMsg(Msg);
    });

    // 新用户进入
    sndMsg(UserEnter);

    // 退出按钮
    connect(ui->exitBtn, &QPushButton::clicked, [=](){
        this->close();
    });



    //----------功能区实现------------------------------------------------------------

    // 字体样式
    connect(ui->fontCbx, &QFontComboBox::currentFontChanged, [=](const QFont &font){
        ui->msgTxtEdit->setFontFamily(font.toString());
        ui->msgTxtEdit->setFocus();
    });

    // 字体大小
    void (QComboBox:: * sizebtn)(const QString &text) = &QComboBox::currentTextChanged; // 函数指针
    connect(ui->sizeCbx, sizebtn, [=](const QString &text){
        ui->msgTxtEdit->setFontPointSize(text.toDouble());
        ui->msgTxtEdit->setFocus();
    });

    // 加粗
    connect(ui->boldTBtn, &QToolButton::clicked, [=](bool checked){
        if(checked)
        {
            ui->msgTxtEdit->setFontWeight(QFont::Bold);
        }
        else
        {
            ui->msgTxtEdit->setFontWeight(QFont::Normal);
        }
    });

    // 倾斜
    connect(ui->italicTbtn, &QToolButton::clicked, [=](bool checked){
        ui->msgTxtEdit->setFontItalic(checked);
        ui->msgTxtEdit->setFocus();
    });

    // 下划线
    connect(ui->underlineTBtn, &QToolButton::clicked, [=](bool checked){
        ui->msgTxtEdit->setFontUnderline(checked);
        ui->msgTxtEdit->setFocus();
    });

    // 设置文本颜色
    connect(ui->colorTBtn, &QToolButton::clicked, [=](){
        QColor color = QColorDialog::getColor(color, this);

        ui->msgTxtEdit->setTextColor(color);
    });

    // 清空聊天区
    connect(ui->clearTBtn, &QToolButton::clicked, [=](){
        ui->msgBrowser->clear();
    });

    // 保存聊天记录
    connect(ui->savaTBtn, &QToolButton::clicked, [=](){
        if(ui->msgBrowser->toPlainText().isEmpty())
        {
            QMessageBox::warning(this, "警告", "警告！ 保存内容不能为空！");
            return ;
        }

        // 默认设置保存名称
        QString filename = QFileDialog::getSaveFileName(this, "保存聊天记录", "聊天记录", "(*.txt)");

        // 输入文件名不为空才进行保存
        if(!filename.isEmpty())
        {
            QFile file(filename);
            file.open(QIODevice::WriteOnly | QFile::Text);
            QTextStream stream(&file);
            stream << ui->msgBrowser->toPlainText();
            file.close();
        }
    });
}

void Widget::closeEvent(QCloseEvent *)
{
    emit this->closeWidget();

    sndMsg(UserLeft);

    udpSocket->close();
    udpSocket->destroyed();
}

void Widget::sndMsg(Widget::Msgtype type)
{
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << type << this->getName();

    switch(type)
    {
    case Msg:
        if(ui->msgTxtEdit->toPlainText() == "")
        {
            QMessageBox::warning(this, "警告", "发送的聊天内容不能为空！");
            return ;
        }
        stream << this->getMsg();
        break;

    case UserEnter:
        break;

    case UserLeft:
        break;
    }

    // 书写 UDP 报文
    udpSocket->writeDatagram(array.data(), array.size(), QHostAddress::Broadcast, this->port);
}

QString Widget::getName()
{
    return this->myname;
};

QString Widget::getMsg()
{
    QString msg = ui->msgTxtEdit->toHtml();     // Html 可以实现加粗等效果
    ui->msgTxtEdit->clear();        // 清空输入区
    ui->msgTxtEdit->setFocus();     // 将输入焦点 光标 设置到输入区

    return msg;
}

void Widget::userEnter(QString username)
{
    bool IsEmpty = ui->tableWidget->findItems(username, Qt::MatchExactly).isEmpty();

    if(IsEmpty)
    {
        // 聊天框右边的群聊列表显示
        QTableWidgetItem *table = new QTableWidgetItem(username);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->setItem(0, 0, table);

        ui->msgBrowser->setTextColor(QColor(Qt::gray));
        ui->msgBrowser->append(username + "已上线");
        ui->userNumLbl->setText(QString("在线人数：%1").arg(ui->tableWidget->rowCount()));

        sndMsg(UserEnter);
    }
}

void Widget::userLeft(QString username, QString time)
{
    bool IsEmpty = ui->tableWidget->findItems(username, Qt::MatchExactly).isEmpty();
    if(!IsEmpty)
    {
        int row = ui->tableWidget->findItems(username, Qt::MatchExactly).first()->row();

        ui->tableWidget->removeRow(row);

        ui->msgBrowser->append(QString("%1用户于%2离开").arg(username).arg(time));

        ui->userNumLbl->setText(QString("在线人数：%1").arg(ui->tableWidget->rowCount()));
    }
}

void Widget::ReciveMessage()
{
    // 获取大小
    qint64 size = udpSocket->pendingDatagramSize();

    // 字节
    int mysize = static_cast<int>(size);
    QByteArray *array = new QByteArray(mysize, 0);

    udpSocket->readDatagram((*array).data(), size);

    // 流
    QDataStream stream(array, QIODevice::ReadOnly);

    int mytype;
    QString name, msg; // 用户名 聊天内容
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");    // 当前时间

    stream >> mytype;

    switch(mytype)
    {
    case Msg:
        stream >> name >> msg;
        ui->msgBrowser->setTextColor(QColor(Qt::blue));
        ui->msgBrowser->append("[" + name + "]" + time);
        ui->msgBrowser->append(msg);
        break;

    case UserEnter:
        stream >> name;
        userEnter(name);
        break;

    case UserLeft:
        stream >> name;
        userLeft(name, time);
        break;
    }

}

Widget::~Widget()
{
    delete ui;
}
