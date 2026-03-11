#pragma once

#include <nartherion/gcc/common/logging/callback.h>
#include <nartherion/gcc/common/logging/severity.h>

#include <format>
#include <source_location>

#define NARTHERION_GCC_LOG(log_severity, ...)                                              \
    do {                                                                                   \
        if (nartherion::gcc::logging::detail::g_severity >= log_severity) {                \
            const auto location = std::source_location::current();                         \
            const auto message = std::format(__VA_ARGS__);                                 \
            nartherion::gcc::logging::detail::g_callback(log_severity, location, message); \
        }                                                                                  \
    } while (false)
