#pragma once

#include <string_view>

namespace nartherion::gcc::logging {

enum class Severity { kDebug, kInfo, kWarning, kError };

constexpr auto kDefaultSeverity = Severity::kInfo;

namespace detail {

inline auto g_severity = kDefaultSeverity;

}  // namespace detail

inline void SetSeverity(const Severity severity) { detail::g_severity = severity; }

[[nodiscard]] constexpr std::string_view ToString(const Severity severity) noexcept {
    switch (severity) {
        case Severity::kDebug:
            return "DEBUG";

        case Severity::kInfo:
            return "INFO";

        case Severity::kWarning:
            return "WARNING";

        case Severity::kError:
            return "ERROR";
    }
    return "UNKNOWN";
}

}  // namespace nartherion::gcc::logging
