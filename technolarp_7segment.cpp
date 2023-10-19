#include "technolarp_7segment.h"

// init display TM1637
TM1637Display display_7seg(CLK_PIN, DIO_PIN);

M_7segment::M_7segment()
{
	display_7seg.setBrightness(0x0b);
	display_7seg.clear();
	
	animationActive=false;
	displayStatus=true;
	iterations = 0;
	
	interval = 100;
	previousMillis = 0;
	
	intervalDoublePoint = 500;
	previousMillisDoublePoint = 0;
	statutDoublePoint = false;
	blinkDoublePoint = false;
	
	intervalBlinkAffichage = 500;
	previousMillisBlinkAffichage = 0;
	blinkAffichage = true;
	statutBlinkAffichage = true;
		
	animForever = false;
	
	animActuelle = ANIM_NONE;
}


void M_7segment::showDecimal(uint16_t aDecimal, bool leadingZero)
{
	display_7seg.showNumberDec(aDecimal, leadingZero);
}

void M_7segment::setBlinkDoublePoint(bool toSet)
{
	blinkDoublePoint = toSet;
}

bool M_7segment::getBlinkDoublePoint()
{
	return(blinkDoublePoint);
}

void M_7segment::setStatutDoublePoint(bool toSet)
{
	statutDoublePoint = toSet;
}

bool M_7segment::getStatutDoublePoint()
{
	return(statutDoublePoint);
}
	
void M_7segment::setBlinkAffichage(bool toSet)
{
	blinkAffichage = toSet;
}

void M_7segment::setStatutBlinkAffichage(bool toSet)
{
	statutBlinkAffichage = toSet;
}

bool M_7segment::getBlinkAffichage()
{
	return(blinkAffichage);
}

void M_7segment::setBlinkMinutesOuSecondes(bool toSet)
{
	blinkMinutesOuSecondes = toSet;
}

bool M_7segment::getBlinkMinutesOuSecondes()
{
	return(blinkMinutesOuSecondes);
}

void M_7segment::showTempsRestant(int16_t tempsRestant)
{
  uint8_t data[] = { 0x00, 0x00, 0x00, 0x00 };
  uint8_t minutes=tempsRestant/60;
  uint8_t seconds=tempsRestant%60;

  data[0] = display_7seg.encodeDigit(minutes/10);
  data[1] = display_7seg.encodeDigit(minutes%10);
  data[2] = display_7seg.encodeDigit(seconds/10);
  data[3] = display_7seg.encodeDigit(seconds%10);
    
  if (statutBlinkAffichage)
  {
    if(blinkMinutesOuSecondes)
    {
      data[0] = 0x00;
      data[1] = 0x00;
    }
    else
    {
      data[2] = 0x00;
      data[3] = 0x00;    
    }
  }
  
  if (statutDoublePoint)
  {
    data[1] = data[1] | 0x80;
  }
  
  display_7seg.setSegments(data);
}

void M_7segment::showExplosee()
{
  // la bombe a deja explosee
  // afficher "- - - -" sur le 4*7segment
  uint8_t SEG_EXPLOSEE[] = {SEG_G, SEG_G, SEG_G, SEG_G};
  display_7seg.setSegments(SEG_EXPLOSEE);
}

void M_7segment::showSafe()
{
  // la bombe a deja explosee
  // afficher "S A F E" sur le 4*7segment
  uint8_t SEG_SAFE[] = {0x6D, 0x77, 0x71, 0x79};
  display_7seg.setSegments(SEG_SAFE);
}



void M_7segment::animationBoomStart(uint16_t intervalToSet, uint16_t dureeToSet)
{
	animActuelle = ANIM_BOOM;
	animationActive=true;
	displayStatus=true;
	iterations = dureeToSet / intervalToSet;
	
	interval = intervalToSet;
	animForever = false;
	
	previousMillis = millis();
}

void M_7segment::animationBoom()
{
	uint8_t SEG_VIDE[] = {0x00, 0x00, 0x00, 0x00};
	uint8_t SEG_BOOM[] = {0x7C, 0x5C, 0x5C, 0x54}; // b, o, o, n
	
	if (displayStatus)
	{
		display_7seg.setSegments(SEG_BOOM, 4, 0);
	}
	else
	{
		display_7seg.setSegments(SEG_VIDE, 4, 0);
	}
}

void M_7segment::animationBoomEnd()
{
	
}

void M_7segment::animationBlinkStart(uint16_t intervalToSet, uint16_t dureeToSet, bool forever)
{
	animActuelle = ANIM_BLINK;
	animationActive=true;
	displayStatus=true;
	iterations = dureeToSet / intervalToSet;
	
	interval = intervalToSet;
	animForever = forever;
	
	previousMillis = millis();
}

void M_7segment::animationBlink()
{
	
}

void M_7segment::animationBlinkEnd()
{
	animForever = false;
}

bool M_7segment::isAnimActive()
{
	if (animActuelle == ANIM_NONE)
	{
		return(false);
	}
	else
	{
		return(true);
	}
}

void M_7segment::setAnimation(uint8_t toSet)
{
	animActuelle = toSet;
}

void M_7segment::switchAnim(uint8_t anim)
{
	switch (anim) 
    {
      case ANIM_BOOM:
        animationBoom();
      break;
	  
      default:
        // nothing to do
      break;
    }
}

void M_7segment::switchAnimEnd(uint8_t anim)
{
	switch (anim) 
    {
      case ANIM_BOOM:
        animationBoomEnd();
      break;
	  
      default:
        // nothing to do
      break;
    }
}

void M_7segment::updateAnimation()
{
	if (blinkDoublePoint)
	{
		if(millis() - previousMillisDoublePoint > intervalDoublePoint)
		{
			previousMillisDoublePoint = millis();
			statutDoublePoint = !statutDoublePoint;
		}
	}
	
	if (blinkAffichage)
	{
		if(millis() - previousMillisBlinkAffichage > intervalBlinkAffichage)
		{
			previousMillisBlinkAffichage = millis();
			statutBlinkAffichage = !statutBlinkAffichage;
		}
	}
	
	if (animationActive)
	{
		if(millis() - previousMillis > interval)
		{
			previousMillis = millis();
			displayStatus = !displayStatus;
			
			switchAnim(animActuelle);
			
			if (iterations>0)
			{
				iterations-=1;
			}
			
			if ( (iterations==0) && animForever )
			{
				iterations = 1;
			}
			
			if (iterations==0)
			{
				animationActive = false;
				switchAnimEnd(animActuelle);
				animActuelle = ANIM_NONE;
			}
		}
	}
}