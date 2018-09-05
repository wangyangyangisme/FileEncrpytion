#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include "des.h"
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("LS文件加密"));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::Findfileall(DES des, QString dirname, const QString key, int mode, int endetype, bool rm)
{
    QFileInfo fileInfo;
    QDir dir(dirname);
    dir.setFilter(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot);//取得所有文件和目录的名字
    dir.setSorting(QDir::DirsFirst);//设置使用顺序为文件夹优先

    //放进链表中，但是去掉.和..目录
    QStringList infolist = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    QFileInfoList list = dir.entryInfoList();//转换为链表，用于存储父节点

    int i = 0;
    int err;

    if (!dirname.endsWith("/"))//路径最后添加斜杠
        dirname += "/";

    do{//递归查找文件
        if ( ui->checkBox_recursion->isChecked())
            fileInfo = list.at(i);
        else
            fileInfo = infolist.at(i);

        if ( fileInfo.isDir() ) //如果是目录继续递归查找
            Findfileall(des, fileInfo.filePath(), key, mode,endetype, rm);
        else
        {
            for(int m = 0; m <infolist.size(); m++)
            {
                QString fileName = infolist.at(m);
                switch (endetype)
                {
                case ENCRYPT:
                    err =  des.FileEncryption(dirname+fileName, key ,mode, rm);
                    break;
                case DECRYPT:
                {
                    if ( fileName.endsWith(".ls") )
                        err =  des.FileDecryption(dirname+fileName, key ,mode, rm);
                    break;
                }
                }

                if ( !err )
                {
                    errorname += '\n'+dirname+fileName;
                    errorval++;
                }
            }
            break;
        }
        i++;
    }while(i < list.size());

}


void MainWindow::on_opendir_button_clicked()
{
    DES des;
    QString dir=QFileDialog::getExistingDirectory(this,QString("Open"));

    if(!dir.isEmpty())
    {
        this->filename=dir;
        ui->lineEdit_2->setText(filename);
    }

}

void MainWindow::on_openfile_button_clicked()
{
    QString file=QFileDialog::getOpenFileName(this,QString("Open"));

    if(!file.isEmpty())
    {
        this->filename=file;
        ui->lineEdit_2->setText(filename);
    }
}

void MainWindow::on_Encryption_Button_clicked()
{
    DES des;
    bool status = 0;
    int mode;//根据不同的加密算法调用不同的函数

    QFileInfo text(ui->lineEdit_2->text());

    if (ui->lineEdit_2->text() != NULL && ui->lineEdit_passwd->text() != NULL)
    {
        if ( ui->lineEdit_passwd->text() != ui->lineEdit_passwd_2->text())
        {
            QMessageBox::warning(this, tr("Warning"),tr("两次密码不匹配！"));
//            return;
        }
        else if ( ui->lineEdit_passwd->text().length() <= 3 )
        {
            QMessageBox::warning(this, tr("Warning"),tr("密码长度必须大于3！"));
        }
        else if (ui->comboBox->currentText() == "DES-ECB")
        {
            mode = ECB;
        }
        else if (ui->comboBox->currentText() == "DES-CBC")
        {
            mode = CBC;
        }

        if ( text.isFile() )//判断是文件还是目录
        {
            status = des.FileEncryption(ui->lineEdit_2->text(), ui->lineEdit_passwd->text(), mode, ui->checkBox_rm->isChecked() );
        }
        else if ( text.isDir() )
        {
            errorname=' ';
            errorval = 0;
            Findfileall(des, ui->lineEdit_2->text(), ui->lineEdit_passwd->text(), mode, ENCRYPT,ui->checkBox_rm->isChecked());
            if ( errorval )
            {
                status = false;
                QMessageBox::critical(this, QObject::tr("错误"),QObject::tr("以下文件加密错误：")+errorname);
            }
            else
                status = true;
        }

        if ( status )
        {
            QMessageBox::information(this, tr("成功"),tr("加密成功！"));

        }
        else
        {
            QMessageBox::critical(this, QObject::tr("错误"),QObject::tr("加密失败："),errorname);
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"),tr("请选择需要加密的文件并且输入密码！"));
    }

}

void MainWindow::on_Decryption_Button_clicked()
{
    DES des;
    bool status;
    int mode;
    QFileInfo text(ui->lineEdit_2->text());

    if (ui->lineEdit_2->text() != NULL && ui->lineEdit_passwd->text() != NULL)
    {
        if ( ui->lineEdit_passwd->text() != ui->lineEdit_passwd_2->text())
        {
            QMessageBox::warning(this, tr("Warning"),tr("两次密码不匹配！"));
//            return;
        }
        else if (ui->comboBox->currentText() == "DES-ECB")
            mode = ECB;
        else if (ui->comboBox->currentText() == "DES-CBC")
            mode = CBC;

        if ( text.isFile() )
        {
            status = des.FileDecryption(ui->lineEdit_2->text(), ui->lineEdit_passwd->text(),mode,ui->checkBox_rm->isChecked());
        }
        else if ( text.isDir() )
        {
            errorname=' ';
            errorval = 0;
            Findfileall(des, ui->lineEdit_2->text(), ui->lineEdit_passwd->text(), mode, DECRYPT,ui->checkBox_rm->isChecked());
            if ( errorval )
            {
                status = false;
                QMessageBox::critical(this, QObject::tr("错误"),QObject::tr("以下文件解密错误："),errorname);
            }
            else
                status = true;
        }

        if ( status )
        {
            if (ui->checkBox_rm->isChecked())
                QFile::remove(ui->lineEdit_2->text());
            QMessageBox::information(this, tr("成功"),tr("解密成功！"));
        }
        else
        {
            QMessageBox::critical(this, QObject::tr("错误"),QObject::tr("解密失败："),errorname);
        }
    }
    else
    {
        QMessageBox::warning(this, tr("Warning"),tr("请选择需要解密的文件并且输入密码！"));
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QMessageBox::about(NULL,tr("关于"),"版本：v1.1<br/>作者：史前小虫<br/>编译时间：2015-6-14 14:54:01<a href='http://www.slanl.com'>http://www.slanl.com</a>");
}
