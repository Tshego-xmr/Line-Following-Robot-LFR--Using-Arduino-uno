/*
 * Line Follower Robot - Configuration Header
 * 
 * Adjust these values to fine-tune robot behavior for your specific setup.
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================
// SENSOR CONFIGURATION
// ============================================

/*
 * SENSOR_THRESHOLD: The analog value below which a sensor is considered
 * to be detecting the black line.
 * 
 * Typical values:
 * - White surface: ~900-1023
 * - Black line: ~0-100
 * - Recommended threshold: 500 (middle point)
 * 
 */
#define SENSOR_THRESHOLD 100

// ============================================
// MOTOR CONFIGURATION
// ============================================

/*
 * BASE_SPEED: Default speed for forward motion (0-255)
 * 
 * Start with 150-200 for stable line following.
 * Higher values = faster but may overshoot line.
 * Lower values = slower but more stable.
 */
#define BASE_SPEED 125

/*
 * TURN_FACTOR: Multiplier for turn speed relative to base speed (0.0-1.0)
 * 
 * When turning, the inner motor runs at BASE_SPEED * TURN_FACTOR.
 * Lower values = sharper turns, more responsive
 * Higher values = gentler turns, smoother movement
 * 
 * Recommended: 0.5-0.7
 */
#define TURN_FACTOR 0.6

// ============================================
// DEBUG CONFIGURATION
// ============================================

/*
 * DEBUG_MODE: Enable serial debugging output
 * 
 * When enabled, sensor values and status are printed every 500ms.
 * Disable for faster operation or to reduce serial communication overhead.
 */
#define DEBUG_MODE 0

#endif // CONFIG_H
