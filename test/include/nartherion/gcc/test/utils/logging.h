#include <nartherion/gcc/common/logging/callback.h>
#include <nartherion/gcc/common/logging/severity.h>
#include <nartherion/gcc/common/utils/delete_special_members.h>

#include <print>

namespace nartherion::gcc::test::utils {

class Logging final {
public:
    Logging() { ConfigureLogging(); }
    ~Logging() { ResetLogging(); }

    NARTHERION_GCC_DELETE_COPY_AND_MOVE(Logging)

private:
    static void ConfigureLogging() {
        using namespace common::logging;
        SetCallback(LoggingCallback);
        SetSeverity(Severity::kDebug);
    }

    static void ResetLogging() {
        using namespace common::logging;
        SetCallback(DefaultCallback);
        SetSeverity(kDefaultSeverity);
    }

    static void LoggingCallback(const common::logging::Severity severity, const std::source_location location,
                                const std::string& message) {
        constexpr auto kColons = std::string_view{"::"};
        constexpr auto kPathDelimiter = std::string_view{"/"};
        constexpr auto kOpenParenthesis = '(';
        auto function_name = std::string_view{location.function_name()};
        if (const auto i = function_name.rfind(kColons); i != std::string_view::npos) {
            function_name = function_name.substr(i + std::size(kColons));
        }
        if (const auto i = function_name.rfind(kOpenParenthesis); i != std::string_view::npos) {
            function_name = function_name.substr(0, i);
        }
        auto file_name = std::string_view{location.file_name()};
        if (const auto i = file_name.rfind(kPathDelimiter); i != std::string_view::npos) {
            file_name = file_name.substr(i + std::size(kPathDelimiter));
        }
        std::println("[{}] [{}:{}] [{}] [{}]", common::logging::ToString(severity), file_name, location.line(),
                     function_name, message);
    }
};

inline const auto g_logging = Logging{};

}  // namespace nartherion::gcc::test::utils
