#pragma once
//List of Data Types
//1 Byte	:	bool, char
//2 Bytes	:	short, wchar_t(Windows)
//4 Bytes	:	int, float, long
//8 Bytes	:	long long
namespace PP {
	enum PPAdditionalPacketType : unsigned short {
		TYPE_REQ_MATCHING = 100,
		TYPE_ACK_MATCHING_HOST,
		TYPE_ACK_MATCHING_GUEST,
		TYPE_REJ_MATCHING_HOST,
		TYPE_REJ_MATCHING_GUEST,
		TYPE_REQ_SIGN_IN,
		TYPE_ACK_SIGN_IN
	};
#pragma pack(push, 1)
	struct PPPacketReqMatching {
		short iMaximumPlayer;
	};
	struct PPPacketAckMatchingGuest {
		char charHostAddress[15];
	};
	//사용자명, 비밀번호 전송을 위한 구조체
	struct PPPacketReqSignIn {
		wchar_t m_wcharUsername[16];			//사용자명 16 * 2 바이트
		wchar_t m_wcharPassword[16];			//비밀번호 16 * 2 바이트
	};
	struct PPPacketAckSignIn {
		short m_sSignIn;
	};
#pragma pack(pop)
}