#include "LEDSTRUCT.h"


#define PIN 6

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
const int NUMPIXELS = 37;  // Total number of NeoPixels (adjust as per your actual number)
// Array of indices for StraightLines
const int STRAIGHT_LINES_INDICES[] = {0, 4, 8, 12, 16, 19, 22, 25, 28, 30, 32, 34, 36};
const int STRAIGHT_LINES_COUNT = sizeof(STRAIGHT_LINES_INDICES) / sizeof(STRAIGHT_LINES_INDICES[0]);

// Array of indices for DiagonalLines
const int DIAGONAL_LINES_INDICES[] = {1, 5, 9, 13, 17, 21, 24, 26, 29, 31, 33, 35};
const int DIAGONAL_LINES_COUNT = sizeof(DIAGONAL_LINES_INDICES) / sizeof(DIAGONAL_LINES_INDICES[0]);

const int PINWHEEL_INDICES[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36};
const int PINWHEEL_COUNT = sizeof(PINWHEEL_INDICES) / sizeof(PINWHEEL_INDICES[0]);


void applyPattern(const NeoPixelconfig& config) {
    int startIdx = 0, endIdx = 0;

    // Determine start and end indices based on geometry
    switch (config.geometry) {
        case Ring_1:
            startIdx = 0;
            endIdx = 15;
            break;
        case Ring_2:
            startIdx = 16;
            endIdx = 27;
            break;
        case Ring_3:
            startIdx = 28;
            endIdx = 35;
            break;
        case Ring_4:
            startIdx = 36;
            endIdx = 36;
            break;
        case AllRings:
            startIdx = 0;
            endIdx = 36;
            break;
        case StraightLines:
            startIdx = 0;
            endIdx = STRAIGHT_LINES_COUNT - 1;  // Use the indices array
            break;
        case DiagonalLines:  // Add this case for the new geometry
            startIdx = 0;
            endIdx = DIAGONAL_LINES_COUNT - 1;  // Use the indices array
            break;
        default:
            break;
    }

    // Apply pattern to NeoPixels within the specified range (startIdx to endIdx) using config.color
    switch (config.pattern) {
        case PULSE: {
            float brightness = 0.5 * (1 + sin(millis() / 1000.0 * 2 * PI)); // Adjust brightness over time
            for (int i = startIdx; i <= endIdx; ++i) {
                int idx;
                if (config.geometry == StraightLines) {
                    idx = STRAIGHT_LINES_INDICES[i];
                } else if (config.geometry == DiagonalLines) {  // Use diagonal lines indices
                    idx = DIAGONAL_LINES_INDICES[i];
                } else {
                    idx = i;
                }
                uint8_t r = (uint8_t)(((config.color >> 16) & 0xFF) * brightness);
                uint8_t g = (uint8_t)(((config.color >> 8) & 0xFF) * brightness);
                uint8_t b = (uint8_t)((config.color & 0xFF) * brightness);
                strip.setPixelColor(idx, strip.Color(r, g, b));
            }
            break;
        }
        case BLINK:
            for (int i = startIdx; i <= endIdx; ++i) {
                int idx = (config.geometry == StraightLines) ? STRAIGHT_LINES_INDICES[i] : ((config.geometry == DiagonalLines) ? DIAGONAL_LINES_INDICES[i] : i);
                strip.setPixelColor(idx, config.color);  // Turn on the NeoPixels
            }
            strip.show();
            delay(1000);
            for (int i = startIdx; i <= endIdx; ++i) {
                int idx = (config.geometry == StraightLines) ? STRAIGHT_LINES_INDICES[i] : ((config.geometry == DiagonalLines) ? DIAGONAL_LINES_INDICES[i] : i);
                strip.setPixelColor(idx, 0);  // Turn off the NeoPixels
            }
            strip.show();
            delay(1000);
            break;
        case STEADY:
            for (int i = startIdx; i <= endIdx; ++i) {
                int idx = (config.geometry == StraightLines) ? STRAIGHT_LINES_INDICES[i] : ((config.geometry == DiagonalLines) ? DIAGONAL_LINES_INDICES[i] : i);
                strip.setPixelColor(idx, config.color);  // Set the NeoPixels to the specified color
            }
            break;
        case TWINKLE:
            for (int i = startIdx; i <= endIdx; ++i) {
                int idx = (config.geometry == StraightLines) ? STRAIGHT_LINES_INDICES[i] : ((config.geometry == DiagonalLines) ? DIAGONAL_LINES_INDICES[i] : i);
                if (random(100) < 10) { // 10% chance to set a pixel
                    strip.setPixelColor(idx, config.color);
                } else {
                    strip.setPixelColor(idx, 0);  // Turn off the NeoPixel
                }
            }
            break;
       case SPARKLE: {
    // Clear NeoPixels to start with a clean slate
    strip.clear();

    // Loop through each color transition in the pinwheel
    for (int offset = 0; offset < (endIdx - startIdx + 1); ++offset) {
        // Calculate the current color index based on offset
        int currentColorIdx = offset % 3;  // Assuming 3 colors for example

        // Calculate the previous and next color indices
        int prevColorIdx = (currentColorIdx == 0) ? 2 : currentColorIdx - 1;
        int nextColorIdx = (currentColorIdx + 1) % 3;

        // Set colors for each NeoPixel based on its position
        for (int i = startIdx; i <= endIdx; ++i) {
            int idx = i;
            if ((i - startIdx) % 3 == prevColorIdx) {
                strip.setPixelColor(idx, strip.Color(0, 0, 0));  // Previous color
            } else if ((i - startIdx) % 3 == currentColorIdx) {
                strip.setPixelColor(idx, config.color);  // Current color
            } else if ((i - startIdx) % 3 == nextColorIdx) {
                strip.setPixelColor(idx, strip.Color(0, 0, 0));  // Next color
            }
        }

        strip.show();  // Update NeoPixel strip after setting colors
        delay(200);  // Adjust delay as needed for animation speed
    }

    strip.clear();  // Clear NeoPixels after completing the pinwheel
    break;
 }
    default:
            break;
    }

    strip.show();  // Update NeoPixel strip after setting colors
 }



// Function to apply a pattern to NeoPixels based on geometry and pattern
void applyPatternToNeoPixels(Geometry geom, Pattern pat, uint32_t color) {
    NeoPixelconfig config = {geom, pat, color};
    applyPattern(config);
}
