#pragma once
#include "../../libppnetwork/libppnetwork/PPNetworkObject.h"
#include <sql.h>
#include <sqlext.h>

namespace PP {
	class PPSQL {
	public:
		SQLHENV m_hEnv;
		SQLHDBC m_hDBC;
		SQLHSTMT m_hSTMT;
		SQLWCHAR m_wcharInCon[255];
		std::wstring m_wstrInCon;
		SQLWCHAR m_wcharOutCon[1024];
		SQLSMALLINT m_sSizeOutCon;
	public:
		PPSQL();
		~PPSQL();
	public:
		int Init();
		int Run();
		int Release();
	public:
		int DisplayError(const SQLWCHAR * wcharParam, SQLSMALLINT sParam, SQLHANDLE hParam);
		int SignIn(std::wstring wstrUsername, std::wstring wstrPassword);
	};
}