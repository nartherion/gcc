#include <nartherion/gcc/arrival_time_filter/kalman_filter.h>
#include <nartherion/gcc/common/logging/logging.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace nartherion::gcc::arrival_time_filter {

namespace {

[[nodiscard]] constexpr double CalculateKalmanGain(const double projected_estimation_error_variance,
                                                   const double measurement_noise_variance) noexcept {
    return projected_estimation_error_variance / (measurement_noise_variance + projected_estimation_error_variance);
}

[[nodiscard]] constexpr double CalculateNextEstimationErrorVariance(
    const double kalman_gain, const double projected_estimation_error_variance) noexcept {
    return (1 - kalman_gain) * projected_estimation_error_variance;
}

[[nodiscard]] constexpr double CalculateSmoothedInnovation(const double innovation,
                                                           const double measurement_noise_variance) noexcept {
    const auto sign = innovation < 0 ? -1 : 1;
    const auto smoothed_innovation = std::min(std::abs(innovation), 3 * std::sqrt(measurement_noise_variance));
    return sign * smoothed_innovation;
}

}  // namespace

KalmanFilter::KalmanFilter(const Parameters& parameters) : filter_coefficient_{parameters.filter_coefficient} {
    if (std::clamp(filter_coefficient_, kMinFilterCoefficient, kMaxFilterCoefficient) != filter_coefficient_) {
        throw std::logic_error{"Wrong filter coefficient."};
    }
}

void KalmanFilter::PushMeasurement(const double measurement, const double highest_group_receive_rate) noexcept {
    const auto innovation = measurement - state_estimation_;
    measurement_noise_variance_ = CalculateNextMeasurementNoiseVariance(innovation, highest_group_receive_rate);
    const auto projected_estimation_error_variance = CalculateProjectedEstimationErrorVariance();
    const auto kalman_gain = CalculateKalmanGain(projected_estimation_error_variance, measurement_noise_variance_);
    estimation_error_variance_ = CalculateNextEstimationErrorVariance(kalman_gain, projected_estimation_error_variance);
    state_estimation_ += innovation * kalman_gain;
    NARTHERION_GCC_LOG_DEBUG("M={:.3f}, I={:.3f}, MNV={:.3f}, PEEV={:.3f}, KG={:.3f}, EEV={:.3f}, SE={:.3f}",
                             measurement, innovation, measurement_noise_variance_, projected_estimation_error_variance,
                             kalman_gain, estimation_error_variance_, state_estimation_);
}

double KalmanFilter::CalculateNextMeasurementNoiseVariance(const double innovation,
                                                           const double highest_group_receive_rate) const noexcept {
    const auto smoothed_innovation = CalculateSmoothedInnovation(innovation, measurement_noise_variance_);
    const auto heuristic_exponent = 30 / (1000 * highest_group_receive_rate);
    const auto weight = std::pow(1 - filter_coefficient_, heuristic_exponent);
    const auto next_measurement_noise_variance =
        weight * measurement_noise_variance_ + (1 - weight) * pow(smoothed_innovation, 2);
    NARTHERION_GCC_LOG_DEBUG("SI={:.3f}, HGRR={:.3f}, HE={:.3f}, W={:.3f}, NMNV={:.3f}", smoothed_innovation,
                             highest_group_receive_rate, heuristic_exponent, weight, next_measurement_noise_variance);
    return std::max(kMinMeasurementNoiseVariance, next_measurement_noise_variance);
}

double KalmanFilter::CalculateProjectedEstimationErrorVariance() const noexcept {
    return estimation_error_variance_ + kStateNoiseVariance;
}

}  // namespace nartherion::gcc::arrival_time_filter
