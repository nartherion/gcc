#pragma once

#include <chrono>

namespace nartherion::gcc {

using Duration = std::chrono::steady_clock::duration;
using Milliseconds = std::chrono::milliseconds;

[[nodiscard]] constexpr Milliseconds ToMilliseconds(const Duration& duration) noexcept {
    return std::chrono::duration_cast<Milliseconds>(duration);
}

}  // namespace nartherion::gcc