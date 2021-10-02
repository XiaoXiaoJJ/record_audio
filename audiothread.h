#ifndef AUDIOTHREAD_H
#define AUDIOTHREAD_H
#include<QDebug>
#include <QThread>

class AudioThread : public QThread
{
    Q_OBJECT
private:
    void run();
public:
    explicit AudioThread(QObject *parent = nullptr);
    ~AudioThread();
signals:
    void timeChange(unsigned long long ms);
};

#endif // AUDIOTHREAD_H
