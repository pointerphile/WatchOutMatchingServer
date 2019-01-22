#include "PPSQL.h"

PP::PPSQL::PPSQL() {}
PP::PPSQL::~PPSQL() {}

int PP::PPSQL::Init() {
	int iReturn = 0;

	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv) != SQL_SUCCESS)
		return -1;
	if (SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
		return -1;
	if (SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDBC) != SQL_SUCCESS)
		return -1;

	m_wstrInCon = L"Driver={SQL Server}; SERVER=127.0.0.1, 1433; DATABASE=testDB; Network=dbmssocn; UID=sa; PWD=kgca!@3;";
	iReturn = SQLDriverConnectW(m_hDBC, NULL, (SQLWCHAR*)m_wstrInCon.c_str(), (SQLSMALLINT)m_wstrInCon.length(), m_wcharOutCon,
		_countof(m_wcharOutCon), &m_sSizeOutCon, SQL_DRIVER_NOPROMPT);
	if ((iReturn != SQL_SUCCESS) && (iReturn != SQL_SUCCESS_WITH_INFO)) {
		DisplayError(L"SQLDriverConnect():", SQL_HANDLE_DBC, m_hDBC);
		return -1;
	}

	if (SQLAllocHandle(SQL_HANDLE_STMT, m_hDBC, &m_hSTMT) != SQL_SUCCESS)
		return -1;

	return 0;
}

int PP::PPSQL::Run() {
	return 0;
}

int PP::PPSQL::Release() {
	if (m_hSTMT) SQLCloseCursor(m_hSTMT);
	if (m_hSTMT) SQLFreeHandle(SQL_HANDLE_STMT, m_hSTMT);
	if (m_hDBC) SQLDisconnect(m_hDBC);
	if (m_hSTMT) SQLFreeHandle(SQL_HANDLE_DBC, m_hDBC);
	if (m_hEnv) SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);

	return 0;
}

int PP::PPSQL::DisplayError(const SQLWCHAR * wcharParam, SQLSMALLINT sParam, SQLHANDLE hParam) {
	SQLWCHAR wcharState[1024] = {};
	SQLWCHAR wcharMessage[1024] = {};

	if (SQL_SUCCESS == SQLGetDiagRecW(sParam, hParam, 1, wcharState, NULL, wcharMessage, 1024, NULL)) {
		std::wcout << wcharParam << L": Message: " << wcharMessage << std::endl;
		std::wcout << L"SQLSTATE: " << wcharState << std::endl;
	}
	return 0;
}

int PP::PPSQL::SignIn(const SQLWCHAR * wcharUsername, const SQLWCHAR * wcharPassword) {
	return 0;
}
