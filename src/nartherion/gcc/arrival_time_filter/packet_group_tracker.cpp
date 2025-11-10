#include <nartherion/gcc/arrival_time_filter/packet_group_tracker.h>

#include <chrono>
#include <stdexcept>

namespace nartherion::gcc::arrival_time_filter {

PacketGroupTracker::PacketGroupTracker(const Parameters& parameters) noexcept
    : burst_duration_{parameters.burst_duration} {}

std::optional<InterGroupDelayVariation> PacketGroupTracker::Push(const std::chrono::steady_clock::duration departure,
                                                                 const std::chrono::steady_clock::duration arrival) {
    const auto packet = Packet{.departure = departure, .arrival = arrival};
    if (!current_) {
        current_ = PacketGroup{.first_packet = packet, .last_packet = packet};
        return {};
    }
    if (const auto& last_packet = current_->last_packet;
        departure < last_packet.departure || arrival < last_packet.arrival) {
        throw std::logic_error{"Packet departure and arrival times should be monotonically increasing."};
    }
    if (IsWithinBurst(packet, *current_) || BelongsToGroup(packet, *current_)) {
        current_->last_packet = packet;
        return {};
    }
    const auto inter_group_delay_variation = GetInterGroupDelayVariation(*current_);
    previous_ = current_;
    current_ = PacketGroup{.first_packet = packet, .last_packet = packet};
    return inter_group_delay_variation;
}

bool PacketGroupTracker::IsWithinBurst(const Packet& packet, const PacketGroup& group) const noexcept {
    return group.first_packet.departure + burst_duration_ <= packet.departure;
}

bool PacketGroupTracker::BelongsToGroup(const Packet& packet, const PacketGroup& group) const noexcept {
    const auto& last_packet = group.last_packet;
    const auto inter_departure = packet.departure - last_packet.departure;
    const auto inter_arrival = packet.arrival - last_packet.arrival;
    const auto arrived_within_single_burst = inter_arrival <= burst_duration_;
    const auto is_negative_delay_variation =
        inter_arrival - inter_departure < std::chrono::steady_clock::duration::zero();
    return arrived_within_single_burst && is_negative_delay_variation;
}

std::optional<InterGroupDelayVariation> PacketGroupTracker::GetInterGroupDelayVariation(
    const PacketGroup& current) const noexcept {
    if (!previous_) {
        return {};
    }
    const auto& previous = *previous_;
    return InterGroupDelayVariation{.inter_arrival = current.last_packet.arrival - previous.last_packet.arrival,
                                    .inter_departure = current.last_packet.departure - previous.last_packet.departure};
}

}  // namespace nartherion::gcc::arrival_time_filter
