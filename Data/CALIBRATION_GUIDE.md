# Line Follower Robot - Calibration Guide

## Overview
Proper calibration is essential for accurate line following. This guide walks through the process step-by-step.

---

## Step 1: Hardware Setup

### Components Needed
- Arduino UNO
- 2x IR Sensors (typically mounted on the bottom of the robot, ~1-2 cm from the surface)
- L298N Motor Driver
- 2x DC Motors (same specifications for balanced movement)
- Power supply (5V for Arduino, 12V for motors - separate supplies recommended)
- USB cable for Arduino programming

### Wiring Verification
Before starting, verify:
1. ✓ Left IR sensor → Arduino A0
2. ✓ Right IR sensor → Arduino A1
3. ✓ Motor A (left) → L298N pins 9 (IN1), 8 (IN2), 10 (EN)
4. ✓ Motor B (right) → L298N pins 11 (IN3), 12 (IN4), 3 (EN)
5. ✓ Ground connections: Arduino GND connected to motor driver GND
6. ✓ Power: Motors have separate 12V supply

---

## Step 2: Sensor Calibration

### Objective
Find the correct `SENSOR_THRESHOLD` value that reliably detects the black line.

### Procedure

1. **Load calibration sketch:**
   Upload the basic line follower code with `DEBUG_MODE` enabled.

2. **Open Serial Monitor:**
   - Arduino IDE → Tools → Serial Monitor
   - Set baud rate to 9600
   - You'll see sensor readings like: `L:850 R:920 | L_OFF R_OFF`

3. **Get white surface reading:**
   - Place the robot on the white track surface away from the black line
   - Note the sensor values (should be HIGH, e.g., 800-1023)
   - Average white value: ________

4. **Get black line reading:**
   - Place the robot so sensors are directly over the black line
   - Note the sensor values (should be LOW, e.g., 0-200)
   - Average black value: ________

5. **Calculate threshold:**
   ```
   SENSOR_THRESHOLD = (white_value + black_value) / 2
   ```
   
   Example:
   - White: 950
   - Black: 100
   - Threshold: (950 + 100) / 2 = 525
   
   **Your calculated threshold: ________**

6. **Update config.h:**
   ```cpp
   #define SENSOR_THRESHOLD 525  // Use your calculated value
   ```

7. **Verify threshold:**
   - Upload updated code
   - Test both on white and black surfaces
   - Sensors should toggle between ON/OFF correctly

---

## Step 3: Motor Balance Calibration

### Objective
Ensure both motors run at the same speed so the robot moves straight.

### Procedure

1. **Remove sensors temporarily** (or move robot away from track)

2. **Run motors forward:**
   - Upload the line follower code
   - Manually command motors forward: set `BASE_SPEED = 200`
   - Observe motor behavior

3. **Check straight movement:**
   - Draw a straight line on the ground with chalk/tape
   - Set both sensors to detect (simulate line detected)
   - Place robot on line
   - Observe: Does it move straight or drift left/right?

4. **Adjust motor speeds if needed:**
   - If drifts RIGHT: Increase left motor speed (reduce BASE_SPEED slightly or increase left EN pin value in code)
   - If drifts LEFT: Increase right motor speed
   - Note: Motor variance is normal; consider gear differences

5. **Alternative: Use PWM adjustment:**
   - Some L298N modules have potentiometers to adjust speed independently
   - Refer to your motor driver's documentation

---

## Step 4: Speed Optimization

### Objective
Find the optimal `BASE_SPEED` for stable line following.

### Procedure

1. **Start conservative:**
   - Set `BASE_SPEED = 100` initially
   - Test on track

2. **Gradually increase speed:**
   - Increment by 20-30 units each test
   - Observe behavior at each step
   - Look for: smooth following, minimal oscillation

3. **Note optimal speed:**
   - Too slow: Robot is sluggish, may get stuck
   - Too fast: Robot overshoots curves, loses line
   - Optimal: Smooth, responsive following with minimal overshoot
   
   **Optimal BASE_SPEED: ________**

4. **Update config.h:**
   ```cpp
   #define BASE_SPEED 180  // Your optimal value
   ```

---

## Step 5: Turn Response Calibration

### Objective
Fine-tune `TURN_FACTOR` for smooth turns.

### Procedure

1. **Test with different TURN_FACTOR values:**
   - Start with 0.5 (sharp turns)
   - Test: Robot on track, observe curve following

2. **Evaluate behavior:**
   - Too low (< 0.4): Turns too sharp, jerky movement
   - Too high (> 0.8): Turns too gentle, overshoots curves
   - Optimal: Smooth, controlled turns that stay on line

3. **Run on test track:**
   - Create or use track with curves
   - Optimal track: 90-degree corners at 20+ cm radius

4. **Your optimal TURN_FACTOR: ________**

5. **Update config.h:**
   ```cpp
   #define TURN_FACTOR 0.6  // Your optimal value
   ```

---

## Step 6: Final Testing

### Test Scenarios

1. **Straight line:**
   - Robot should maintain center on straight section
   - Minimal left-right oscillation

2. **Left turn:**
   - Robot should smoothly follow curve to the left
   - Should not lose line detection

3. **Right turn:**
   - Robot should smoothly follow curve to the right
   - Should not lose line detection

4. **Sharp corner:**
   - 90-degree angle - robot should navigate without losing line

5. **Line loss recovery:**
   - If line is lost briefly, robot should find it again quickly
   - Check serial output for "WARNING: Line lost!"

---

## Calibration Summary Table

Fill in this table with your calibration values:

| Parameter | Value | Unit | Notes |
|-----------|-------|------|-------|
| Left sensor (white) | _______ | counts | White surface reading |
| Left sensor (black) | _______ | counts | Black line reading |
| Right sensor (white) | _______ | counts | White surface reading |
| Right sensor (black) | _______ | counts | Black line reading |
| SENSOR_THRESHOLD | _______ | counts | (white + black) / 2 |
| BASE_SPEED | _______ | 0-255 | Forward speed |
| TURN_FACTOR | _______ | 0.0-1.0 | Turn responsiveness |
| Optimal performance speed | _______ | cm/s | Measured on track |

---

## Troubleshooting

### Robot won't start or moves erratically
- Check power supply to Arduino and motors
- Verify all wire connections
- Check battery voltage (motors need adequate power)

### Sensors always OFF or always ON
- Check `SENSOR_THRESHOLD` value
- Verify sensor wiring (A0 and A1)
- Ensure sensors are properly mounted and positioned

### Robot drifts left or right on straight sections
- Motor speeds are unbalanced
- Adjust L298N potentiometers or motor speed in code
- Check for mechanical issues (wheel alignment, tire wear)

### Robot oscillates left-right excessively
- BASE_SPEED is too high
- TURN_FACTOR is too low (turns too sharp)
- Increase SENSOR_THRESHOLD for more stable line detection

### Robot loses line on curves
- BASE_SPEED is too high
- TURN_FACTOR is too high (turns too gentle)
- Sensor threshold may be too tight

### Serial monitor shows no output
- Check USB connection
- Verify baud rate (9600)
- Check Arduino board selection in IDE

---

## Performance Tips

1. **Surface quality:** Use matte black line on white matte surface for consistent readings
2. **Lighting:** Avoid direct sunlight on sensors; consistent indoor lighting is best
3. **Sensor height:** Keep sensors 1-2 cm from surface for optimal readings
4. **Track design:** Use 5-10 cm wide lines; avoid very sharp corners
5. **Battery:** Low battery voltage affects motor performance; use fresh batteries for testing

---

## Next Steps

Once calibration is complete:
- Document your values in this guide
- Consider adding speed control via potentiometer
- Explore PID control for even smoother line following
- Test on different track designs and surfaces
