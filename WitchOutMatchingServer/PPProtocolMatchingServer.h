#pragma once
//List of Data Types
//1 Byte	:	bool, char
//2 Bytes	:	short, wchar_t(Windows)
//4 Bytes	:	int, float, long
//8 Bytes	:	long long
namespace PP {
	enum AdditionalPacketType : unsigned short {
		TYPE_REQ_MATCHING = 6,
		TYPE_ACK_MATCHING_HOST,
		TYPE_ACK_MATCHING_GUEST
	};
#pragma pack(push, 1)

#pragma pack(pop)
}