#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace nde {

struct GpuDevice {
    std::string name;
    int memoryMb = 0;
    bool available = false;
};

struct ComputeProfile {
    std::string cpuVendor;
    int cpuThreads = 0;
    int memoryMb = 0;
    int estimatedDiskMbps = 0;
    std::vector<GpuDevice> gpuDevices;

    static ComputeProfile detectSynthetic();
    std::string summary() const;
};

} // namespace nde
