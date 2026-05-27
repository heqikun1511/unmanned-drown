#ifndef COM_CONFIG_H
#define COM_CONFIG_H
#include "main.h"
typedef enum{
    REMOTE_CONNECT=0,
    REMOTE_DISCONNECT,
}Remotestate;

typedef enum{
    IDLE=0,
    NORMAL,
    FIX_HEIGHT,
    FAIL,
}flightstate;

#endif 