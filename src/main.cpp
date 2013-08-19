#include "lib/log/Log.h"

using namespace XCF;

int main() {

    Log *logger = LogFactory::get(LogType::SysLog);

    logger->setPrivilege(LogPrivilege::Error);

    logger->log("log data"); // FIXME: log still works, loop through the C++ book OOP part

    return 0;
}
