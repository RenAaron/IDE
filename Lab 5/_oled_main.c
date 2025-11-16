#include "i2c.h"
#include "oled.h"
#include "switches.h"

unsigned char OLED_TEXT_ARR[1024];
unsigned char OLED_GRAPH_ARR[1024];

void art1(){
	OLED_PrintLine("                ");
	OLED_PrintLine("        O       ");
	OLED_PrintLine("       /|\\      ");
	OLED_PrintLine("        /\\      ");
}

void art2(){
	OLED_PrintLine("                ");
	OLED_PrintLine("        O/     ");
	OLED_PrintLine("       /|       ");
	OLED_PrintLine("        /\\      ");
}

void art3(){
	OLED_PrintLine("                ");
	OLED_PrintLine("        \\O     ");
	OLED_PrintLine("        |\\       ");
	OLED_PrintLine("        /\\      ");
}
	
int main(){
    OLED_Init();
    
  // OLED_display_off();
  OLED_display_on();
    OLED_display_clear();
    OLED_display_on();
    
    
    OLED_PrintLine("Hello World");
    OLED_PrintLine("How are you");
    OLED_PrintLine("Goodbye");

    uint16_t myNumbers[128];
    
    for(int i = 128; i > 0; i--){
         myNumbers[i] = (128-i)*i*4;
    }
    
    OLED_DisplayCameraData(myNumbers);
        
    return 0;
}
