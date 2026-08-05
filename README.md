# Line Follower Robot - Arduino UNO Project

>[!NOTE]
> There are 3 variants of the code you can try out. However, if you download `LineFollowerRobot.ino`, make sure to download `config.h` header file as they are linked.
> Also ensure that they're in the same sketch folder.

## 🤖 Overview

A line-following robot built with Arduino UNO that autonomously follows a black line on a white surface using two IR sensors and an L298N motor driver.

### Key Features
- **Autonomous line following** using two IR sensors
- **Adjustable speed and turn sensitivity** via configuration
- **Debug mode** for sensor monitoring and troubleshooting
- **Simple, well-documented code** for learning and modification
- **Complete calibration guide** for setup optimization

---

## 📦 Hardware Requirements

| Component | Quantity | Notes |
|-----------|----------|-------|
| Arduino UNO | 1 | Microcontroller |
| IR Sensor Module | 2 | Analog IR proximity sensors |
| L298N Motor Driver | 1 | Dual motor control module |
| DC Motor (3-6V) | 2 | Should have similar specs for balanced movement |
| Wheel | 2 | Attached to motors |
| Caster Wheel | 1 | For balance (optional but recommended) |
| Chassis | 1 | Robot body/frame |
| Power Supply (5V) | 1 | For Arduino and sensors |
| Power Supply (12V) | 1 | For motors (separate recommended) |
| USB Cable | 1 | For programming/debugging |
| Jumper Wires | ~20 | For connections |

### Track Requirements
- Black line (matte finish, ~5-10 cm wide)
- White surface (matte finish)
- Curves with radius ≥ 20 cm

---

## 🔌 Wiring Diagram

### Sensor Connections
```
IR Sensor (Left)  → Arduino A0
IR Sensor (Right) → Arduino A1
```

### Motor Driver (L298N) Connections
```
Motor A (Left Motor)
├── IN1 → Arduino Pin 9
├── IN2 → Arduino Pin 8
├── ENs → Arduino Pin 10 (PWM)
└── Motor output → Left motor

Motor B (Right Motor)
├── IN3 → Arduino Pin 11
├── IN4 → Arduino Pin 12
├── ENs → Arduino Pin 3 (PWM)
└── Motor output → Right motor

Power:
├── +12V → Motor driver +12V
├── GND → Motor driver GND
├── Arduino GND → Motor driver GND (common ground)
└── Motors → Motor driver output terminals
```

### Pin Summary
| Pin | Function | Type |
|-----|----------|------|
| A0 | Left Sensor | Input |
| A1 | Right Sensor | Input |
| 3 | Motor B Enable | PWM Output |
| 8 | Motor A Direction 2 | Output |
| 9 | Motor A Direction 1 | Output |
| 10 | Motor A Enable | PWM Output |
| 11 | Motor B Direction 3 | Output |
| 12 | Motor B Direction 4 | Output |

---

## 🚀 Quick Start

### 1. Upload Code
1. Connect Arduino UNO via USB
2. Open the `ino` file in Arduino IDE
3. Select correct board and port
4. Upload code

### 2. Calibrate Sensors
1. Open Serial Monitor (Tools → Serial Monitor, 9600 baud)
2. Follow steps in `CALIBRATION_GUIDE.md`
3. Note sensor readings on white and black surfaces
4. Update `SENSOR_THRESHOLD` in `config.h`

### 3. Test Motors
1. Verify motor connections
2. Run code with robot on flat surface
3. Check for straight movement
4. Adjust motor speeds if needed

### 4. Tune Performance
1. Set `BASE_SPEED` to optimal value
2. Adjust `TURN_FACTOR` for smooth turns
3. Test on track with curves
4. Fine-tune as needed

### 5. Deploy
1. Create your track
2. Place robot on line
3. Power on and watch it follow!

---

## 🧮 How It Works

### Sensor Reading
The robot reads two analog sensors:
- **Left Sensor (A0):** Detects if left wheel is on line
- **Right Sensor (A1):** Detects if right wheel is on line

Readings are compared against `SENSOR_THRESHOLD`:
- Below threshold → Line detected (ON)
- Above threshold → No line (OFF)

### Line Following Logic
The robot uses a simple but effective algorithm:

| Left Sensor | Right Sensor | Action |
|-------------|--------------|--------|
| ON | ON | Move forward (line centered) |
| ON | OFF | Turn left (drifted right) |
| OFF | ON | Turn right (drifted left) |
| OFF | OFF | Stop (line lost) |

### Motor Control
Motors are controlled via PWM (Pulse Width Modulation):
- Speed: 0-255 (0 = stopped, 255 = full speed)
- Direction: Controlled by IN1/IN2 and IN3/IN4 pins
- Turning: Achieved by reducing inner motor speed (differential drive)

---

## ⚙️ Configuration Parameters

> [!IMPORTANT]
> Use `config.h` to config the parameters as mentioned below.

### SENSOR_THRESHOLD
- **Default:** 500
- **Range:** 0-1023
- **Purpose:** Determines whether sensor detects the line
- **Adjustment:** 
  - Increase if too many false positives on white
  - Decrease if line not detected on black
  - See calibration guide for precise calculation

### BASE_SPEED
- **Default:** 180
- **Range:** 0-255
- **Purpose:** Forward movement speed
- **Adjustment:**
  - Increase for faster movement (may oversshoot lines)
  - Decrease for slower, more stable following

### TURN_FACTOR
- **Default:** 0.6
- **Range:** 0.0-1.0
- **Purpose:** Controls turn sharpness
- **Adjustment:**
  - Lower (0.4) for sharp, responsive turns
  - Higher (0.8) for gentle, smooth turns

### DEBUG_MODE
- **Default:** 1 (enabled)
- **Values:** 0 (disabled) or 1 (enabled)
- **Purpose:** Prints sensor values and status to Serial Monitor
- **Performance:** Disable in final deployment for speed boost

---

## 🐛 Troubleshooting

### Robot won't move
- Check power supply to motors
- Verify motor connections to L298N
- Check Arduino pin assignments in code

### Robot moves but won't follow line
- Verify sensor connections
- Check `SENSOR_THRESHOLD` (see calibration guide)
- Ensure sensors are clean and properly positioned

### Erratic movement
- Motor speeds may be unbalanced (see calibration guide)
- BASE_SPEED might be too high
- Check for loose wires or poor connections

### Robot oscillates heavily
- Reduce BASE_SPEED
- Increase TURN_FACTOR
- Increase SENSOR_THRESHOLD

### Robot loses line on curves
- Increase BASE_SPEED slightly
- Decrease TURN_FACTOR
- Check track surface quality

See `CALIBRATION_GUIDE.md` for detailed troubleshooting.

---

## 📚 Learning Resources

### Understanding PWM
PWM (Pulse Width Modulation) controls motor speed by varying the duty cycle. Analogous to an electrical switch that turns on and off rapidly - the longer it stays on relative to off, the faster the motor runs.

### Motor Control Logic
The L298N uses four control pins per motor:
- IN1 and IN2: Direction control (which way motor spins)
- EN: Speed control via PWM signal

### Sensor Types
The IR sensors typically work by:
1. Emitting infrared light
2. Measuring reflected light
3. Outputting analog voltage proportional to reflection intensity
4. Black absorbs light → low reading; White reflects → high reading

---

## 🔧 Advanced Modifications

### 1. Add Speed Control Potentiometer
- Connect potentiometer to analog input (e.g., A2)
- Read value and map to 0-255 range
- Adjust BASE_SPEED dynamically

### 2. Implement PID Control
- Use error measurement (which sensor sees line)
- Adjust motor speeds proportionally to error
- Achieve smoother line following

### 3. Add Line Following Indicators
- LED on when following line
- LED off when line is lost
- Visual feedback during operation

### 4. Add Obstacle Avoidance
- Include ultrasonic sensor
- Stop or reverse when obstacle detected
- Resume following after obstacle passes

### 5. Wireless Control
- Add HC-05 Bluetooth module
- Control robot from phone app
- Monitor sensor readings remotely

---

## 📝 Notes

- Always use separate power supplies for Arduino and motors for stability
- Sensors work best with consistent, indirect lighting
- Test calibration on the actual track surface before deployment
- Keep wheels clean for consistent speed
- Use battery pack with adequate current capacity for motors

---

## 📄 License

Feel free to modify and share this project.

---

## 🤝 Support

For issues or questions:
1. Check the CALIBRATION_GUIDE.md troubleshooting section
2. Review sensor readings in Serial Monitor
3. Verify all hardware connections
4. Test individual components (motors, sensors) separately

---
