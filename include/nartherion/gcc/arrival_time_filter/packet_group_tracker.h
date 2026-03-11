#pragma once

#include <nartherion/gcc/arrival_time_filter/inter_group_delay_variation.h>
#include <nartherion/gcc/common/duration.h>

#include <optional>

namespace nartherion::gcc::arrival_time_filter {

class PacketGroupTracker final {
public:
    static constexpr auto kDefaultBurstDuration = Milliseconds{5};

    struct Parameters {
        Duration burst_duration = kDefaultBurstDuration;
    };

    explicit PacketGroupTracker(const Parameters& parameters) noexcept;

    [[nodiscard]] std::optional<InterGroupDelayVariation> Push(Duration departure, Duration arrival) noexcept;

private:
    struct Packet {
        Duration departure{};
        Duration arrival{};
    };

    struct PacketGroup {
        Packet first_packet;
        Packet last_packet;
    };

    [[nodiscard]] static InterGroupDelayVariation MakeInterGroupDelayVariation(
        const Packet& previous_group_last, const Packet& current_group_last) noexcept;
    [[nodiscard]] bool IsWithinBurst(Duration first, Duration second) const noexcept;
    [[nodiscard]] bool DepartedWithinBurst(const Packet& first_packet, const Packet& last_packet) const noexcept;
    [[nodiscard]] bool ArrivedWithinBurst(const Packet& first_packet, const Packet& last_packet) const noexcept;
    [[nodiscard]] bool BelongsTo(const Packet& packet, const PacketGroup& current) const noexcept;
    [[nodiscard]] std::optional<InterGroupDelayVariation> GetInterGroupDelayVariation(
        const PacketGroup& current) const noexcept;

    const Duration burst_duration_;

    std::optional<PacketGroup> previous_;
    std::optional<PacketGroup> current_;
};

}  // namespace nartherion::gcc::arrival_time_filter
