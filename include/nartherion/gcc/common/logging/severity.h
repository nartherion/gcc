#pragma once

namespace nartherion::gcc::common::logging {

enum class Severity { kDebug, kInfo, kWarning, kError };

constexpr auto kDefaultSeverity = Severity::kInfo;

namespace detail {

inline auto g_severity = kDefaultSeverity;

}  // namespace detail

inline void SetSeverity(const Severity severity) { detail::g_severity = severity; }

}  // namespace nartherion::gcc::common::logging
