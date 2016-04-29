

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QTime>
#include <stdint.h>
#include <QMutex>
#include "mavlink/common/mavlink.h"
#include "lcm_messages/geometry/pose.hpp"
#include "MavState.h"


class PositionDispatcher : public QObject
{
    Q_OBJECT
public:
    explicit PositionDispatcher(QObject *parent = 0);
    void sendPosition(int64_t ts, MavState vision, MavState sp, bool visionValid, bool spValid);
    ~PositionDispatcher();


signals:
    void finished();

public slots:



private:
    QSerialPort _serial;
    QTime _dispatchTime;
    double _xSp;
    double _ySp;
    void setSp(double x, double y);
    void _sendMavlinkMessage(mavlink_message_t *msg);

};
