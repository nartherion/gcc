#pragma once

#include <nartherion/gcc/arrival_time_filter/inter_group_delay_variation.h>

#include <chrono>
#include <optional>

namespace nartherion::gcc::arrival_time_filter {

class PacketGroupTracker final {
public:
    static constexpr auto kDefaultBurstDuration = std::chrono::milliseconds{5};

    struct Parameters {
        std::chrono::steady_clock::duration burst_duration = kDefaultBurstDuration;
    };

    explicit PacketGroupTracker(const Parameters& parameters) noexcept;

    [[nodiscard]] std::optional<InterGroupDelayVariation> Push(std::chrono::steady_clock::duration departure,
                                                               std::chrono::steady_clock::duration arrival) noexcept;

private:
    struct Packet {
        std::chrono::steady_clock::duration departure{};
        std::chrono::steady_clock::duration arrival{};
    };

    struct PacketGroup {
        Packet first_packet;
        Packet last_packet;
    };

    [[nodiscard]] static InterGroupDelayVariation GetInterGroupDelayVariation(
        const Packet& previous_group_last, const Packet& current_group_last) noexcept;
    [[nodiscard]] bool IsWithinBurst(std::chrono::steady_clock::duration first,
                                     std::chrono::steady_clock::duration second) const noexcept;
    [[nodiscard]] bool DepartedWithinBurst(const Packet& first_packet, const Packet& last_packet) const noexcept;
    [[nodiscard]] bool ArrivedWithinBurst(const Packet& first_packet, const Packet& last_packet) const noexcept;
    [[nodiscard]] bool BelongsTo(const Packet& packet, const PacketGroup& current) const noexcept;
    [[nodiscard]] std::optional<InterGroupDelayVariation> GetInterGroupDelayVariation(
        const PacketGroup& current) const noexcept;

    const std::chrono::steady_clock::duration burst_duration_;

    std::optional<PacketGroup> previous_;
    std::optional<PacketGroup> current_;
};

}  // namespace nartherion::gcc::arrival_time_filter
