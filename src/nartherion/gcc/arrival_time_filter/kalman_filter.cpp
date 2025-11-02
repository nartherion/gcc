#include <nartherion/gcc/arrival_time_filter/kalman_filter.h>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace nartherion::gcc {

namespace {

[[nodiscard]] double CalculateKalmanGain(const double projected_estimation_error_variance,
                                         const double measurement_noise_variance) noexcept {
    return projected_estimation_error_variance / (measurement_noise_variance + projected_estimation_error_variance);
}

[[nodiscard]] double CalculateNextEstimationErrorVariance(const double kalman_gain,
                                                          const double projected_estimation_error_variance) noexcept {
    return (1 - kalman_gain) * projected_estimation_error_variance;
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
}

double KalmanFilter::CalculateNextMeasurementNoiseVariance(const double innovation,
                                                           const double highest_group_receive_rate) const noexcept {
    const auto smoothed_innovation = std::min(innovation, 3 * sqrt(measurement_noise_variance_));
    const auto heuristic_exponent = 30 / (1000 * highest_group_receive_rate);
    const auto weight = std::pow(1 - filter_coefficient_, heuristic_exponent);
    return std::max(kMinMeasurementNoiseVariance,
                    weight * measurement_noise_variance_ + (1 - weight) * pow(smoothed_innovation, 2));
}

double KalmanFilter::CalculateProjectedEstimationErrorVariance() const noexcept {
    return estimation_error_variance_ + kStateNoiseVariance;
}

}  // namespace nartherion::gcc
