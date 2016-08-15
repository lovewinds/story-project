#include <string>

#include "scripting/SpLogHelper.hpp"
#include "util/LogHelper.hpp"
#include "util/IPCHelper.hpp"

SpLog::SpLog()
{
	unsigned long err_code;

	Log::init();
}

SpLog::~SpLog()
{

}

void SpLog::dbg()
{
	LOGF(PY_LOG, "Python debug Message !!!");
}

void SpLog::connect()
{
	long unsigned int t = 0;
	std::string test("test");
    IPCClient::OpenIPC();
}

void SpLog::send()
{
    IPCClient::SendIPC(NULL, NULL, 0);
}
