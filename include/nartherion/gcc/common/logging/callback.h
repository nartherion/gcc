#pragma once

#include <nartherion/gcc/common/logging/severity.h>

#include <functional>
#include <source_location>
#include <string>

namespace nartherion::gcc::logging {

inline void DefaultCallback(const Severity, const std::source_location, const std::string&) {}

using Callback = std::function<void(Severity, std::source_location, const std::string&)>;

namespace detail {

inline auto g_callback = Callback{DefaultCallback};

}  // namespace detail

inline void SetCallback(Callback callback) { detail::g_callback = std::move(callback); }

}  // namespace nartherion::gcc::logging