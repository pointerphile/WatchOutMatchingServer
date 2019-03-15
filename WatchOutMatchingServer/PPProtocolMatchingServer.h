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
	//����ڸ�, ��й�ȣ ������ ���� ����ü
	struct PPPPacketAccountInfo {
		wchar_t m_wcharUsername[16];			//����ڸ� 16 * 2 ����Ʈ
		wchar_t m_wcharPassword[16];			//��й�ȣ 16 * 2 ����Ʈ
	};
	//ȸ�������� ���� ����ü
	//�������� �Է��� ��й�ȣ�� ��ȿ���� ���� �˻縦 �ǽ������� �ʴ´�.
	struct PPPacketReqSignUp : PPPPacketAccountInfo {};
	//�α����� ���� ����ü
	struct PPPacketReqSignIn : PPPPacketAccountInfo {};
	//���� �������� �˸��� ���� ����ü
	struct PPPacketAckQuery {
		short m_sReturn;
	};
	//ȸ������ �������� �˸��� ���� ����ü
	struct PPPacketAckSignUp : PPPacketAckQuery {};
	//�α��� �������� �˸��� ���� ����ü
	struct PPPacketAckSignIn : PPPacketAckQuery {};
	//������ ������ ���� ����ü
	struct PPPacketReqUpdateUsername {
		wchar_t m_wcharUsername[16];
		wchar_t m_wcharNewUsername[16];
	};
#pragma pack(pop)
}