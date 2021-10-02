#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_audiobutton_clicked()
{
    if( !_audiothread){//点击了“开始录音”
        _audiothread=new AudioThread;
        _audiothread->start();
        connect( _audiothread,&AudioThread::finished,
                 [this](){
            _audiothread=nullptr;
            ui->audiobutton->setText("开始录音");
        });
    //将文字设置为"结束录音"
        ui->audiobutton->setText("结束录音");
    }else{//点击了"结束录音"
        //结束线程
        _audiothread->requestInterruption();
        _audiothread=nullptr;
            //将文字设置为"结束录音"
           ui->audiobutton->setText("开始录音");
    }

}
