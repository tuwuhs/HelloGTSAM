
#pragma once

#include <gtsam/geometry/PinholeCamera.h>
#include <gtsam/geometry/PinholePose.h>
#include <gtsam/geometry/Cal3DS2.h>
#include <boost/make_shared.hpp>
#include <boost/optional/optional_io.hpp>

using namespace gtsam;
using namespace gtsam::noiseModel;

template<class CALIBRATION>
class ResectioningFactor : public NoiseModelFactor1<Pose3>
{
  typedef NoiseModelFactor1<Pose3> Base;

  const boost::shared_ptr<CALIBRATION> K_; ///< camera's intrinsic parameters
  const Vector3 P_;             ///< 3D point on the calibration rig
  const Vector2 p_;             ///< 2D measurement of the 3D point

public:
  /// Construct factor given known point P and its projection p
  ResectioningFactor(const SharedNoiseModel &model, const Key &key,
    const boost::shared_ptr<CALIBRATION> calib, const Vector2 &p, const Vector3 &P)
    : Base(model, key), K_(calib), P_(P), p_(p)
  {
  } 

  /// evaluate the error
  Vector evaluateError(const Pose3 &pose,
    boost::optional<Matrix &> H = boost::none) const override
  {
    // pose is oTe
    PinholePose<CALIBRATION> camera(pose, K_);

    Matrix26 Dproject;
    const auto error = camera.project(P_, H, boost::none, boost::none) - p_;

    // std::cout << error << std::endl << std::flush;
    return error;
  }
};
