#include "audiothread.h"
#include<QDebug>
#include<QFile>
#include<QDateTime>
#include"ffmpegs.h"
extern "C"{
//设备
#include<libavdevice/avdevice.h>
//格式
#include<libavformat/avformat.h>
//工具
#include<libavutil/avutil.h>
}

#ifdef Q_OS_WIN
    //格式名称
    #define FMT_NAME "dshow"
    //设备名称
    #define DEVICE_NAME "audio=麦克风 (Realtek Audio)"
    //pcm文件名
    #define FILEPATH "F:/Dev/"
#else
    #define FMT_NAME "avfoundation"
    #define DEVICE—NAME ":0"
    #define FILE_NAME "usr/Desktop"
#endif
AudioThread::AudioThread(QObject *parent) : QThread(parent)
{
    //监听线程结束时自动发出的信号 然后回收内存
    connect(this,&AudioThread::finished,
            this,&AudioThread::deleteLater);
}
AudioThread::~AudioThread(){
    //结束线程的监听
    disconnect();
    //结束线程
    requestInterruption();
    //让系统等待一下 程序执行完上述操作
    quit();
    wait();
    qDebug() <<this<<"析构（内存被回收）";
}
//当线程启动的时候，就会自动调用run
//run函数中的代码在子线程中执行
//耗时操作应该放在run函数中
void AudioThread::run(){
    //获取输入格式对象
    AVInputFormat *fmt=av_find_input_format(FMT_NAME);
    if(!fmt){
        qDebug() <<"获取输入格式失败"<<FMT_NAME;
        return;
    }
    //格式上下文（将来可以利用上下文来操作设备）
    AVFormatContext *ctx=nullptr;
    //打开设备
    int ret = avformat_open_input(&ctx, DEVICE_NAME, fmt, nullptr);
    if(ret<0){
        char errorbuf[1024]={0};
        av_strerror(ret,errorbuf,sizeof(errorbuf));
        qDebug() <<"打开设备失败"<<errorbuf;
        return;
    }
    //文件名
    QString filename=FILEPATH;
    filename +=  QDateTime::currentDateTime().toString("yy_MM_dd_HH_mm");
    filename += ".pcm";
    //打开文件
    QFile file(filename);
    //WriteOnly:只写模式 如果文件不存在 就创建文件 如果存在 就清空文件
    if(!file.open(QFile::WriteOnly)){
        qDebug() <<"打开文件失败"<<filename;
        //关闭设备
        avformat_close_input(&ctx);
        return;
    }

    AVPacket pkt;
    //不断采集数据
    while(!isInterruptionRequested() && av_read_frame(ctx, &pkt) == 0){
        file.write((const char *)pkt.data, pkt.size);
    }
    //释放资源
    //关闭文件
    file.close();
    //关闭设备
    avformat_close_input(&ctx);

}


