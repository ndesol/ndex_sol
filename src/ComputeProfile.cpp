#include "nde/ComputeProfile.hpp"

#include <sstream>
#include <thread>

namespace nde {

ComputeProfile ComputeProfile::detectSynthetic() {
    ComputeProfile profile;
    profile.cpuThreads = static_cast<int>(std::max(1u, std::thread::hardware_concurrency()));
    profile.memoryMb = 16384;
    profile.estimatedDiskMbps = 180;
    profile.gpuDevices.push_back({"Demo GPU Adapter 0", 8192, true});
    profile.gpuDevices.push_back({"Demo Integrated Adapter", 1024, false});
    return profile;
}

std::string ComputeProfile::summary() const {
    std::ostringstream output;
    output << "CPU threads=" << cpuThreads
           << ", memoryMb=" << memoryMb
           << ", diskMbps=" << estimatedDiskMbps
           << ", gpuCount=" << gpuDevices.size();
    return output.str();
}

} // namespace nde
