#pragma once

#include <chrono>

namespace nartherion::gcc::arrival_time_filter {

struct InterGroupDelayVariation {
    std::chrono::steady_clock::duration inter_arrival;
    std::chrono::steady_clock::duration inter_departure;

    [[nodiscard]] constexpr std::chrono::steady_clock::duration Get() const noexcept {
        return inter_arrival - inter_departure;
    }
};

}  // namespace nartherion::gcc::arrival_time_filter
