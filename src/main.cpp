#include "lib/log/Log.h"

using namespace XCF;

int main() {
    Log *logger = LogFactory::get(LogType::SysLog);

    logger->setPrivilege(LogPrivilege::Info);

    logger->log("debug message!");
    logger->log(LogPrivilege::Info, "info message!");
    logger->log(LogPrivilege::Warning, "warning message!");
    logger->log(LogPrivilege::Error, "error message!");

    return 0;
}
