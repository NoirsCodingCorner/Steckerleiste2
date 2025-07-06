
#define DEBUG_I2C_TIMING

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "attiny_communication.h"
#include "lightstrip.h"
#include "main_flow.h"  // Enthält die Klasse MainFlow

#define LED_BUILTIN 2

// === Wi-Fi AP + WebServer Configuration ===
const char* ssid     = "KilisStrip";
const char* password = "12345678";  // mindestens 8 Zeichen
WebServer server(80);

// Einfaches HTML mit Slider (0–128) und JavaScript, das GET-Requests an /set?value=xx schickt
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="de">
  <head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>KilisStrip Helligkeit</title>
    <style>
      /* Reset-Stile */
      * {
        margin: 0;
        padding: 0;
        box-sizing: border-box;
      }
      html, body {
        height: 100%;
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
        background-color: #000000;        /* Start schwarz */
        color: #39ff14;                    /* Neon-Gelb/Grün für Text */
        transition: background 0.2s ease;  /* sanfter Farbübergang */
      }

      /* Zentriertes Container-Card */
      .container {
        display: flex;
        align-items: center;
        justify-content: center;
        height: 100%;
        padding: 20px;
      }
      .card {
        background: #000000;               /* Schwarze Karte */
        border: 2px solid #39ff14;         /* Neon-Rahmen */
        border-radius: 12px;
        padding: 30px;
        max-width: 380px;
        width: 100%;
        text-align: center;
        box-shadow: 0 0 15px rgba(57, 255, 20, 0.8);
      }
      .card h1 {
        font-size: 1.8rem;
        margin-bottom: 20px;
        color: #39ff14;
        text-shadow: 0 0 8px rgba(57, 255, 20, 0.7);
      }
      .brightness-display {
        font-size: 2.5rem;
        font-weight: bold;
        margin: 15px 0;
        color: #39ff14;
        text-shadow: 0 0 6px rgba(57, 255, 20, 0.7);
      }
      .slider-container {
        position: relative;
        width: 100%;
      }
      .slider {
        -webkit-appearance: none;
        width: 100%;
        height: 8px;                /* Track-Höhe */
        border-radius: 4px;
        background: #222222;        /* Dunkles Grau/Schwarz für Track */
        outline: none;
        margin: 30px 0;
        transition: background 0.2s ease; /* sanfter Track-Fill-Übergang */
      }
      .slider:hover {
        background: #333333;
      }
      /* Thumb-Stil (WebKit) */
      .slider::-webkit-slider-thumb {
        -webkit-appearance: none;
        appearance: none;
        width: 24px;
        height: 24px;
        border-radius: 50%;
        background: #39ff14;              /* Neon-Gelb/Grün */
        border: 2px solid #ffffff;
        cursor: pointer;
        box-shadow: 0 0 10px rgba(57, 255, 20, 0.8);
        margin-top: -8px;                 /* Thumb zentriert auf 8px-Track */
        transition: background 0.2s ease, transform 0.2s ease;
      }
      .slider::-webkit-slider-thumb:hover {
        background: #a0ff5c;
        transform: scale(1.1);
      }
      /* Thumb-Stil (Firefox) */
      .slider::-moz-range-thumb {
        width: 24px;
        height: 24px;
        border-radius: 50%;
        background: #39ff14;
        border: 2px solid #ffffff;
        cursor: pointer;
        box-shadow: 0 0 10px rgba(57, 255, 20, 0.8);
        margin-top: 0; /* Firefox zentriert automatisch */
        transition: background 0.2s ease, transform 0.2s ease;
      }
      .slider::-moz-range-thumb:hover {
        background: #a0ff5c;
        transform: scale(1.1);
      }
      /* Track-Füllung (WebKit) */
      .slider::-webkit-slider-runnable-track {
        height: 8px;
        background: linear-gradient(
          to right,
          #39ff14 0%,
          #39ff14 0%,
          #222222 0%,
          #222222 100%
        );
        border-radius: 4px;
      }
      /* Track-Füllung (Firefox) */
      .slider::-moz-range-track {
        height: 8px;
        background: linear-gradient(
          to right,
          #39ff14 0%,
          #39ff14 0%,
          #222222 0%,
          #222222 100%
        );
        border-radius: 4px;
      }
      /* Footer */
      .footer {
        margin-top: 20px;
        font-size: 0.9rem;
        color: #777777;
      }
      .footer a {
        color: #39ff14;
        text-decoration: none;
      }
      .footer a:hover {
        text-decoration: underline;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <div class="card">
        <h1>KilisStrip Helligkeit</h1>
        <div class="brightness-display" id="valueDisplay">128</div>
        <div class="slider-container">
          <input
            type="range"
            min="0"
            max="128"
            value="128"
            id="brightnessSlider"
            class="slider"
          />
        </div>
        <div class="footer">
          Verbinde dich mit dem WLAN „<strong>KilisStrip</strong>“<br />
          Powered by ESP32
        </div>
      </div>
    </div>

    <script>
      document.addEventListener("DOMContentLoaded", () => {
        const slider  = document.getElementById("brightnessSlider");
        const display = document.getElementById("valueDisplay");

        // Farb-Mischfunktion: von Schwarz (0) → Neon-Gelb (128)
        function mixColor(val) {
          const max = parseInt(slider.max, 10);
          const t = val / max;
          // Neon-Gelb (#39ff14) entspricht (57,255,20)
          const r = Math.round(57 * t);
          const g = Math.round(255 * t);
          const b = Math.round(20 * t);
          return `rgb(${r},${g},${b})`;
        }

        // Aktualisiert Track-Füllung und Hintergrund-Farbhelligkeit
        function updateVisuals(val) {
          display.textContent = val;
          const max = parseInt(slider.max, 10);
          const percentage = (val / max) * 100;

          // Track-Filling: Neon (#39ff14) bis 'percentage' %, Rest #222222
          const trackGradient = `
            linear-gradient(
              to right,
              #39ff14 0%,
              #39ff14 ${percentage}%,
              #222222 ${percentage}%,
              #222222 100%
            )`;
          slider.style.background = trackGradient;

          // Hintergrund: skaliert nur die untere Farbe von Schwarz → Neon
          // Wir setzen: Hintergrundfarbe = Schwarz, aber übergreifender
          // Farb-Layer: rgba(Neon, t), sodass abgedunkelt → aufgehellt
          const neonRgb = mixColor(val);
          document.body.style.background = `
            linear-gradient(
              to bottom,
              #000000 0%,
              ${neonRgb} 100%
            )`;
        }

        // Initiales Rendering
        updateVisuals(parseInt(slider.value, 10));

        // Event-Listener für Slider-Änderung
        slider.addEventListener("input", () => {
          const val = parseInt(slider.value, 10);
          updateVisuals(val);
          fetch(`/set?value=${val}`)
            .catch((err) => console.error("Fehler beim Senden:", err));
        });
      });
    </script>
  </body>
</html>
)rawliteral";




MainFlow flow;

// Handler für "/" → Seite ausliefern
void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

// Handler für "/set?value=<0..128>"
void handleSet() {
  if (server.hasArg("value")) {
    String valStr = server.arg("value");
    int brightness = valStr.toInt();
    if (brightness < 0)   brightness = 0;
    if (brightness > 128) brightness = 128;
    Serial.print("Brightness set to: ");
    Serial.println(brightness);
    flow.brightness=brightness;

    server.send(200, "text/plain", "OK");
    return;
  }
  server.send(400, "text/plain", "Missing 'value' parameter");
}

// === Bestehende MainFlow-Instanz ===


// Für zeitgesteuertes Zurücksetzen Ihrer Logik
unsigned long lastTriggeredTime = 0;
const unsigned long resetInterval = 10000;  // 10 Sekunden

void setup() {
  // *** Serielle Schnittstelle initialisieren ***
  Serial.begin(9600);
  delay(100);

  pinMode(LED_BUILTIN, OUTPUT);

  // Sensor-Adressen für MainFlow setzen (0x5C und 0x5D)
  flow.comm.addAddress(0x6A);
  flow.comm.addAddress(0x6B);
  flow.comm.addAddress(0x6C);
  flow.comm.addAddress(0x6D);
  flow.comm.addAddress(0x6E);

  flow.init();
  Serial.println("System gestartet.");

  // *** Wi-Fi Access Point starten ***
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.print("\" gestartet. IP-Adresse: ");
  Serial.println(IP);

  // *** WebServer-Routen registrieren ***
  server.on("/", HTTP_GET, handleRoot);
  server.on("/set", HTTP_GET, handleSet);
  server.begin();
  Serial.println("HTTP-Server gestartet");
}


int measure=0;
void loop() {
  server.handleClient();

  if (measure==0)
  {
    flow.run();
    flow.lightStrip.updateFades();    
    measure=50;

  }
  else{
    flow.lightStrip.updateFades();
    measure--;
  }
  

  delay(1);  // 1 Sekunde
}
