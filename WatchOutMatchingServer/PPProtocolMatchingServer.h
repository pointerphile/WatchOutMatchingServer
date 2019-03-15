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
		TYPE_REQ_SIGN_UP,
		TYPE_ACK_SIGN_UP,
		TYPE_REQ_SIGN_IN,
		TYPE_ACK_SIGN_IN,
		TYPE_REQ_UPDATE_USERNAME,
		TYPE_ACK_UPDATE_USERNAME,
	};
#pragma pack(push, 1)
	struct PPPacketReqMatching {
		short iMaximumPlayer;
	};
	struct PPPacketAckMatchingGuest {
		char charHostAddress[15];
	};
	//사용자명, 비밀번호 전송을 위한 구조체
	struct PPPPacketAccountInfo {
		wchar_t m_wcharUsername[16];			//사용자명 16 * 2 바이트
		wchar_t m_wcharPassword[16];			//비밀번호 16 * 2 바이트
	};
	//회원가입을 위한 구조체
	//서버에서 입력한 비밀번호의 유효성에 대한 검사를 실시하지는 않는다.
	struct PPPacketReqSignUp : PPPPacketAccountInfo {};
	//로그인을 위한 구조체
	struct PPPacketReqSignIn : PPPPacketAccountInfo {};
	//쿼리 성공여부 알림을 위한 구조체
	struct PPPacketAckQuery {
		short m_sReturn;
	};
	//회원가입 성공여부 알림을 위한 구조체
	struct PPPacketAckSignUp : PPPacketAckQuery {};
	//로그인 성공여부 알림을 위한 구조체
	struct PPPacketAckSignIn : PPPacketAckQuery {};
	//계정명 변경을 위한 구조체
	struct PPPacketReqUpdateUsername {
		wchar_t m_wcharUsername[16];
		wchar_t m_wcharNewUsername[16];
	};
#pragma pack(pop)
}