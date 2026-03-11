#include <gtest/gtest.h>
#include <nartherion/gcc/arrival_time_filter/packet_group_tracker.h>
#include <nartherion/gcc/test/utils/logging.h>

namespace nartherion::gcc::test {

template <typename PreviousGroupArray, typename CurrentGroupArray>
arrival_time_filter::InterGroupDelayVariation MakeInterGroupDelayVariation(const PreviousGroupArray& previous_group,
                                                                           const CurrentGroupArray& current_group) {
    const auto [previous_departure, previous_arrival] = previous_group.back();
    const auto [current_departure, current_arrival] = current_group.back();
    return arrival_time_filter::InterGroupDelayVariation{.inter_arrival = current_arrival - previous_arrival,
                                                         .inter_departure = current_departure - previous_departure};
}

// TODO: Create an algorithm to generate the packet groups based on inter arrival and inter departure requirements.
class PacketGroupTrackerFixture : public testing::Test {
public:
    [[nodiscard]] static arrival_time_filter::PacketGroupTracker CreatePacketGroupTracker() {
        const auto parameters = arrival_time_filter::PacketGroupTracker::Parameters{};
        return arrival_time_filter::PacketGroupTracker{parameters};
    }

    template <typename Array>
    void ProcessAndAssertEmptyReturnValue(const Array& group,
                                          arrival_time_filter::PacketGroupTracker& packet_group_tracker) {
        for (const auto [departure, arrival] : group) {
            const auto result = packet_group_tracker.Push(departure, arrival);
            ASSERT_FALSE(result);
        }
    }
};

TEST_F(PacketGroupTrackerFixture, FirstPacketInitializesTheGroup) {
    using namespace std::chrono_literals;
    auto packet_group_tracker = CreatePacketGroupTracker();
    const auto [departure, arrival] = std::pair{0ms, 2ms};
    const auto inter_group_delay_variation = packet_group_tracker.Push(departure, arrival);
    ASSERT_FALSE(inter_group_delay_variation);
}

TEST_F(PacketGroupTrackerFixture, PacketsDepartedWithinSingleBurstComposeGroup) {
    using namespace std::chrono_literals;
    constexpr auto first_group = std::array{std::pair{0ms, 2ms}, std::pair{2ms, 4ms}, std::pair{4ms, 6ms}};
    constexpr auto second_group = std::array{std::pair{6ms, 12ms}, std::pair{8ms, 14ms}, std::pair{10ms, 16ms}};
    auto packet_group_tracker = CreatePacketGroupTracker();
    ProcessAndAssertEmptyReturnValue(first_group, packet_group_tracker);
    ProcessAndAssertEmptyReturnValue(second_group, packet_group_tracker);
    const auto [departure, arrival] = std::pair{12ms, 22ms};
    const auto result = packet_group_tracker.Push(departure, arrival);
    ASSERT_TRUE(result);
}

TEST_F(PacketGroupTrackerFixture, PacketsArrivedWithinSingleBurstWithNegativeIGDVComposeGroup) {
    using namespace std::chrono_literals;
    constexpr auto first_group = std::array{std::pair{0ms, 6ms}, std::pair{2ms, 7ms}, std::pair{4ms, 8ms}};
    constexpr auto second_group = std::array{std::pair{6ms, 9ms}, std::pair{8ms, 10ms}, std::pair{12ms, 14ms},
                                             std::pair{18ms, 19ms}, std::pair{19ms, 20ms}};
    auto packet_group_tracker = CreatePacketGroupTracker();
    ProcessAndAssertEmptyReturnValue(first_group, packet_group_tracker);
    ProcessAndAssertEmptyReturnValue(second_group, packet_group_tracker);
    const auto [departure, arrival] = std::pair{20ms, 26ms};
    const auto result = packet_group_tracker.Push(departure, arrival);
    ASSERT_TRUE(result);
}

TEST_F(PacketGroupTrackerFixture, PacketsArrivedWithinSingleBurstWithoutPreviousGroupAvailable) {
    using namespace std::chrono_literals;
    constexpr auto first_group = std::array{std::pair{6ms, 9ms}, std::pair{8ms, 10ms}};
    constexpr auto second_group = std::array{std::pair{12ms, 14ms}, std::pair{14ms, 16ms}, std::pair{16ms, 20ms}};
    auto packet_group_tracker = CreatePacketGroupTracker();
    ProcessAndAssertEmptyReturnValue(first_group, packet_group_tracker);
    ProcessAndAssertEmptyReturnValue(second_group, packet_group_tracker);
    const auto [departure, arrival] = std::pair{18ms, 26ms};
    const auto result = packet_group_tracker.Push(departure, arrival);
    ASSERT_TRUE(result);
    const auto& inter_group_delay_variation = *result;
    const auto expected_inter_group_delay_variation = MakeInterGroupDelayVariation(first_group, second_group);
    ASSERT_EQ(inter_group_delay_variation.inter_arrival, expected_inter_group_delay_variation.inter_arrival);
    ASSERT_EQ(inter_group_delay_variation.inter_departure, expected_inter_group_delay_variation.inter_departure);
    ASSERT_EQ(inter_group_delay_variation.Get(), expected_inter_group_delay_variation.Get());
}

TEST_F(PacketGroupTrackerFixture, ValidInterGroupDelayVariation) {
    using namespace std::chrono_literals;
    constexpr auto first_group = std::array{std::pair{0ms, 2ms}, std::pair{2ms, 4ms}, std::pair{4ms, 6ms}};
    constexpr auto second_group = std::array{std::pair{6ms, 12ms}, std::pair{8ms, 14ms}, std::pair{10ms, 16ms}};
    auto packet_group_tracker = CreatePacketGroupTracker();
    ProcessAndAssertEmptyReturnValue(first_group, packet_group_tracker);
    ProcessAndAssertEmptyReturnValue(second_group, packet_group_tracker);
    const auto [departure, arrival] = std::pair{12ms, 22ms};
    const auto result = packet_group_tracker.Push(departure, arrival);
    ASSERT_TRUE(result);
    const auto& inter_group_delay_variation = *result;
    const auto expected_inter_group_delay_variation = MakeInterGroupDelayVariation(first_group, second_group);
    ASSERT_EQ(inter_group_delay_variation.inter_arrival, expected_inter_group_delay_variation.inter_arrival);
    ASSERT_EQ(inter_group_delay_variation.inter_departure, expected_inter_group_delay_variation.inter_departure);
    ASSERT_EQ(inter_group_delay_variation.Get(), expected_inter_group_delay_variation.Get());
}

}  // namespace nartherion::gcc::test
