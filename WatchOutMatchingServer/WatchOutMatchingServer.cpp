#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "WatchOutMatchingServer.h"
#include "../../libppnetwork/libppnetwork/PPRecvPacketPoolServer.h"						//서버 구동시 필요합니다. 싱글톤 객체.

PP::WatchOutMatchingServer::WatchOutMatchingServer() {}
PP::WatchOutMatchingServer::~WatchOutMatchingServer() {}

int PP::WatchOutMatchingServer::Init() {
	m_pHead = new PP::PPSequence;
	m_pServer = GetServer();
	m_pSender = GetSender();
	m_pServer->SetPortNumber(10001);
	m_pServer->SetNumberOfThreads(2);
	m_pServer->SetFP(std::bind(&WatchOutMatchingServer::ProcessPacket, this));
	return 0;
}

int PP::WatchOutMatchingServer::Run() {
	m_pServer->Startup();
	while (true) {
		Sleep(1000);
	}
	return 0;
}

int PP::WatchOutMatchingServer::Release() {
	m_pServer->Release();
	delete m_pServer;
	delete m_pSender;
	return 0;
}

int PP::WatchOutMatchingServer::ProcessPacket() {
	//패킷을 라이브러리 외부에서 처리하는 함수입니다.
	//서버 객체에서 Startup 실행 전 SetFP()를 실행해야 합니다.
	//std::wcout << "injected ProcessServerPacket()..." << std::endl;
	PP::PPSender* pSender = PP::GetSender();
	PP::PPPacketForProcess packetRecv = {};
	PP::PPPacketForProcess packetSend = {};
	//IOCP 스레드에서 넣엇던 패킷을 담은 패킷풀 접근
	packetRecv = PP::PPRecvPacketPoolServer::GetInstance().front();
	//패킷풀 맨 앞 pop()
	PP::PPRecvPacketPoolServer::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (packetRecv.m_Packet.m_Header.m_type) {
		//클라이언트에게 문자열 패킷을 수신받을 때 처리하는 소스
	case PP::PPPacketType::TYPE_STRING: {
		std::wstring wstrBuf;
		PP::PPPacketMessage* ppacketRecvMsg = (PP::PPPacketMessage*)packetRecv.m_Packet.m_Payload;
		PP::PPPacketMessage packetSendMsg = {};
		int iSizeOfpakcetSendMsg;

		//패킷에서 꺼낸 문자열을 서버의 콘솔창으로 출력하는 소스
		wcharBuf = (wchar_t*)&ppacketRecvMsg->m_charMessage;
		wstrBuf = std::to_wstring(packetRecv.m_socketSession);
		wstrBuf.append(L" socket Broadcasting :");
		wstrBuf.append(wcharBuf);
		wstrBuf.append(L"\n");
		std::wcout << packetRecv.m_socketSession << L" socket: " << wcharBuf << std::endl;
		OutputDebugStringW(wstrBuf.c_str());

		//구조체 packetSendMsg 작성
		iSizeOfpakcetSendMsg = (int)wstrBuf.size() * 2;
		memcpy(packetSendMsg.m_charMessage, wstrBuf.c_str(), iSizeOfpakcetSendMsg);
		//보낼 패킷 작성
		packetSend.m_socketSession = packetRecv.m_socketSession;							//PPPacketForProcess::m_socketSession으로 패킷 수신자 또는 송신자를 지정할 수 있다.
		packetSend.m_Mode = PP::PPPacketMode::SEND;											//현재 아무 기능에 관여하지 않는 변수다.
		memcpy(packetSend.m_Packet.m_Payload,												//패킷 적재부 작성
			(void*)&packetSendMsg, iSizeOfpakcetSendMsg);									//패킷 적재부에 memcpy로 적재할 구조체를 deep copy해서 입력하면 된다.		
		packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iSizeOfpakcetSendMsg;		//패킷 헤더길이 4바이트 + 적재부 길이를 합친 총 길이
		packetSend.m_Packet.m_Header.m_type = PP::PPPacketType::TYPE_STRING;				//패킷 타입. 열거형 변수 PP::PPPacketType에 정의되어있다.

		pSender->Broadcast(packetSend);														//PPSessionManager에 있는 모든 세션들을 순회하여 send를 실시함.
		break;
	}
	case PP::PPPacketType::TYPE_REQ_SOCKET: {
		PP::PPPacketNoticeSession packetSession = {};
		int iSizeOfPayload = sizeof(packetSession);
		packetSession.m_socketSession = packetRecv.m_socketSession;
		packetSend.m_socketSession = packetRecv.m_socketSession;
		packetSend.m_Mode = PP::PPPacketMode::SEND;
		memcpy(packetSend.m_Packet.m_Payload,												//패킷 적재부 작성
			(void*)&packetSession, iSizeOfPayload);									//패킷 적재부에 memcpy로 적재할 구조체를 deep copy해서 입력하면 된다.		
		packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iSizeOfPayload;		//패킷 헤더길이 4바이트 + 적재부 길이를 합친 총 길이
		packetSend.m_Packet.m_Header.m_type = PP::PPPacketType::TYPE_ACK_SOCKET;

		pSender->Send(packetSend);
		break;
	}

	case PP::PPAdditionalPacketType::TYPE_REQ_MATCHING: {
		PP::PPPacketReqMatching* ppacketPayload = (PP::PPPacketReqMatching*)packetRecv.m_Packet.m_Payload;
		int iMaximumPlayer = ppacketPayload->iMaximumPlayer;

		std::wcout << L"매칭 요청 들어옴" << std::endl;
		
		//최대 플레이 인원수와 맞는 방 탐색
		auto iterGroup = m_pHead->listChildren.find(iMaximumPlayer);
		if (iterGroup == m_pHead->listChildren.end()) {
			PP::PPGroup* pGroup = new PP::PPGroup;
			pGroup->m_iMaximumPlayer = iMaximumPlayer;
			pGroup->listSession.push_back(packetRecv.m_socketSession);
			m_pHead->listChildren.insert(std::make_pair(iMaximumPlayer, pGroup));
			std::wcout << L"새 그룹 생성" << std::endl;
			break;
		}
		else {
			std::wcout << L"그룹 찾음" << std::endl;
			iterGroup->second->listSession.push_back(packetRecv.m_socketSession);
			int iReturn;
			int iNumOfHost = 1;
			int iNumOfGuest = iMaximumPlayer - 1;
			std::string strHostIPv4;
			if (iterGroup->second->listSession.size() >= iMaximumPlayer) {
				std::wcout << L"매칭 시도" << std::endl;
				std::wcout << L"호스트를 지정합니다." << std::endl;
				SOCKET socketHost = iterGroup->second->listSession.front();
				iterGroup->second->listSession.pop_front();
				sockaddr_in saHost;
				int iSizeOfsaHost = sizeof(saHost);
				iReturn = getpeername(socketHost, (sockaddr*)&saHost, &iSizeOfsaHost);
				strHostIPv4 = inet_ntoa(saHost.sin_addr);
				packetSend.m_Mode = PP::PPPacketMode::SEND;
				packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE;
				packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PP::PPAdditionalPacketType::TYPE_ACK_MATCHING_HOST;
				packetSend.m_socketSession = socketHost;
				iReturn = m_pSender->Send(packetSend);
				Sleep(1);
				if (iReturn != 0) {
					std::wcout << L"호스트 지정 실패" << std::endl;
					break;
				}

				while (iNumOfGuest != 0) {
					std::wcout << L"게스트를 지정합니다." << std::endl;
					packetSend = {};
					SOCKET socketGuest = iterGroup->second->listSession.front();
					PP::PPPacketAckMatchingGuest packetMatching = {};
					iterGroup->second->listSession.pop_front();
					memcpy(packetMatching.charHostAddress, strHostIPv4.c_str(), strHostIPv4.size());
					memcpy(packetSend.m_Packet.m_Payload, &packetMatching, sizeof(packetMatching));
					packetSend.m_Mode = PP::PPPacketMode::SEND;
					packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + sizeof(packetMatching);
					packetSend.m_Packet.m_Header.m_type = (PP::PPPacketType)PP::PPAdditionalPacketType::TYPE_ACK_MATCHING_GUEST;
					packetSend.m_socketSession = socketGuest;
					iReturn = m_pSender->Send(packetSend);

					iNumOfGuest--;
				}
				
			}
			break;
		}
	}
	default:
		//정의되지 않은 패킷 처리부입니다.
		wcharBuf = (wchar_t*)&packetRecv;
		std::wcout << wcharBuf;
		packetSend = packetRecv;
		pSender->Broadcast(packetSend);
		break;
	}
	return 0;
}
