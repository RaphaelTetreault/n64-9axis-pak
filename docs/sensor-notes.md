Command Details

ID: 0x8883. ASCII for "XS", pronounced "axis" / "aXiS"







Absolute Orientation (Euler Vector, 100Hz) Three axis orientation data based on a 360° sphere
Absolute Orientation (Quatenrion, 100Hz) Four point quaternion output for more accurate data manipulation
Angular Velocity Vector (100Hz) Three axis of 'rotation speed' in rad/s
Acceleration Vector (100Hz) Three axis of acceleration (gravity + linear motion) in m/s^2
Magnetic Field Strength Vector (20Hz) Three axis of magnetic field sensing in micro Tesla (uT)
Linear Acceleration Vector (100Hz) Three axis of linear acceleration data (acceleration minus gravity) in m/s^2
Gravity Vector (100Hz) Three axis of gravitational acceleration (minus any movement) in m/s^2
Temperature (1Hz) Ambient temperature in degrees celsius



* 12 bytes euler
* 16 bytes quternion
* 12 bytes ang vel vec
* 12 bytes accel vec
* 12 bytes mag
* 12 bytes lin acc vec
* 12 bytes grav
* 4 bytes