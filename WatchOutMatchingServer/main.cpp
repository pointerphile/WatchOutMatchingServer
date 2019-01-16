#include "WatchOutMatchingServer.h"

int main(int argc, char* argv[]) {
	std::locale::global(std::locale(""));
	PP::WatchOutMatchingServer Server;
	Server.Init();
	Server.Run();
	return 0;
}