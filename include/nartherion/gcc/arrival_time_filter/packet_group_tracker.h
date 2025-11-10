#pragma once

#include <nartherion/gcc/arrival_time_filter/inter_group_delay_variation.h>

#include <chrono>
#include <optional>

namespace nartherion::gcc::arrival_time_filter {

class PacketGroupTracker final {
public:
    struct Parameters {
        std::chrono::steady_clock::duration burst_duration;
    };

    explicit PacketGroupTracker(const Parameters& parameters) noexcept;

    [[nodiscard]] std::optional<InterGroupDelayVariation> Push(std::chrono::steady_clock::duration departure,
                                                               std::chrono::steady_clock::duration arrival);

private:
    struct Packet {
        std::chrono::steady_clock::duration departure;
        std::chrono::steady_clock::duration arrival;
    };

    struct PacketGroup {
        Packet first_packet;
        Packet last_packet;
    };

    [[nodiscard]] bool IsWithinBurst(const Packet& packet, const PacketGroup& group) const noexcept;
    [[nodiscard]] bool BelongsToGroup(const Packet& packet, const PacketGroup& group) const noexcept;
    [[nodiscard]] std::optional<InterGroupDelayVariation> GetInterGroupDelayVariation(
        const PacketGroup& group) const noexcept;

    const std::chrono::steady_clock::duration burst_duration_;

    std::optional<PacketGroup> previous_;
    std::optional<PacketGroup> current_;
};

}  // namespace nartherion::gcc::arrival_time_filter
