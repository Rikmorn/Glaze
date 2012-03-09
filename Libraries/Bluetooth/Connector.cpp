/*
 * Copyright © 2009 Nokia Corporation.
 */
// INCLUDE FILES
#include "Connector.h"
_LIT(KRfComm,"RFCOMM");

CConnector* CConnector::NewL(MConnectorObserver& aObserver,
		RSocketServ& aSocketServ) {
	CConnector* self = CConnector::NewLC(aObserver, aSocketServ);
	CleanupStack::Pop(self);
	return self;
}

CConnector* CConnector::NewLC(MConnectorObserver& aObserver,
		RSocketServ& aSocketServ) {
	CConnector* self = new (ELeave) CConnector(aObserver, aSocketServ);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

void CConnector::ConstructL() {
}

CConnector::CConnector(MConnectorObserver& aObserver, RSocketServ& aSocketServ) :
	CActive(CActive::EPriorityStandard), iObserver(aObserver), iSocketServ(
			aSocketServ), iState(ENone) {
	CActiveScheduler::Add(this);
}

CConnector::~CConnector() {
	Cancel();
	// disconnect and kill socket
	Disconnect();
}

void CConnector::DoCancel() {
	iSock.CancelAll();
}

// ----------------------------------------------------------------------------
// create a connection to given address on given port.  
// ----------------------------------------------------------------------------
TRequestStatus CConnector::ConnectL(THostName aName, TBTDevAddr aAddr,
		TInt aPort) {
	iName = aName;
	iAddr = aAddr;
	iPort = aPort;

	// load protocol, RFCOMM
	TProtocolDesc pdesc;
	User::LeaveIfError(iSocketServ.FindProtocol(KRfComm(), pdesc));

	// open socket
	User::LeaveIfError(iSock.Open(iSocketServ, KRfComm));
	// set address and port
	TBTSockAddr addr;
	addr.SetBTAddr(iAddr);
	addr.SetPort(iPort);

	// connect socket
	TRequestStatus status;
	iSock.Connect(addr, status);
	User::WaitForRequest(status);
	if (status != KErrNone) {
		// error opening conn
		return status;
	}

	iState = EConnecting;
	WaitAndReceive();
	return status;
}

// ----------------------------------------------------------------------------
// disconnect from remote device, shutdown connected socket
// ----------------------------------------------------------------------------
void CConnector::Disconnect() {
	TRequestStatus status;
	// shutdown socket
	if (iState == ENone) {
		return;
	}
	iSock.Shutdown(RSocket::ENormal, status);
	User::WaitForRequest(status);
	iSock.Close();
}

// ----------------------------------------------------------------------------
// send given data to remote device, write to connected socket
// ----------------------------------------------------------------------------
void CConnector::SendData(const TDesC8& aData) {
	// cancel any read requests on socket
	iSock.CancelRead();
	Cancel();
	// send message
	iState = ESending;
	iSock.Write(aData, iStatus);
	SetActive();
}

// ----------------------------------------------------------------------------
// wait for and receive data from remote device, read connected socket
// ----------------------------------------------------------------------------
void CConnector::WaitAndReceive() {
	// cancel pending operations
	iSock.CancelRead();
	Cancel();
	// receive data from socket
	iState = EWaiting;
	iSock.RecvOneOrMore(iBuffer, 0, iStatus, iLen);
	SetActive();
}

void CConnector::RunL() {
	if (iStatus != KErrNone) {
		iObserver.HandleConnectorErrorL(iName, iStatus.Int());
		return;
	}

	switch (iState) {
	case EConnecting: {
		// wait incoming data on socket
		WaitAndReceive();
		break;
	}
	case EWaiting: {
		// we got incoming data!
		HBufC* text = HBufC::NewLC(iBuffer.Length());
		text->Des().Copy(iBuffer);
		// observer will handle data
		HandleConnectorDataReceivedL(iName, *text);
		CleanupStack::PopAndDestroy(text);

		// start expecting new incoming data
		WaitAndReceive();
		break;
	}
	case ESending: {
		// tried to send a message
		if (iState != KErrNone) {
			// Add error handling / socket re-read code
			// here, not implemented in this example
		}

		// start expecting new incoming data
		WaitAndReceive();
		break;
	}
	default:
		break;
	}
}

TInt CConnector::RunError(TInt /*aError*/) {
	// Add error handling here, not implemented in this example
	return KErrNone;
}

// ----------------------------------------------------------------------------
// a callback to observer indicating that connector has received data
// ----------------------------------------------------------------------------
void CConnector::HandleConnectorDataReceivedL(THostName aName,
		const TDesC& aData) {
	iObserver.HandleConnectorDataReceivedL(aName, aData);
}

