/*
 * Copyright © 2009 Nokia Corporation.
 */

#ifndef LISTENER_H
#define LISTENER_H

// INCLUDES
#include <e32base.h>

#include <es_sock.h>
#include <btdevice.h>
#include <bt_sock.h>
#include <btmanclient.h>

#include "Common.h"

const TInt KReceivedBufferSize = 40;

class MListenerObserver {

public:
	//Observer interface for handling listener data reception event.
	virtual void HandleListenerDataReceivedL(const TDesC& aData)=0;

	//Observer interface for handling listener connection event.
	virtual void HandleListenerConnectedL()=0;

	//Observer interface for handling disconnection event.
	virtual void HandleListenerDisconnectedL()=0;
};

class CListener: public CActive {
public:

	static CListener* NewL(MListenerObserver& aObserver, RSocketServ& aSocketServ);

	static CListener* NewLC(MListenerObserver& aObserver, RSocketServ& aSocketServ);

	~CListener();

	/*
	 * Starts the listener.  listener will open a listening socket and
	 * listen on incoming connections.
	 * aChannel the communication channel (port) the listener listens to.
	 */
	void StartListenerL(TInt& aChannel);

	//Stops the listener, closes the listening socket.
	void StopListener();

	/*
	 * Sends data to master.  user will be prompted to enter the message
	 *  to send.
	 * aData the message to be send.
	 */
	void SendData(const TDesC8& aData);

	/*!
	 * check if listener is connected to.
	 * return true if listener has a connection from master.
	 */
	TBool IsConnected();

protected:

	//Handle the active objects request completion.
	void RunL();

	//Cancels the outstanding request on active object.
	void DoCancel();

	TInt RunError(TInt aError);

private:

	CListener(MListenerObserver& aObserver, RSocketServ& aSocketServ);

	void ConstructL();

	//Receive data from listening socket.
	void ReceiveData();

	//Sets the security of the bluetooth comm channel.
	void SetSecurityL(TInt /*aChannel*/);

	/*!
	 * Handles the data received from a master.  the handling request
	 * will be passed onto handling observer.
	 * aData the data received.
	 */
	void HandleListenerDataReceivedL(const TDesC& aData);

	/*
	 * Handles the event of slave connection (slave was connected to
	 * by master).  the handling request will be passed onto handling observer.
	 */
	void HandleListenerConnectedL();

	/*
	 * Handles the event of slave disconnection, the master connection
	 * to slave was terminated.  the handling request will be passed onto handling
	 * observer.
	 */
	void HandleListenerDisconnectedL();

public:
	// listening socket
	RSocket iListenSock;
	// accepted socket
	RSocket iSock;
	// length of received data
	TSockXfrLength iLen;
	// buffer holding received data
	TBuf8<KReceivedBufferSize> iBuffer;
	// observer reference
	MListenerObserver& iObserver;
	// socket server handle
	RSocketServ& iSocketServ;
	// listener connection status
	TBool iIsConnected;
	// the state of the listener
	enum TState {
		ENone = 1, EConnecting, EWaiting, ESending
	};

	TState iState;
};

#endif
