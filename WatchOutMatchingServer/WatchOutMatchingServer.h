#pragma once

#include "../../libppnetwork/libppnetwork/PPTCPIOCPServer.h"
#include "../../libppnetwork/libppnetwork/PPRecvPacketPoolServer.h"
#include "../../libppnetwork/libppnetwork/PPSender.h"
#include "PPProtocolMatchingServer.h"
#include "PPSequence.h"
#include "PPGroup.h"
#include "PPSQL.h"

#ifdef _DEBUG
#pragma comment(lib, "../../libppnetwork/x64/Debug/libppnetwork_d.lib")					//���� ���̺귯���� lib �ε�. ����� libppnetwork.dll�� �ݵ�� �ʿ��մϴ�.
#else
#pragma comment(lib, "../../libppnetwork/x64/Release/libppnetwork.lib")					//���� ���̺귯���� lib �ε�. ����� libppnetwork.dll�� �ݵ�� �ʿ��մϴ�.
#endif // DEBUG

namespace PP {
	class WatchOutMatchingServer {
	private:
		PPSQL m_SQL;
	public:
		PPSequence* m_pHead;
	public:
		PPTCPIOCPServer* m_pServer;
		PPSender* m_pSender;
	public:
		WatchOutMatchingServer();
		~WatchOutMatchingServer();
	public:
		int Init();
		int Run();
		int Release();
	public:
		int ProcessPacket();
	};
}
