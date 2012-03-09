/*
 * Copyright © 2009 Nokia Corporation.
 */

#ifndef COMMON_H
#define COMMON_H

#include <e32base.h>
#include <es_sock.h>
#include <bttypes.h>

// structure that will hold the info about the remote device
struct TDeviceData
    {
    THostName iDeviceName;
    TBTDevAddr iDeviceAddr;
    TUint iDeviceServicePort;
    };

// for ease of use and clarity, the type definition for device data list
typedef RPointerArray<TDeviceData> TDeviceDataList;

// the service id that identifies our service.  this id will be 
// used when advertising the service and discovering the service.
#define KBT_serviceID 0x1101

// passed to RSocket::Listen, the size of listen queue
const TInt KSizeOfListenQueue = 5; 
_LIT(KRfComm,"RFCOMM");

#endif

