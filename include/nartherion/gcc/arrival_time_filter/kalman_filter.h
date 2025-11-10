#pragma once

namespace nartherion::gcc::arrival_time_filter {

class KalmanFilter final {
public:
    constexpr static auto kMinFilterCoefficient = 0.001;
    constexpr static auto kDefaultFilterCoefficient = 0.01;
    constexpr static auto kMaxFilterCoefficient = 0.1;

    struct Parameters {
        double filter_coefficient = kDefaultFilterCoefficient;
    };

    explicit KalmanFilter(const Parameters& parameters);

    void PushMeasurement(const double measurement, const double last_highest_receive_rate) noexcept;
    [[nodiscard]] double GetEstimation() const noexcept { return state_estimation_; }

private:
    [[nodiscard]] double CalculateNextMeasurementNoiseVariance(const double innovation,
                                                               const double highest_group_receive_rate) const noexcept;
    [[nodiscard]] double CalculateProjectedEstimationErrorVariance() const noexcept;

    constexpr static auto kStateNoiseVariance = 1e-3;
    constexpr static auto kMinMeasurementNoiseVariance = 1.0;

    const double filter_coefficient_;

    double estimation_error_variance_ = 0.1;
    double measurement_noise_variance_ = kMinMeasurementNoiseVariance;
    double state_estimation_ = 0.0;
};

}  // namespace nartherion::gcc::arrival_time_filter
