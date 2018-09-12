void display_setup() {
  // Initializes the SSD1306 display
  
  display.init();
  display.flipScreenVertically();
  display.clear();
  display.display();
}


