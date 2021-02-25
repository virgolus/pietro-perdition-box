#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>


#define MAX_DEVICES 4
#define CLK_PIN   8
#define DATA_PIN  6
#define CS_PIN    7

MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

void setup(void) {
  P.begin();
  P.print("PPPPPPPP");
}

void loop(void) {
}
