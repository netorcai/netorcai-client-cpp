#include <netorcai-client-cpp/version.hpp>

using std::to_string;

int netorcai::majorVersion = 2;
int netorcai::minorVersion = 0;
int netorcai::patchVersion = 0;

std::string netorcai::metaprotocolVersion()
{
    return to_string(majorVersion) + "." + to_string(minorVersion) + "." + to_string(patchVersion);
}
