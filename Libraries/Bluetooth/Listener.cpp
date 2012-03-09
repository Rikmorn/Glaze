/*
 * Copyright © 2009 Nokia Corporation.
 */

// INCLUDE FILES
#include "Listener.h"

CListener* CListener::NewL(MListenerObserver& aObserver,
		RSocketServ& aSocketServ) {
	CListener* self = CListener::NewLC(aObserver, aSocketServ);
	CleanupStack::Pop(self);
	return self;
}

CListener* CListener::NewLC(MListenerObserver& aObserver,
		RSocketServ& aSocketServ) {
	CListener* self = new (ELeave) CListener(aObserver, aSocketServ);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}

void CListener::ConstructL() {
}

CListener::CListener(MListenerObserver& aObserver, RSocketServ& aSocketServ) :
	CActive(CActive::EPriorityStandard), iObserver(aObserver), iSocketServ(
			aSocketServ), iIsConnected(EFalse), iState(ENone) {
	CActiveScheduler::Add(this);
}

CListener::~CListener() {
	// cancel active object
	Cancel();
	// close sockets
	StopListener();
}

// ----------------------------------------------------------------------------
// listener will open a listening socket on a channel (port)
// gotten from GetOpt() call.  
// ----------------------------------------------------------------------------
void CListener::StartListenerL(TInt& aChannel) {
	// get out if we're already running..
	if (iState != ENone) {
		User::Leave(KErrInUse);
	}

	// set this active object to connecting state
	iState = EConnecting;

	// load protocol, RFCOMM
	TProtocolDesc pdesc;
	User::LeaveIfError(iSocketServ.FindProtocol(KRfComm(), pdesc));

	// open a socket
	User::LeaveIfError(iListenSock.Open(iSocketServ, pdesc.iAddrFamily,
			pdesc.iSockType, KRFCOMM));

	// get listening channel
	User::LeaveIfError(iListenSock.GetOpt(KRFCOMMGetAvailableServerChannel,
			KSolBtRFCOMM, aChannel));

	// bluetooth socket address object
	TBTSockAddr btsockaddr;
	btsockaddr.SetPort(aChannel);
	// bind socket
	User::LeaveIfError(iListenSock.Bind(btsockaddr));
	// listen on port
	iListenSock.Listen(KSizeOfListenQueue);

	//set security
	TBTServiceSecurity secSettings;

	TUid settingsUID;
	settingsUID.iUid = KBT_serviceID; // change
	secSettings.SetUid(settingsUID);
	secSettings.SetAuthentication(EFalse);
	secSettings.SetAuthorisation(EFalse);
	secSettings.SetEncryption(EFalse);

	// attach the security settings.
	btsockaddr.SetSecurity(secSettings);

	// close old accepted socket if open
	iSock.Close();

	// open blank socket and pass it to accept to be assigned a proper
	// socket upon completion of Accept()
	User::LeaveIfError(iSock.Open(iSocketServ));

	// set to accept incoming connections, active object will handle
	iListenSock.Accept(iSock, iStatus);
	SetActive();
}

// ----------------------------------------------------------------------------
// CListener::SetSecurityL(TInt aChannel)
//
// sets the security of given bluetooth channel.  these settings will turn
// off authentication, authorisation and encryption on given channel.
// 
// Not supported in 2nd ed. FP2 and newer versions.
// Left here just for comparing.
// ----------------------------------------------------------------------------
void CListener::SetSecurityL(TInt /*aChannel*/) {
	//this is not supported in 2nd ed. FP2 and above:
	//setup channel security:
	//TRequestStatus status;
	//RBTMan secManager;

	//RBTSecuritySettings secDatabase;
	//TBTServiceSecurity secSettings;
	// connect to security manager
	//User::LeaveIfError(secManager.Connect());
	//CleanupClosePushL(secManager);
	//User::LeaveIfError(secDatabase.Open(secManager));
	//CleanupClosePushL(secDatabase);
	// setup security
	//TUid settingsUID;
	//settingsUID.iUid = KBT_serviceID;
	//secSettings.SetUid(settingsUID);
	//secSettings.SetChannelID(aChannel);
	//secSettings.SetProtocolID(KSolBtRFCOMM);
	//secSettings.SetAuthentication(EFalse);
	//secSettings.SetAuthorisation(EFalse);
	//secSettings.SetEncryption(EFalse);
	// register settings with security
	//secDatabase.RegisterService(secSettings, status);
	//User::WaitForRequest(status);
	//CleanupStack::PopAndDestroy(2,&secManager);//SecDatabase, secManager
}

// ----------------------------------------------------------------------------
// stops the listener by closing the listening socket
// ----------------------------------------------------------------------------
void CListener::StopListener() {
	// kill sockets
	if (iState != ENone) {
		iSock.Close();
		iListenSock.Close();
		iState = ENone;
	}
}

// ----------------------------------------------------------------------------
// receive more data from listening socket, asynchronously.  
// ----------------------------------------------------------------------------
void CListener::ReceiveData() {
	// set state to waiting - for RunL()
	iState = EWaiting;
	// make async request
	iSock.RecvOneOrMore(iBuffer, 0, iStatus, iLen);
	// set as active to get the async req response (iState) in RunL()
	SetActive();
}

// ----------------------------------------------------------------------------
// send data to remote device, write data to socket.
// ----------------------------------------------------------------------------
void CListener::SendData(const TDesC8& aData) {
	if (iState != EWaiting)
		return;
	// cancel any read requests on socket
	Cancel();
	// try to send message by writing to socket
	// - set the state of this active object to "sending"
	iState = ESending;
	// - make async socket write request
	iSock.Write(aData, iStatus);
	// - start waiting async req response (iState) from active scheduler
	SetActive();
}

void CListener::RunL() {
	if (iStatus != KErrNone) {
		StopListener();
		HandleListenerDisconnectedL();
		return;
	}

	switch (iState) {
	case EConnecting: {
		// connected listening socket!
		HandleListenerConnectedL();
		ReceiveData();
		break;
	}
	case EWaiting: {
		// returned from receiving data
		if (iState != KErrNone) {
			// add the error handling / re-reading code here..
			// not needed in this example
		}

		HBufC* text = HBufC::NewLC(iBuffer.Length());
		text->Des().Copy(iBuffer);
		// observer will handle data
		HandleListenerDataReceivedL(*text);
		CleanupStack::PopAndDestroy(text);
		// start expecting next data to be read
		ReceiveData();
		break;
	}
	case ESending: {
		// returned from sending the date, check the state
		if (iState != KErrNone) {
			// add the error handling / re-sending code here..
			// not needed in this example
		}

		// start expecting next data to be read
		ReceiveData();
		break;
	}
	default:
		break;
	}
}

TInt CListener::RunError(TInt /*aError*/) {
	// add the error handling
	return KErrNone;
}

void CListener::DoCancel() {
	// cancel all pending socket operations
	iSock.CancelAll();
	iListenSock.CancelAll();
}

// ----------------------------------------------------------------------------
// returns true if listener has been connected to
// ----------------------------------------------------------------------------
TBool CListener::IsConnected() {
	return iIsConnected;
}

// ----------------------------------------------------------------------------
// a callback to observer indicating that listener has received data
// ----------------------------------------------------------------------------
void CListener::HandleListenerDataReceivedL(const TDesC& aData) {
	iObserver.HandleListenerDataReceivedL(aData);
}

// ----------------------------------------------------------------------------
// a callback to observer indicating that listener has been connected to
// ----------------------------------------------------------------------------
void CListener::HandleListenerConnectedL() {
	iIsConnected = ETrue;
	iObserver.HandleListenerConnectedL();
}

// ----------------------------------------------------------------------------
// a callback to observer indicating that listener has been disconnected
// ----------------------------------------------------------------------------
void CListener::HandleListenerDisconnectedL() {
	iIsConnected = EFalse;
	iObserver.HandleListenerDisconnectedL();
}

