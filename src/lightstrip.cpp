#include "LightStrip.h"

//
// Konstruktor: Initialisiert die beiden Vektoren und NeoPixel‐Instanz
//
LightStrip::LightStrip(int numPixels, int pin)
    : strip(numPixels, pin, NEO_GRB + NEO_KHZ400),
      ledValues(numPixels, 0),
      targetValues(numPixels, 0)
{
}

//
// Hardware‐Initialisierung (muss in setup() aufgerufen werden)
//
void LightStrip::begin() {
    strip.begin();
    strip.clear();
    strip.show();
}

//
// Schaltet ALLE LEDs sofort aus (ledValues[] und strip auf 0 setzen)
//
void LightStrip::clear() {
    for (size_t i = 0; i < ledValues.size(); ++i) {
        ledValues[i] = 0;
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        targetValues[i] = 0;
    }
    strip.show();
}

//
// Setzt eine EINZELNE LED (Index) sofort auf 'value' (0–255), ohne Fading
//
void LightStrip::setLedValue(int index, uint8_t value) {
    if (index < 0 || index >= (int)ledValues.size()) return;
    ledValues[index]  = value;
    targetValues[index] = value;  // auch Sollwert direkt anpassen, damit kein Fade passiert

    uint8_t r = (uint8_t)( (double)value * warmth );
    uint8_t g = (uint8_t)( (double)value * (1.0 - warmth) );
    strip.setPixelColor(index, strip.Color(r, g, 0));
}

//
// Diesen Aufruf machen, um alle aktualisierten ledValues[] ans Strip zu senden
//
void LightStrip::show() {
    strip.show();
}

//
// =============================================================
// Blockierende Methoden (bleiben unverändert, falls Du sie nutzt)
// =============================================================

void LightStrip::updateWave(int waveHeadIndex) {
    clear();
    for (int j = 0; j < WAVE_LENGTH; j++) {
        int pos = waveHeadIndex - j;
        if (pos >= 0 && pos < (int)ledValues.size()) {
            int brightness = 255 - (j * (255 / WAVE_LENGTH));
            setLedValue(pos, brightness);
        }
    }
    show();
}

void LightStrip::setAll(uint8_t brightness) {
    for (int i = 0; i < strip.numPixels(); ++i) {
        ledValues[i]   = brightness;
        targetValues[i] = brightness;
        uint8_t r = (uint8_t)((double)brightness * warmth);
        uint8_t g = (uint8_t)((double)brightness * (1.0 - warmth));
        strip.setPixelColor(i, strip.Color(r, g, 0));
    }
    show();
}

void LightStrip::direction_wave(int wavedirection, double brightness, int delaytime) {
    size_t n = ledValues.size();
    if (wavedirection > 0) {
        for (size_t i = 0; i < n; ++i) {
            clear();
            setLedValue(i, (uint8_t)brightness);
            show();
            delay(delaytime);
        }
    }
    else if (wavedirection < 0) {
        for (int i = int(n) - 1; i >= 0; --i) {
            clear();
            setLedValue(i, (uint8_t)brightness);
            show();
            delay(delaytime);
        }
    }
    else {
        clear();
        show();
    }
}

void LightStrip::setAllFromAnalog(int analogPin) {
    int analogValue = analogRead(analogPin);
    Serial.println("Analog Value: " + String(analogValue));
    int brightness = map(analogValue, 0, 4095, 0, 255);
    brightness = constrain(brightness, 0, 255);
    setAll((uint8_t)brightness);
}

void LightStrip::fadeTo(uint8_t targetBrightness, int stepDelay) {
    uint8_t current = ledValues.empty() ? 0 : ledValues[0];
    if (current < targetBrightness) {
        for (uint8_t b = current; b < targetBrightness; ++b) {
            setAll(b);
            show();
            delay(stepDelay);
        }
    }
    else if (current > targetBrightness) {
        for (int b = current; b >= targetBrightness; --b) {
            setAll((uint8_t)b);
            show();
            delay(stepDelay);
        }
    }
}

void LightStrip::fadeOut(int stepDelay) {
    fadeTo(0, stepDelay);
}

//
// =============================================================
// NEU: Nicht-blockierendes Segment-Fading (vereinfachte Variante)
// =============================================================

//
// 1) setSegmentTarget: Setzt für alle LEDs im Bereich [startIndex..endIndex]
//    denselben Sollwert (0..255). Im nächsten updateFades-Laudaufruf faden sie dorthin.
//
void LightStrip::setSegmentTarget(int startIndex, int endIndex, uint8_t targetBrightness) {
    // Grenzen prüfen und anpassen
    if (startIndex < 0)              startIndex = 0;
    if (endIndex >= (int)targetValues.size()) endIndex = (int)targetValues.size() - 1;
    if (startIndex > endIndex)       return;

    for (int i = startIndex; i <= endIndex; ++i) {
        targetValues[i] = targetBrightness;
    }
}

//
// 2) updateFades: Muss in loop() regelmäßig aufgerufen werden. 
//    Hier wird jede LED einzeln um ±1 an den in targetValues[…] definierten Sollwert herangeführt.
//    Wenn sich irgendetwas ändert, wird strip.show() aufgerufen.
//
//    ACHTUNG: Kein delay() hier! Diese Methode soll so schnell wie möglich in jedem loop-Durchlauf laufen.
//
void LightStrip::updateFades() {
    bool anyChange = false;

    for (size_t i = 0; i < ledValues.size(); ++i) {
        uint8_t curr   = ledValues[i];
        uint8_t target = targetValues[i];
        if (curr < target) {
            curr++;
            anyChange = true;
        }
        else if (curr > target) {
            curr--;
            anyChange = true;
        }

        if (anyChange) {
            ledValues[i] = curr;
            uint8_t r = (uint8_t)((double)curr * warmth);
            uint8_t g = (uint8_t)((double)curr * (1.0 - warmth));
            strip.setPixelColor(i, strip.Color(r, g, 0));
        }
    }

    if (anyChange) {
        strip.show();
    }
}
