#include <QIcon>
#include <QToolButton>
#include <QMessageBox>

#include "widget.h"
#include "login.h"
#include "ui_login.h"

login::login(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);

    // 设置QQ企鹅图标
    // 路径：冒号+前缀+路径
    this->setWindowIcon(QIcon(":/images/qq.png"));

    // 设置名称
    this->setWindowTitle("GroupChat");

    QList<QString> nameList;
    nameList << "勾勾メ小手指☆" << "淡蓝↙爱。" << "心脏旳⒉／⒈。" << "ヤ埖伞╰" << "Fly Pig" << "花丶尕旭" << "莪狠爱星。" << "‘メ⒈辈孓。" << "◇◆格子、涂个的夏天。";

    // 头像资源列表
    QStringList iconNameList;
    iconNameList << "shan" << "ymrl" << "qq" << "Cherry" << "dr" << "jj" << "lswh" << "qmnn" << "shan";
    // iconNameList << "superwings" << "ironman" << "shan" << "korean" << "flypig" << "dog" << "snake" << "jokic" << "curry";


    //
    QVector<QToolButton *> vector;
    for(int i = 0; i < 9; i++)
    {
        QToolButton *btn = new QToolButton(this);

        // 加载头像
        btn->setIcon(QPixmap(QString(":/images/%1.png").arg(iconNameList[i])));

        // 设置头像大小
        btn->setIconSize(QPixmap(QString(":/images/%1.png").arg(iconNameList[i])).size());

        // 设置网名
        btn->setText(nameList[i]);
        // btn->setText(QString("%1").arg(nameList[i]));

        // 设置为透明
        btn->setAutoRaise(true);

        // 头像+网名同时显示
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

        // 放到 layout 布局中
        ui->vlayout->addWidget(btn);

        vector.push_back(btn);

        // 初始化重复检测数组
        IsShow.push_back(false);
    }
    for(int i = 0; i < 9; i++)
    {
        connect(vector[i], &QToolButton::clicked, [=](){
            if(IsShow[i])
            {
                QMessageBox::warning(this, "警告", "该聊天框已被打开！");
                return;
            }
            IsShow[i] = true;
            Widget *widget = new Widget(nullptr, vector[i]->text());
            widget->setWindowIcon(vector[i]->icon());
            widget->setWindowTitle(vector[i]->text());
            // 设置图标大小
            // TODO
            widget->show();

            // 关闭时，将对应的IsShow变为false
            connect(widget, &Widget::closeWidget, this, [=](){
                IsShow[i] = false;
            });
        });
    }
}

login::~login()
{
    delete ui;
}
