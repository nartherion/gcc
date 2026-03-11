#pragma once

#include <nartherion/gcc/common/logging/detail/logging_base.h>

#define NARTHERION_GCC_LOG_DEBUG(...) NARTHERION_GCC_LOG(nartherion::gcc::logging::Severity::kDebug, __VA_ARGS__)
#define NARTHERION_GCC_LOG_INFO(...) NARTHERION_GCC_LOG(nartherion::gcc::logging::Severity::kInfo, __VA_ARGS__)
#define NARTHERION_GCC_LOG_WARNING(...) NARTHERION_GCC_LOG(nartherion::gcc::logging::Severity::kWarning, __VA_ARGS__)
#define NARTHERION_GCC_LOG_ERROR(...) NARTHERION_GCC_LOG(nartherion::gcc::logging::Severity::kError, __VA_ARGS__)
