#pragma once

#include <chrono>

namespace nartherion::gcc::arrival_time_filter {

struct InterGroupDelayVariation {
    std::chrono::steady_clock::duration inter_arrival{};
    std::chrono::steady_clock::duration inter_departure{};

    [[nodiscard]] constexpr double Get() const noexcept { return 0.0; }
};

}  // namespace nartherion::gcc::arrival_time_filter
