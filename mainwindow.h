#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "des.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void Findfileall(DES des, QString dirname, const QString key, int mode, int endetype,bool rm);

private slots:
    void on_opendir_button_clicked();
    void on_openfile_button_clicked();
    void on_Encryption_Button_clicked();
    void on_Decryption_Button_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QString filename;
    QString errorname;
    int errorval;
};



#endif // MAINWINDOW_H
