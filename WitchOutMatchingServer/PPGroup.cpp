#include "PPGroup.h"

PP::PPGroup::PPGroup() {
	m_timeCreate = std::chrono::system_clock::now();
}
PP::PPGroup::~PPGroup() {}

int PP::PPGroup::Run() {
	return 0;
}
