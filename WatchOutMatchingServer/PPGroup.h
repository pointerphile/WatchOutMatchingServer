#pragma once
#include "PPNode.h"

namespace PP {
	class PPGroup :	public PPNode {
	public:
		int m_iMinimumPlayer;
		int m_iMaximumPlayer;
		std::list<SOCKET> listSession;
		std::chrono::time_point<std::chrono::system_clock> m_timeCreate;
	public:
		PPGroup();
		virtual ~PPGroup();
	public:
		virtual int Run();
	};
}

