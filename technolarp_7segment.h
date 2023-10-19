#include <Arduino.h>
#include <TM1637Display.h>

#define CLK_PIN D5
#define DIO_PIN D3

class M_7segment
{
  private:
	bool animationActive;
	bool displayStatus;
	uint16_t iterations;
	
	uint16_t interval;
	uint32_t previousMillis;
	
	uint32_t previousMillisDoublePoint;
	uint32_t intervalDoublePoint;
	bool statutDoublePoint;
	bool blinkDoublePoint;
	
	uint32_t previousMillisBlinkAffichage;
	uint32_t intervalBlinkAffichage;
	bool blinkAffichage;
	bool blinkMinutesOuSecondes;
	bool statutBlinkAffichage;
	
	bool animForever;
	
	enum {ANIM_NONE, ANIM_BOOM, ANIM_BLINK};
	uint8_t animActuelle;
	
  public:  
	M_7segment();
	
	void showDecimal(uint16_t aDecimal, bool leadingZero);
	
	void setBlinkDoublePoint(bool toSet);
	bool getBlinkDoublePoint();
	
	void setStatutDoublePoint(bool toSet);
	bool getStatutDoublePoint();
	
	void setBlinkAffichage(bool toSet);
	bool getBlinkAffichage();
	
	void showTempsRestant(int16_t tempsRestant);
	
	void updateAnimation();
	void switchAnim(uint8_t anim);
	void switchAnimEnd(uint8_t anim);
	
	bool isAnimActive();
	void setAnimation(uint8_t toSet);
	
	void animationBoomStart(uint16_t intervalToSet, uint16_t dureeToSet);
	void animationBoom();
	void animationBoomEnd();
	
	void animationBlinkStart(uint16_t intervalToSet, uint16_t dureeToSet, bool forever);
	void animationBlink();
	void animationBlinkEnd();
	
	void showExplosee();
	void showSafe();
	
	void setStatutBlinkAffichage(bool toSet);
	
	void setBlinkMinutesOuSecondes(bool toSet);
	bool getBlinkMinutesOuSecondes();
};
