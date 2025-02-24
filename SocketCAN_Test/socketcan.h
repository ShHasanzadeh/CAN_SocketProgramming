#ifndef SOCKETCAN_H
#define SOCKETCAN_H

#include "constants.h"
#include <reportresult.h>

#include <QObject>
#include <QTimer>

/* Linux Kernel libraries for socketCAN */
#include <QDateTime>
#include <linux/can.h>
#include <linux/can/error.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

/*  Read() Kernel API lib */
#include <unistd.h>

/* new_Defined_Socket_CAN */
#include <QDebug>
#include <QProcess>
#include <QString>
#include <cstring>
#include <iostream>
#include <string>

/* Queue & Mutex Method */
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

/*  to set Filter IDs */
#include <QTextStream>
#include <iomanip>
#include <set>

/* to set queue map msg */
#include <cstdint>
#include <functional>
#include <unordered_map>

/* to use atomic bool flag */
#include <atomic>

/* to manage Linux Command */
//#include <cstdio>
//#include <sstream>

/* Non-Blocking  Socket CAN Mode  */
#include <fcntl.h>

/* to use memory managment */
#include <memory>

struct CanMessage {
  uint32_t id : 29;
  uint8_t dlc : 4;
  uint8_t data[8];
  uint8_t CAN_Channel : 1;
  uint8_t type : 1;
  /*
   * @type 0 is Stanard
   * @type 1 is Extended
   */
};



class SocketCAN : public QObject {
  Q_OBJECT
public:
  /* constructor need to be to avoid Implicit conversions in QObject */
  explicit SocketCAN(QObject *parent = nullptr);

  /* destructor */
  ~SocketCAN();

  /* to Iniet The CAN Socket Just For Once At The Start Time */
  void canInit(int init_canChannel);
  int getSocket() const; // use const to avoid changesthe Sockets

  
signals:

  void CANChangeStatusSignal(int canchanel_number, Status newstatus);

public slots:


private:
  /* to init the CAN Interface By SocketCAN Object */
  int socketCAN_OBD;
  std::string init_can_interface;
  std::atomic<bool> init_stopFlage{false};

};

#endif // SOCKETCAN_H
