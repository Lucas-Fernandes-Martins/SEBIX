#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
//Import the Liquid Crystal Library
#include <LiquidCrystal.h>
//Initialize the LCD with the arduino
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define BEGINNING 15;

int isJump = 0;

int start = 0;

unsigned long time;

unsigned long int render_time = 100;

unsigned long int jump_time = 300;

int points = 0;

byte cloud[8] = {0b01110, 0b10001, 0b11111, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000};

byte pteroDown[8] = {0b00000, // ptero with wings down
               	     0b01000, 
                     0b11000, 
                     0b00111, 
                     0b00011, 
                     0b00010, 
                     0b00000, 
                     0b00000};

byte pteroUp[8] = {0b00000,  // ptero with wings up
                   0b01010, 
                   0b11011, 
                   0b00111, 
                   0b00000, 
                   0b00000, 
                   0b00000, 
                   0b00000};

byte cactus1[8] = {
	0b00100,
	0b00100,
	0b10101,
	0b10101,
	0b11101,
	0b00111,
	0b00100,
	0b00100
};



byte cactus2[8] = {
	0b00000,
	0b00100,
	0b10101,
	0b10101,
	0b10111,
	0b11100,
	0b00100,
	0b00100
};

byte cactus3[8] = {
  	0b00000,
	0b00000,
	0b00000,
	0b00010,
	0b01010,
	0b11011,
	0b11111,
  	0b01010
};

void update_menu(){
  	lcd.setCursor(15, 0);
  	lcd.print(String(points));
}

byte* choose_cactus(){
  	int n = random(0,3);
  	byte* selected;
  switch(n){
  		case 0:
  			selected = cactus1;
  			break;
  		case 1:
  			selected = cactus2;
  			break;
 		case 2:
  			selected = cactus3;
  			break;
  	return selected;
    
  }
}

struct Obst{
     int x;
     int y;
  	 int type;
  	 byte* image;
  	 unsigned long int time;
} Obst;

void menu(){
  	lcd.clear();
    lcd.setCursor(0, 0);
  	lcd.write('-');
  	lcd.print("Play!");
  	lcd.setCursor(1, 1);
  	lcd.print("Instructions");
  	while(1){
      if(digitalRead(10)){
        delay(100);
        lcd.clear();
        break;
      }
      if(digitalRead(8)){
        delay(50);
        lcd.clear();
        break;
      }
  	}

}

void game_over(){
  lcd.clear();
  lcd.print("GAME OVER!");
  delay(500);
  lcd.clear();
  menu();
}

//time to make our charcaters and scenario

byte player[9]= {0b11111, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111};

void del(int x, int y){
    lcd.setCursor(x,y);
    lcd.write(' ');
    lcd.setCursor(1,0);
}

void move_cloud(struct Obst* o){
    	if(o->x == 0){
           del(o->x,o->y);
           o->x = BEGINNING;
           return;
    	}
  		if(o->x == 1 && isJump)
    		return;
    	 lcd.setCursor(o->x,o->y);
    	 lcd.write(' ');
    	 lcd.createChar(4, o->image);
 		 lcd.setCursor(o->x-1, o->y);
    	 lcd.write(byte(4));
    	 (o->x)--;
}
  
 void move_ptero(struct Obst* o, int ptero2){
   if(o->x == 1){
     if((!isJump && o->y == 1) || (isJump && o->y == 0)){
       game_over();
     }else{
      points++;
     }
   }
	if(o->x == 0){
      	   start++;
           del(o->x,o->y);
           return;
    	}
    	 lcd.setCursor(o->x,o->y);
    	 lcd.write(' ');
    	 lcd.createChar(5, o->image);
 		 lcd.setCursor(o->x-1, o->y);
    	 lcd.write(byte(5));
    	 (o->x)--;
}

void move_cactus(struct Obst* o){
   if(o->x == 1){
     if(!isJump){
       game_over();
     }else{
      	points++; 
     }
   }
	if(o->x == 0){
      	   start++;
           del(o->x,o->y);
           return;
    }
    	 lcd.setCursor(o->x,o->y);
    	 lcd.write(' ');
    	 lcd.createChar(6, o->image);
 		 lcd.setCursor(o->x-1, o->y);
    	 lcd.write(byte(6));
    	 (o->x)--;
}
   void move(int type, struct Obst* obst){
      switch(type){
       		case 0:
        		move_ptero(obst, 1);
        		break;
        	case 1:
        		move_cloud(obst);
        		break;
        	case 2:
        		move_cactus(obst);
        		break;
      }
  	}
   
////////////////

class Dino{
  
  public:
  
  byte dino_l[8] = {0b00000, 0b00000, 0b10000, 0b10111, 0b10101, 0b01111, 0b01100, 0b01010};

  byte dino_r[8] = {0b00000, 0b00000, 0b01111, 0b01101, 0b01111, 0b01100, 0b11111, 0b00001};
  
  byte dino_j[8]= {0b00000, 0b00000, 0b00000, 0b00000, 0b10111, 0b11101, 0b01111, 0b10100};  

  Dino(){
    lcd.begin(16,2);
    pinMode(8, INPUT); 
  }
  
  void left_dino(){
    lcd.createChar(1, this->dino_r);
    lcd.setCursor(1, 1);
    lcd.write(byte(1));
  }
  
  void right_dino(){
    lcd.createChar(1, this->dino_l);
    lcd.setCursor(1, 1);
    lcd.write(byte(1));
  }
  
  void move_dino(){
	check();
    if(!isJump){
      	del(1,0);
      	lcd.createChar(2, this->dino_l);
    	lcd.setCursor(1, 1);
    	lcd.write(byte(2));
    }else{
      	del(1,1);
      	lcd.createChar(1, this->dino_j);
    	lcd.setCursor(1, 0);
    	lcd.write(byte(1));
    }
    
  }
  
  void jump(){
    isJump = 1;
    del(1,1);
    lcd.createChar(1, this->dino_j);
    lcd.setCursor(1, 0);
    lcd.write(byte(1));
    del(1,0);
    time = millis();
  }
  
  void check(){
  	int input = digitalRead(8);
    if(isJump && millis() - time >= jump_time){
     	 isJump = 0;
    }
  	if(input == 1){
   		jump();
      	del(1,0);
  	}  
  }
  
};

struct Obst cld;

struct Obst* list;
   
Dino dino;

void setup(){
  	pinMode(6, OUTPUT);
  	pinMode(10, INPUT);
  	pinMode(13, INPUT);
  	digitalWrite(6, LOW);
  	cld.type = 1;
	cld.x = BEGINNING;
	cld.y = 0;
  	cld.image = cloud;
  	cld.time = millis();
  	list = (struct Obst*) malloc(sizeof(struct Obst)*10);
	randomSeed(digitalRead(13));
  	menu();
}

int counter = 0;

void loop(){
  
  dino.move_dino();
  
  move(cld.type, &cld);
  
  struct Obst * new_obst;
  dino.check();
  switch(random(0,25)){
   	 case 0:
    	struct Obst new_ptero;
    	new_ptero.type = 0;
    	new_ptero.x = BEGINNING;
    	new_ptero.y = random(0,2);
    	new_ptero.image = pteroUp;
    	new_ptero.time = millis();
    	move(new_ptero.type, &new_ptero);
    	list[counter] = new_ptero;
    	counter++;
    	break;
     case 1:
    	struct Obst new_cactus;
    	new_cactus.type = 2;
    	new_cactus.x = BEGINNING;
    	new_cactus.y = 1;
    	new_cactus.image = choose_cactus();
    	new_cactus.time = millis();
    	move(new_cactus.type, &new_cactus);
    	list[counter] = new_cactus;
    	counter++;
    	break;
  }
  for(int i = start; i < counter; i++){
    dino.check();
   	move(list[i].type, &(list[i]));
  }
  update_menu();
  delay(render_time);
      
}
