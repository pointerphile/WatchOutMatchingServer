#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "WatchOutMatchingServer.h"
#include "../../libppnetwork/libppnetwork/PPRecvPacketPoolServer.h"						//���� ������ �ʿ��մϴ�. �̱��� ��ü.

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
	//��Ŷ�� ���̺귯�� �ܺο��� ó���ϴ� �Լ��Դϴ�.
	//���� ��ü���� Startup ���� �� SetFP()�� �����ؾ� �մϴ�.
	//std::wcout << "injected ProcessServerPacket()..." << std::endl;
	PP::PPSender* pSender = PP::GetSender();
	PP::PPPacketForProcess packetRecv = {};
	PP::PPPacketForProcess packetSend = {};
	//IOCP �����忡�� �־��� ��Ŷ�� ���� ��ŶǮ ����
	packetRecv = PP::PPRecvPacketPoolServer::GetInstance().front();
	//��ŶǮ �� �� pop()
	PP::PPRecvPacketPoolServer::GetInstance().pop_front();
	wchar_t* wcharBuf = nullptr;
	switch (packetRecv.m_Packet.m_Header.m_type) {
		//Ŭ���̾�Ʈ���� ���ڿ� ��Ŷ�� ���Ź��� �� ó���ϴ� �ҽ�
	case PP::PPPacketType::TYPE_STRING: {
		std::wstring wstrBuf;
		PP::PPPacketMessage* ppacketRecvMsg = (PP::PPPacketMessage*)packetRecv.m_Packet.m_Payload;
		PP::PPPacketMessage packetSendMsg = {};
		int iSizeOfpakcetSendMsg;

		//��Ŷ���� ���� ���ڿ��� ������ �ܼ�â���� ����ϴ� �ҽ�
		wcharBuf = (wchar_t*)&ppacketRecvMsg->m_charMessage;
		wstrBuf = std::to_wstring(packetRecv.m_socketSession);
		wstrBuf.append(L" socket Broadcasting :");
		wstrBuf.append(wcharBuf);
		wstrBuf.append(L"\n");
		std::wcout << packetRecv.m_socketSession << L" socket: " << wcharBuf << std::endl;
		OutputDebugStringW(wstrBuf.c_str());

		//����ü packetSendMsg �ۼ�
		iSizeOfpakcetSendMsg = (int)wstrBuf.size() * 2;
		memcpy(packetSendMsg.m_charMessage, wstrBuf.c_str(), iSizeOfpakcetSendMsg);
		//���� ��Ŷ �ۼ�
		packetSend.m_socketSession = packetRecv.m_socketSession;							//PPPacketForProcess::m_socketSession���� ��Ŷ ������ �Ǵ� �۽��ڸ� ������ �� �ִ�.
		packetSend.m_Mode = PP::PPPacketMode::SEND;											//���� �ƹ� ��ɿ� �������� �ʴ� ������.
		memcpy(packetSend.m_Packet.m_Payload,												//��Ŷ ����� �ۼ�
			(void*)&packetSendMsg, iSizeOfpakcetSendMsg);									//��Ŷ ����ο� memcpy�� ������ ����ü�� deep copy�ؼ� �Է��ϸ� �ȴ�.		
		packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iSizeOfpakcetSendMsg;		//��Ŷ ������� 4����Ʈ + ����� ���̸� ��ģ �� ����
		packetSend.m_Packet.m_Header.m_type = PP::PPPacketType::TYPE_STRING;				//��Ŷ Ÿ��. ������ ���� PP::PPPacketType�� ���ǵǾ��ִ�.

		pSender->Broadcast(packetSend);														//PPSessionManager�� �ִ� ��� ���ǵ��� ��ȸ�Ͽ� send�� �ǽ���.
		break;
	}
	case PP::PPPacketType::TYPE_REQ_SOCKET: {
		PP::PPPacketNoticeSession packetSession = {};
		int iSizeOfPayload = sizeof(packetSession);
		packetSession.m_socketSession = packetRecv.m_socketSession;
		packetSend.m_socketSession = packetRecv.m_socketSession;
		packetSend.m_Mode = PP::PPPacketMode::SEND;
		memcpy(packetSend.m_Packet.m_Payload,												//��Ŷ ����� �ۼ�
			(void*)&packetSession, iSizeOfPayload);									//��Ŷ ����ο� memcpy�� ������ ����ü�� deep copy�ؼ� �Է��ϸ� �ȴ�.		
		packetSend.m_Packet.m_Header.m_len = PACKET_HEADER_SIZE + iSizeOfPayload;		//��Ŷ ������� 4����Ʈ + ����� ���̸� ��ģ �� ����
		packetSend.m_Packet.m_Header.m_type = PP::PPPacketType::TYPE_ACK_SOCKET;

		pSender->Send(packetSend);
		break;
	}

	case PP::PPAdditionalPacketType::TYPE_REQ_MATCHING: {
		PP::PPPacketReqMatching* ppacketPayload = (PP::PPPacketReqMatching*)packetRecv.m_Packet.m_Payload;
		int iMaximumPlayer = ppacketPayload->iMaximumPlayer;

		std::wcout << L"��Ī ��û ����" << std::endl;
		
		//�ִ� �÷��� �ο����� �´� �� Ž��
		auto iterGroup = m_pHead->listChildren.find(iMaximumPlayer);
		if (iterGroup == m_pHead->listChildren.end()) {
			PP::PPGroup* pGroup = new PP::PPGroup;
			pGroup->m_iMaximumPlayer = iMaximumPlayer;
			pGroup->listSession.push_back(packetRecv.m_socketSession);
			m_pHead->listChildren.insert(std::make_pair(iMaximumPlayer, pGroup));
			std::wcout << L"�� �׷� ����" << std::endl;
			break;
		}
		else {
			std::wcout << L"�׷� ã��" << std::endl;
			iterGroup->second->listSession.push_back(packetRecv.m_socketSession);
			int iReturn;
			int iNumOfHost = 1;
			int iNumOfGuest = iMaximumPlayer - 1;
			std::string strHostIPv4;
			if (iterGroup->second->listSession.size() >= iMaximumPlayer) {
				std::wcout << L"��Ī �õ�" << std::endl;
				std::wcout << L"ȣ��Ʈ�� �����մϴ�." << std::endl;
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
					std::wcout << L"ȣ��Ʈ ���� ����" << std::endl;
					break;
				}

				while (iNumOfGuest != 0) {
					std::wcout << L"�Խ�Ʈ�� �����մϴ�." << std::endl;
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
		//���ǵ��� ���� ��Ŷ ó�����Դϴ�.
		wcharBuf = (wchar_t*)&packetRecv;
		std::wcout << wcharBuf;
		packetSend = packetRecv;
		pSender->Broadcast(packetSend);
		break;
	}
	return 0;
}
