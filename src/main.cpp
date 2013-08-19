#include "lib/log/Log.h"

using namespace XCF;

int main() {
    Log *logger = LogFactory::get(LogType::SysLog);

    logger->setPrivilege(LogPrivilege::Info);

    logger->debug("debug message!");
    logger->info("info message!");
    logger->warn("warning message!");
    logger->error("error message!");

    return 0;
}
