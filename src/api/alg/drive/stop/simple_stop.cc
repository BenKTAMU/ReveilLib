#include "rev/api/alg/drive/stop/simple_stop.hh"

namespace rev {

rev::QAngle near_semicircle(rev::QAngle angle, rev::QAngle reference);

SimpleStop::SimpleStop(QTime iharsh_threshold,
                       QTime icoast_threshold,
                       double icoast_power)
    : harsh_threshold(iharsh_threshold),
      coast_threshold(icoast_threshold),
      coast_power(fabs(icoast_power)) {}
stop_state SimpleStop::get_stop_state(OdometryState current_state,
                                      Position target_state,
                                      Position start_state,
                                      QLength drop_early) {
  // Now actually calculate the other stuff
  // For now we will just assume latitudinal distance is negligible
  QSpeed longitudinal_speed =
      sqrt(current_state.vel.xv * current_state.vel.xv + current_state.vel.yv * current_state.vel.yv);

  Pose pos_current = current_state.pos;

  // Find the pose which is at target_state
  Pose pos_final = target_state;
  // but make this reference frame face directly away from the start state
  pos_final.theta = atan2(pos_final.y - start_state.y, pos_final.x - start_state.x);

  // Reframe the robots current position in reference to the target state
  // Because of the previous step, this should result in the starting longitudinal_distance being always (-d, 0)
  // Successive errors should also be of the form (x, y), where x < 0 and y is small
  Pose error = pos_current.to_relative(pos_final);

  QLength longitudinal_distance = -error.x - drop_early;

  // Begin stopping the robot if we've passed the target
  if (longitudinal_distance.get_value() < 0) {
    stop_state_last = stop_state::BRAKE;
    return harsh_threshold.convert(second) > 0.001 ? stop_state::BRAKE
                                                  : stop_state::EXIT;
  }

  // Handle harsh stop
  if (longitudinal_speed * harsh_threshold > longitudinal_distance ||
      stop_state_last == stop_state::BRAKE) {
    stop_state_last = stop_state::BRAKE;
    return stop_state::BRAKE;
  }

  // Handle coast transition
  if (longitudinal_speed * coast_threshold > longitudinal_distance ||
      stop_state_last == stop_state::COAST) {
    stop_state_last = stop_state::COAST;
    return stop_state::COAST;
  }

  return stop_state::GO;
}

double SimpleStop::get_coast_power() {
  return coast_power;
}

}  // namespace rev