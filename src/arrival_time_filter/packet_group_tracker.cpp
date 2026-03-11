#include <nartherion/gcc/arrival_time_filter/packet_group_tracker.h>
#include <nartherion/gcc/common/logging/logging.h>

namespace nartherion::gcc::arrival_time_filter {

PacketGroupTracker::PacketGroupTracker(const Parameters& parameters) noexcept
    : burst_duration_{parameters.burst_duration} {}

std::optional<InterGroupDelayVariation> PacketGroupTracker::Push(const Duration departure,
                                                                 const Duration arrival) noexcept {
    const auto packet = Packet{.departure = departure, .arrival = arrival};
    if (!current_) {
        NARTHERION_GCC_LOG_DEBUG("Created first group (D={}, A={})", packet.departure.count(), packet.arrival.count());
        current_ = PacketGroup{.first_packet = packet, .last_packet = packet};
        return {};
    }
    auto& current = *current_;
    if (const auto& last_packet = current.last_packet;
        departure < last_packet.departure || arrival < last_packet.arrival) {
        NARTHERION_GCC_LOG_WARNING("Packet departure or arrival time is not monotonically increasing");
        return {};
    }
    if (BelongsTo(packet, current)) {
        NARTHERION_GCC_LOG_DEBUG("Added packet to the group (D={}, A={})", packet.departure.count(),
                                 packet.arrival.count());
        current.last_packet = packet;
        return {};
    }
    const auto inter_group_delay_variation = GetInterGroupDelayVariation(*current_);
    previous_ = current_;
    NARTHERION_GCC_LOG_DEBUG("Created new group (D={}, A={})", packet.departure.count(), packet.arrival.count());
    current_ = PacketGroup{.first_packet = packet, .last_packet = packet};
    return inter_group_delay_variation;
}

InterGroupDelayVariation PacketGroupTracker::MakeInterGroupDelayVariation(const Packet& previous_group_last,
                                                                          const Packet& current_group_last) noexcept {
    return InterGroupDelayVariation{.inter_arrival = current_group_last.arrival - previous_group_last.arrival,
                                    .inter_departure = current_group_last.departure - previous_group_last.departure};
}

bool PacketGroupTracker::IsWithinBurst(const Duration first, const Duration second) const noexcept {
    return first + burst_duration_ >= second;
}

bool PacketGroupTracker::DepartedWithinBurst(const Packet& first, const Packet& second) const noexcept {
    return IsWithinBurst(first.departure, second.departure);
}

bool PacketGroupTracker::ArrivedWithinBurst(const Packet& first, const Packet& second) const noexcept {
    return IsWithinBurst(first.arrival, second.arrival);
}

bool PacketGroupTracker::BelongsTo(const Packet& packet, const PacketGroup& current) const noexcept {
    if (DepartedWithinBurst(current.first_packet, packet)) {
        return true;
    }
    if (!ArrivedWithinBurst(current.last_packet, packet) || !previous_) {
        return false;
    }
    const auto& previous = *previous_;
    const auto inter_group_delay_variation = MakeInterGroupDelayVariation(previous.last_packet, packet);
    const auto inter_group_delay_variation_value = inter_group_delay_variation.Get();
    NARTHERION_GCC_LOG_DEBUG("Packet arrived within burst (IGDV={})", inter_group_delay_variation_value.count());
    return inter_group_delay_variation_value < Duration::zero();
}

std::optional<InterGroupDelayVariation> PacketGroupTracker::GetInterGroupDelayVariation(
    const PacketGroup& current) const noexcept {
    if (!previous_) {
        return {};
    }
    const auto& previous = *previous_;
    return MakeInterGroupDelayVariation(previous.last_packet, current.last_packet);
}

}  // namespace nartherion::gcc::arrival_time_filter
