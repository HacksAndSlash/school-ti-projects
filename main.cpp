#include <tice.h>
#include <graphx.h>
#include <keypadc.h>
#include <fileioc.h>
#include <stdlib.h>
#include <math.h>
#include <debug.h>
#include <gfx/gfx.h>
#include <stdlib.h>
#include <string.h>
int lives = 3;
int points = 0;
uint16_t x = 320/2-8;
int counter = 0;
int enemyFireCounter = 0;
bool Bfire = true;
uint8_t y = 240/2-8;
bool eDir;
float EY = 0;
float EX = 0;
float shotX = (320/2)-8,shotY = 0;
float shotDir = atan2(x-shotX,y-shotY);
int8_t shotDY = 5;
int8_t shotDX = 5;
float difficulty = 1;
bool wave;
float waveNum = 1;
int score = 0;

uint8_t powerupType;
uint16_t pwrx,pwry;

bool hasSheild, hasLaser;

uint8_t eList[8][8] = 
{	
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1},
	{1,1,1,1,1,1,1,1}
};
char* float2str(float value)
{
    real_t tmp_real = os_FloatToReal(value);
	char* str = (char*)" ";
    os_RealToStr(str, &tmp_real, 8, 1, 2);
	return str;
}
void fire()
{
	Bfire = false;
	gfx_SetColor(224);
	gfx_Line(x+11,240-10,x+8,y+8);
	gfx_Line(x+5,240-10,x+8,y+8);
	for(int i = 0; i < 8; i++)
	for(int j = 0; j < 8; j++)
	{
		if(eList[i][j] && (i*16 + EX >= x - 8) && (i*16 + EX <= x + 8) && (j*16 + EY >= y - 8) && (j*16 + EY <= y + 8))
		{
			eList[i][j] = 0;
			score++;
		}
	}
	
	if(hasLaser)
	{
		for(int i = 0; i < 16; i++)
		{
			gfx_Line(x+i,240-10,x+i,0);
		}
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++)
			{
				if(eList[i][j] && (i*16 + EX >= x - 8) && (i*16 + EX <= x + 8))
				{
					eList[i][j] = 0;
					score++;
				}
			}
		hasLaser = false;
	}
	
	if((x - 8 <= pwrx) && (x + 8 >= pwrx) && (y - 8 <= pwry) && (y + 8 >= pwry))
	{
		pwrx = 400;
		switch (powerupType)
		{
			case 0:
				lives++;
				break;
			case 1:
				while(kb_Data[1] & kb_2nd)
					kb_Scan();
				hasLaser = true;
				break;
			case 2:
				hasSheild = true;;
				break;
		}
		powerupType = rand() % 3;
	}
}

void PowerupAct()
{
	switch (powerupType)
		{
			case 0:
				gfx_TransparentSprite(pwr_0,pwrx,pwry);
				break;
			case 1:
				gfx_TransparentSprite(pwr_1,pwrx,pwry);
				break;
			case 2:
				gfx_TransparentSprite(pwr_2,pwrx,pwry);
				break;
		}
	if(pwrx == 400 && rand() >= 0.999)
	{
		pwrx = rand() % 320;
		pwry = rand() % 240;
	}
}

void EnemyAct()
{
	wave = false;
	for(int i = 0; i < 8; i++)
	for(int j = 0; j < 8; j++)
	{
		if(eList[i][j])
		{
			wave = true;
			gfx_TransparentSprite(TIE,i*16 + EX,j*16 + EY);
			if(shotY>240)
			{
				shotY = j*16 + EY;
				shotX = i*16 + EX;
				shotDir = atan2(x-shotX,224-shotY);
				shotDir += ((rand() % 10) - 5) * (1/difficulty) * 0.06;
			}
		}
		if(shotX <= x + 15 && shotX >= x - 15 && shotY >= 240-32 && shotY <= 240)
		{
			if(!hasSheild)
			{
				lives--;
				x = 320/2-8;
			}
			hasSheild = false;
			shotY = 256;
		}
		
	}
	if(!wave)
	{
		score+=250;
		waveNum+=0.1;
		
		for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++)
				eList[i][j] = (int)waveNum;
	}
	if(cos(shotDir) == 0.0f)
		shotDir = atan2(x-shotX,224-shotY);
	shotX-=shotDX*difficulty*sin(shotDir);
	shotY+=shotDY*difficulty*cos(shotDir);
	if(shotX < 0)
		shotDX *= -1;
	if(shotX > 304)
		shotDir *= -1;
	if(cos(shotDir) < 0.0f)
		shotDY = -5;
	else
		shotDY = 5;
	
	gfx_TransparentSprite(E_SHOT,shotX,shotY);
	if(EX<=0)
		eDir = true;
	if(EX >= 192)
		eDir = false;

	if(eDir)
		EX+=0.5;
	else
		EX-=0.5;
}	
void input()
{
	kb_key_t arrows;
	
		if(counter > 4)
		{
			Bfire = true;
			counter = 0;
		} else 
			counter++;
		/* Scan the keypad to update kb_Data */
		kb_Scan();

		/* Get the arrow key statuses */
		arrows = kb_Data[7];

		/* Check if any arrows are pressed */
		if (arrows)
		{
            /* Do different directions depending on the keypress */
            if (arrows & kb_Right)
            {
                x += 2;
            }
            if (arrows & kb_Left)
            {
                x -= 2;
            }
            if (arrows & kb_Down)
            {
                y += 2;
            }
            if (arrows & kb_Up)
            {
                y -= 2;
            }
			if(x == 320)
				x = 0;
			if(x > 320)
				x = 320;
			
		}	
		if(kb_Data[1] & kb_2nd && Bfire)
		fire();
}
void menu()
{
	lives = 3;
	for(int i = 0; i < 8; i++)
			for(int j = 0; j < 8; j++)
				eList[i][j] = 1;
	waveNum = 1;
	score = 0;
	kb_Scan();
	bool key,prevkey;
	while(!(kb_On) && !(kb_Data[1] & kb_2nd))
	{
		kb_Scan();
		gfx_ScaledSprite_NoClip(background, 0, 0, 10, 10);
		gfx_PrintStringXY("PRESS 2ND TO BEGIN!",0,200);
		gfx_PrintStringXY("PRESS UP/DOWN TO CHANGE DIFFICULTY",0,215);
		switch((int)(difficulty * 2))
		{
			case 2:
				gfx_PrintStringXY("DIFFICULTY: 'Baby's First Plasma Rifle'",0,175);
				break;
			case 3:
				gfx_PrintStringXY("DIFFICULTY: 'Mars Vacation'",0,175);
				break;
			case 4:
				gfx_PrintStringXY("DIFFICULTY: 'Now This Is Podracing'",0,175);
				break;
			case 5:
				gfx_PrintStringXY("DIFFICULTY: 'Beat Me Up Scotty'",0,175);
				break;
			case 6:
				gfx_PrintStringXY("DIFFICULTY: 'Order 66'",0,175);
				break;
		}
		kb_key_t arrows;
		kb_Scan();
		gfx_BlitBuffer();
		/* Get the arrow key statuses */
		arrows = kb_Data[7];
		key = (arrows == 0);
		/* Check if any arrows are pressed */
		if (arrows)
		{
            /* Do different directions depending on the keypress */
            if (arrows & kb_Down && !key && prevkey)
            {
                difficulty-=0.5;
            }
            if (arrows & kb_Up && !key && prevkey)
            {
                difficulty+=0.5;
            }
			if(difficulty > 3)
			{
				difficulty = 3;
			}
			if(difficulty == 0.5)
			{
				difficulty = 1;
			}
		}
		prevkey = key;
		
	}
		
}
void gameOver()
{
		while(kb_Data[1] & kb_2nd)
			kb_Scan();
		uint8_t temp = 0;
		
		uint8_t* High = &temp;
		
		ti_var_t hs = ti_Open("XW3HS","r");
		
		ti_Read(High,4,1,hs);
		ti_Close(hs);
		if(*High < score)
		{
			hs = ti_Open("XW3HS","w");
			ti_PutC((char)score,hs);
			ti_Close(hs);
			pgrm_CleanUp();
		} else
		{
			ti_Close(hs);
			pgrm_CleanUp();
		}
		char* appendValue1 = float2str(*High);
		char* appendValue2 = float2str(score);
		gfx_ScaledSprite_NoClip(background, 0, 0, 10, 10);

		
		gfx_PrintStringXY("YOUR SCORE:",(24+(16*11)),100);
		gfx_PrintStringXY(appendValue2,(24+(16*11)),110);
		
		gfx_PrintStringXY("PREV BEST:" ,24,100);
		gfx_PrintStringXY(appendValue1,24,110);
		
		gfx_PrintStringXY("PRESS 2ND TO CONTINUE",24,160);
		gfx_BlitBuffer();
		while(!(kb_Data[1] & kb_2nd))
			kb_Scan();
		while(kb_Data[1] & kb_2nd)
			kb_Scan();
}
int main()
{
	srand (0);
	gfx_Begin();
	gfx_SetPalette(global_palette, sizeof_global_palette, 0);
	gfx_SetDrawBuffer();
    gfx_FillScreen(1);
    gfx_SetTransparentColor(0);
	gfx_ScaledSprite_NoClip(background, 0, 0, 10, 10);
	gfx_BlitBuffer();
	gfx_SetTextBGColor(255);
	menu();
	while(true)
	{
		if(kb_On)
			break;
		while(lives > 0)
		{
			gfx_BlitBuffer();
			gfx_ScaledSprite_NoClip(background, 0, 0, 10, 10);
			EnemyAct();
			PowerupAct();
			gfx_TransparentSprite(xwing, x, 240-16);
		
			gfx_TransparentSprite(crosshair, x, y);
			input();
		}
		gameOver();
		menu();
	}
	while(kb_On)
	{
		kb_Scan();
	}
	gfx_End();
	return 0;
	
}

