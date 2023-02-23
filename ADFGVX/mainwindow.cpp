#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include <bits/stdc++.h>

/* 变量定义 */
QString s_AfterDeal, s_ShKey, s_BeforeDeal, s_DeKey;
QString s_Key = "ADFGVX";
QString eu = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int order[24] = {};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_ShKey->setPlaceholderText("由“a-zA-Z”构成的各位不重复字符串");            //将移位密钥的格式要求作为其输入框的占位字符
    ui->plainTextEdit_BeforeDeal->setPlaceholderText("由“0-9a-zA-Z”构成的字符串");  //将密文的格式要求作为其输入框的占位字符
    ui->plainTextEdit_DeKey->setPlaceholderText("由“0-9a-zA-Z”构成的36位各位不重复字符串");   //将分解密钥的格式要求作为其输入框的占位字符
}

MainWindow::~MainWindow()
{
    delete ui;
}

void getOrder(){       //获得移位密钥的排序顺序
    int t = -1;
    QString shKey = s_ShKey.toUpper();
    for (int i=0;i<26;i++){
        if (shKey.indexOf(eu[i],0) != -1){
            order[++t] = shKey.indexOf(eu[i],0);
        }
    }
}

void doEncrypt(){        //加密处理
    QString s = "";
    char *tmpcs,tmpc;
    int pos,i,j;

    /* 扫描未处理的密文，将其进行加密 */
    for (i=0;i<s_BeforeDeal.length();i++){
        /* 获取密文第i个字符 */
        tmpcs = s_BeforeDeal.toLatin1().data();
        tmpc = tmpcs[i];
        if (tmpc >= 'A' && tmpc <= 'Z') tmpc = tmpc  + 'a' - 'A';
        /* 密文第i个字符按分解密钥分解 */
        if ((tmpc >= '0' && tmpc <= '9')||(tmpc >= 'a' && tmpc <= 'z')){
            pos = s_DeKey.indexOf(tmpc,0);
            s = s + s_Key[pos/6] + s_Key[pos%6];
        }
    }

    /* 最后一行如不满，用字符“_”填充 */
    while (s.length()%s_ShKey.length() != 0){
        s += "_";
    }

    /* 根据移位密钥将密文进一步加密 */
    for (i=0;i<s_ShKey.length();i++){
        j = 0;
        while (order[i]+j*s_ShKey.length() < s.length()){
            s_AfterDeal += s[order[i]+j*s_ShKey.length()];
            j++;
        }
    }
}

void doDecrypt(){        //解密处理
    QString s = "";
    QString tmp;
    int i,j;
    s_BeforeDeal = s_BeforeDeal.toUpper();

    /* 根据分解密钥获得分解顺序 */
    int ordertmp[24] = {};
    for (i=0;i<s_ShKey.length();i++){
        ordertmp[order[i]] = i;
    }

    /* 根据移位密钥将密文解密 */
    for (i=0;i<s_BeforeDeal.length()/s_ShKey.length();i++){
        for (j=0;j<s_ShKey.length();j++){
            tmp = s_BeforeDeal[ordertmp[j]*(s_BeforeDeal.length()/s_ShKey.length())+i];
            if (tmp != "_") s += tmp;
        }
    }

    /* 根据分解密钥将密文进一步解密 */
    for (i=0;i<s.length();i+=2){
        s_AfterDeal += s_DeKey[s_Key.indexOf(s[i],0)*6+s_Key.indexOf(s[i+1],0)];
    }
}

void MainWindow::on_button_run_clicked() //单击“Run”按钮触发函数
{
    /* 变量初始化 */
    s_AfterDeal = "";
    s_ShKey = ui->lineEdit_ShKey->text();
    s_BeforeDeal = ui->plainTextEdit_BeforeDeal->toPlainText();
    s_DeKey = ui->plainTextEdit_DeKey->toPlainText();

    /* ！对密钥和待处理密文进行合法性处理 */
    s_DeKey = s_DeKey.simplified();
    s_BeforeDeal = s_BeforeDeal.simplified();

    /* 获得移位密钥的排序顺序 */
    getOrder();

    if(ui->encrypt->isChecked()){          //加密状态
        doEncrypt();
    } else if(ui->decrypt->isChecked()){   //解密状态
        doDecrypt();
    }

    /* 将处理后的密文内容回显在窗口中 */
    ui->plainTextEdit__AfterDeal->setPlainText(s_AfterDeal);

    /* 将输入框恢复成默认状态 */
    ui->lineEdit_ShKey->clear();
    //ui->lineEdit_ShKey->setEchoMode(QLineEdit::Normal); //将移位密钥输入框设置为“Normal”格式
    ui->plainTextEdit_BeforeDeal->clear();
    ui->plainTextEdit_DeKey->clear();

}

