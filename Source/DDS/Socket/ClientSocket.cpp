// Copyright © Team DDS 2025. Project DDS™ is a trademark of Team DDS. All Rights Reserved.


#include "ClientSocket.h"

#include "Sockets.h"
#include "SocketSubsystem.h"
#include "ETC/CustomLog.h"
#include "Interfaces/IPv4/IPv4Address.h"

FSocketReceivedData UClientSocket::CreateSocket(const FString RequestMessage, const FString& ExtraInfo, bool bIsLocal)
{
	// 소켓 생성
	FSocket* Socket;
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->
		CreateSocket(NAME_Stream, "TCPSocket", false);

	// RAII 방식으로 소켓 종료
	ON_SCOPE_EXIT
	{
		Socket->Close();
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
	};
	
	// IP 연결
	FIPv4Address IPv4;
	if(!bIsLocal)
	{
		FIPv4Address::Parse(ServerIP, IPv4);
	}
	else
	{
		FIPv4Address::Parse("127.0.0.10", IPv4);
	}

	// 요청을 날릴 IP와 Port 연결 (MainServer)
	TSharedRef<FInternetAddr> Address = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	Address->SetIp(IPv4.Value);
	Address->SetPort(7777);

	// 소켓 연결
	bool bConnected = Socket->Connect(*Address);
	if(bConnected)
	{
		// 전송할 메세지 
		int32 BytesSent = 0;
		FString ReqMessage;

		// 요청사항|추가정보 형식으로 메세지 생성
		// ex) Client_JoinPrivateRoom|DDSRoom
		if(!ExtraInfo.IsEmpty()) ReqMessage = RequestMessage + "|" + ExtraInfo;
		else ReqMessage = RequestMessage;
		MY_LOG(LogTemp, Warning, TEXT("Request Message to MainServer = %s"), *ReqMessage);

		// 생성된 메세지 전송한다
		const TCHAR* SerializedChar  = ReqMessage.GetCharArray().GetData();
		Socket->Send(reinterpret_cast<uint8*>(TCHAR_TO_UTF8(SerializedChar)), FCString::Strlen(SerializedChar), BytesSent);

		// 수신할 메세지
		uint8 ReceiveBuf[1000] = {0};
		int32 BytesRead = 0;
		Socket->Recv(ReceiveBuf, sizeof(ReceiveBuf), BytesRead);

		// 수신 성공했을경우
		if(BytesRead > 0)
		{
			FString ReceivedMessage = UTF8_TO_TCHAR(reinterpret_cast<const char*>(ReceiveBuf));
			UE_LOG(LogTemp, Log, TEXT("Recieved Message from MainServer : %s"), *ReceivedMessage);

			if(RequestMessage != "")
			{
				if(ReceivedMessage != "")
				{
					TArray<FString> ReceivedData = StringTokenizer(ReceivedMessage);
					return FSocketReceivedData(ReceivedData);
				}
			}
		}
	}
	else
	{
		MY_LOG(LogTemp, Error, TEXT("Failed to Connect MainServer"));
	}

	return FSocketReceivedData();
}

TArray<FString> UClientSocket::StringTokenizer(FString String)
{
	TArray<FString> Tokens;
	String.ParseIntoArray(Tokens, TEXT("|"), true);
	return Tokens;
}
