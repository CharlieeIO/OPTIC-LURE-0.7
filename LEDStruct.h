#ifndef LEDSTRUCT_H
#define LEDSTRUCT_H

 #include <Adafruit_NeoPixel.h>
 #include <Adafruit_DotStar.h>

extern const int PIN;

extern const int STRAIGHT_LINES_INDICES[];
extern const int STRAIGHT_LINES_COUNT;
extern const int DIAGONAL_LINES_INDICES[];
extern const int DIAGONAL_LINES_COUNT;
extern const int PINWHEEL_INDICES[];
extern const int PINWHEEL_COUNT;
extern const int NUMPIXELS;
extern Adafruit_NeoPixel strip;
enum Pattern {
    PULSE,
    BLINK,
    STEADY,
    TWINKLE,
    SPARKLE,
    PINWHEEL
};

// Enum to define different geometries
enum Geometry { // Wanting to add new Geom just add , +(geom)
    Ring_1,
    Ring_2,
    Ring_3,
    Ring_4,
    StraightLines,
    DiagonalLines,  
    AllRings
};

//NeoPixel Data Struct
struct NeoPixelconfig {
    Geometry geometry;
    Pattern pattern;
    uint32_t color;
};

void applyPattern(const NeoPixelconfig& config);
void applyPatternToNeoPixels(Geometry geom, Pattern pat, uint32_t color);


#endif // LEDSTRUCT_h