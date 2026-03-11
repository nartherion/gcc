#pragma once

#include <nartherion/gcc/common/duration.h>

namespace nartherion::gcc::arrival_time_filter {

struct InterGroupDelayVariation {
    Duration inter_arrival;
    Duration inter_departure;

    [[nodiscard]] constexpr Duration Get() const noexcept { return inter_arrival - inter_departure; }
};

}  // namespace nartherion::gcc::arrival_time_filter
