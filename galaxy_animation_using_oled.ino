#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128   
#define SCREEN_HEIGHT 64   
#define OLED_RESET    -1   

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUM_STARS 100
#define NUM_PLANETS 3
#define NUM_GALAXIES 2

struct Star {
  float x, y;
  float speed;
  uint8_t brightness;
  uint8_t size;
  float depth;
};
struct Planet {
  float x, y;
  float radius;
  float speed;
  float angle;
  uint8_t brightness;
};

struct Galaxy {
  float x, y;
  float rotationSpeed;
  float angle;
  uint8_t size;
};

Star stars[NUM_STARS];
Planet planets[NUM_PLANETS];
Galaxy galaxies[NUM_GALAXIES];

void setup() {

  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  for (int i = 0; i < NUM_STARS; i++) {
    stars[i].x = random(0, SCREEN_WIDTH);
    stars[i].y = random(0, SCREEN_HEIGHT);
    stars[i].speed = random(1, 4) / 2.0;  
    stars[i].brightness = random(128, 256); 
    stars[i].size = random(1, 3); 
    stars[i].depth = random(1, 5);
  }

  for (int i = 0; i < NUM_PLANETS; i++) {
    planets[i].x = random(0, SCREEN_WIDTH);
    planets[i].y = random(0, SCREEN_HEIGHT);
    planets[i].radius = random(3, 8); 
    planets[i].speed = random(1, 3) / 10.0; 
    planets[i].angle = random(0, 360); 
    planets[i].brightness = random(128, 256); 
  }

  for (int i = 0; i < NUM_GALAXIES; i++) {
    galaxies[i].x = random(0, SCREEN_WIDTH);
    galaxies[i].y = random(0, SCREEN_HEIGHT);
    galaxies[i].rotationSpeed = random(1, 5) / 100.0; 
    galaxies[i].angle = random(0, 360); 
    galaxies[i].size = random(10, 20); 
  }

  display.clearDisplay();
}

void loop() {
  display.clearDisplay();

  for (int i = 0; i < NUM_STARS; i++) {
    stars[i].x -= stars[i].speed / stars[i].depth;  // Move star with parallax effect
    if (stars[i].x < 0) {  // Reset star position if it goes off screen
      stars[i].x = SCREEN_WIDTH;
      stars[i].y = random(0, SCREEN_HEIGHT);
      stars[i].speed = random(1, 4) / 2.0;
      stars[i].brightness = random(128, 256);
      stars[i].size = random(1, 3);
      stars[i].depth = random(1, 5);
    }

    display.fillCircle(stars[i].x, stars[i].y, stars[i].size, SSD1306_WHITE);
  }

  for (int i = 0; i < NUM_PLANETS; i++) {
    planets[i].angle += planets[i].speed;  // Update angle
    if (planets[i].angle >= 360) planets[i].angle = 0;

    planets[i].x = SCREEN_WIDTH / 2 + (SCREEN_WIDTH / 2 - planets[i].radius) * cos(planets[i].angle * PI / 180);
    planets[i].y = SCREEN_HEIGHT / 2 + (SCREEN_HEIGHT / 2 - planets[i].radius) * sin(planets[i].angle * PI / 180);
    display.fillCircle(planets[i].x, planets[i].y, planets[i].radius, SSD1306_WHITE);
  }

  for (int i = 0; i < NUM_GALAXIES; i++) {
    galaxies[i].angle += galaxies[i].rotationSpeed;  
    if (galaxies[i].angle >= 360) galaxies[i].angle = 0;

    for (int j = 0; j < galaxies[i].size; j++) {
      float angle = galaxies[i].angle + j * (360 / galaxies[i].size);
      float x = galaxies[i].x + j * cos(angle * PI / 180);
      float y = galaxies[i].y + j * sin(angle * PI / 180);
      display.drawPixel(x, y, SSD1306_WHITE);
    }
  }
  display.display();
  delay(30);  
}

