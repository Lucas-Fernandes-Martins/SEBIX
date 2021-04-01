#define NOTE_E6  1319
#define NOTE_D8  4699

//Import the Liquid Crystal Library
#include <LiquidCrystal.h>
//Initialize the LCD with the arduino
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

#define BEGINNING 15

#define MIN_DELAY 1000

#define BUTTON_DELAY 50

#define MAX_OBST 3

struct Obst* list;

int isJump = 0;

int isDown = 0;

int start = 0;

int counter = 0;

unsigned long button_time;

unsigned long time;

unsigned long int render_time = 50;

unsigned long int jump_time = 300;

int points = 0;

byte up[8]={0b00000, 0b00100,  0b01110, 0b11111, 0b00100, 0b00100, 0b00100, 0b00000}; 
byte down[8]={0b00000, 0b00100, 0b00100, 0b00100, 0b11111, 0b01110, 0b00100, 0b00000};

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
                   0b00000
};

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
  if(points < 10){
  	lcd.setCursor(15, 0);
  	lcd.print(String(points));
  }else{
     lcd.setCursor(15, 0);
     int d = points / 10;
  	lcd.print(String(points%10));
    lcd.setCursor(14, 0);
    lcd.print(String(d));
  }
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
    points=0;
  	lcd.clear();
    lcd.setCursor(0, 0);
  	lcd.write('-');
  	lcd.print("Play!");
  	lcd.setCursor(1, 1);
  	lcd.print("Instructions");
  	int pos=0;
  	while(1){
      if(!pos){
        if(digitalRead(8)){
          del(0, 0);
          lcd.setCursor(0, 1);
          pos=1;
          lcd.write('-');
        }
        if(digitalRead(7)){
          lcd.clear();
          lcd.setCursor(3, 0);
          lcd.print("Go Go Go");
          delay(300);
          lcd.clear();
          list = (struct Obst*) malloc(sizeof(struct Obst)*10);
          start = 0;
          counter = 0;
          update_menu();
          break;
      	}
      }
      if(pos){
        if(digitalRead(9)){
          del(0, 1);
          lcd.setCursor(0, 0);
          pos=0;
          lcd.write('-');
        }
        if(digitalRead(7)){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Left:Up");
          lcd.print(" ");
          lcd.print("Mid:Down");
          lcd.setCursor(0, 1);
          lcd.print("Right: Esc/Ok");
          delay(500);
          while(1){
            if(digitalRead(7)){
              lcd.clear();
              lcd.setCursor(1, 0);
              lcd.print("Play!");
              lcd.setCursor(0, 1);
              lcd.write('-');
              lcd.print("Instructions");
              delay(500);
              pos=1;
              break;
            }
          }
        }
      }
    }
  }

void game_over(){
  tone(10, NOTE_D8, jump_time);
  lcd.clear();
  free(list);
  list = NULL;
  counter = 0;
  lcd.setCursor(0,0);
  lcd.print("GAME OVER!");
  lcd.setCursor(0,1);
  lcd.print("Score: " + String(points));
  delay(1000);
  lcd.clear();
  menu();
}

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
     if(((!isJump && o->y == 1) || (isJump && o->y == 0)) && !isDown){
       game_over();
       return;
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
       return;
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
void move(struct Obst* obst){
      switch(obst->type){
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
void pause(){
    lcd.clear();
  	delay(100);
	lcd.setCursor(5, 0);
  	lcd.print("-");
  	lcd.print("Resume");
  	lcd.setCursor(6, 1);
  	lcd.print("Quit");
  	int pos=0;
    while(1){
      if(!pos){
        if(digitalRead(8)){
          del(5, 0);
          lcd.setCursor(5, 1);
          pos=1;
          lcd.write('-');
        }
        if(digitalRead(7)){
          lcd.clear();
      lcd.setCursor(3, 0);
      lcd.print("Go Go Go");
      delay(300);
      lcd.clear();
          break;
      	}
      }
      if(pos){
        if(digitalRead(9)){
          del(5, 1);
          lcd.setCursor(5, 0);
          pos=0;
          lcd.write('-');
        }
        if(digitalRead(7)){
			game_over();
        }
      }
    }
}
  	
   
////////////////

class Dino{
  
  public:
  
  byte dino_l[8] = {0b00000, 0b00000, 0b10000, 0b10111, 0b10101, 0b01111, 0b01100, 0b01010};

  byte dino_r[8] = {0b00000, 0b00000, 0b10000, 0b10111, 0b10101, 0b01111, 0b01100, 0b10100};
  
  byte dino_j[8]= {0b00000, 0b00000, 0b10000, 0b10111, 0b10101, 0b01111, 0b01100, 0b10010};

  byte dino_d1[8] = {0b00000, 0b00000, 0b00000, 0b00000, 0b10111, 0b11101, 0b01111, 0b10100};
  
  byte dino_d2[8]={0b00000, 0b00000, 0b00000, 0b00000, 0b10111, 0b11101, 0b01111, 0b01010};
  
    
  Dino(){
    lcd.begin(16,2);
    pinMode(8, INPUT); 
  }
  
  void move_dino(){
	check();
    if(!isJump){
      	del(1,0);
      	lcd.createChar(0, this->dino_l);
    	lcd.setCursor(1, 1);
    	lcd.write(byte(0));
		del(1,0);
      	lcd.createChar(0, this->dino_r);
    	lcd.setCursor(1, 1);
    	lcd.write(byte(0));
    }else{
      	del(1,1);
      	lcd.createChar(0, this->dino_j);
    	lcd.setCursor(1, 0);
    	lcd.write(byte(0));
    }
    
    if(isDown == 1){
      	del(1,0);
      	lcd.createChar(1, this->dino_d1);
    	lcd.setCursor(1, 1);
    	lcd.write(byte(1));
		del(1,0);
      	lcd.createChar(1, this->dino_d2);
    	lcd.setCursor(1, 1);
    	lcd.write(byte(1));
  }
    
  }
  
  void jump(){
    isJump = 1;
    tone(10, NOTE_E6, jump_time);
    time = millis();
  }
  
  void down_dino(){
     isDown = 1;
  }
  
  void check(){
  	int input_l = digitalRead(9);
    int input_m = digitalRead(8);
    int input_r = digitalRead(7);
    if(isJump && millis() - time >= jump_time){
     	 isJump = 0;
      	 lcd.setCursor(1, 1);
      	 button_time = millis();
    }else if (input_l == 1 && millis() - button_time >= BUTTON_DELAY && !isJump){
   		jump();
      	del(1,0);
      	button_time = 0;
      	
    }
    
    if(input_m == 1){
     	 down_dino();
    }else{
     		isDown = 0; 
    }
    
    if(input_r ==1){
      	pause();
  }
  }
  
};

struct Obst cld;
   
Dino dino;

void setup(){
  	pinMode(6, OUTPUT);
  	pinMode(8, INPUT);
  	pinMode(9, INPUT);
  	pinMode(13, INPUT);
  	digitalWrite(6, LOW);
  	cld.type = 1;
	cld.x = BEGINNING;
	cld.y = 0;
  	cld.image = cloud;
  	cld.time = millis();
  	//list = (struct Obst*) malloc(sizeof(struct Obst)*10);
	randomSeed(digitalRead(13));
  	menu();
}

int time_obst = 0;

int obj_delay=0;
int actual_time=0;
int dist=1500;
int speed=9;

int verify_position(struct Obst * list, int counter){
  for(int i = 0; i < counter; i++){
   		if(list[i].x == BEGINNING)
          return 0;
  }
  
  return 1;
}

void loop(){
  
  dino.move_dino();
  
  move(cld.type, &cld);
  
  struct Obst * new_obst;
  dino.check();
  
  if(points%5==0 && speed>5){
	speed--;
  }
  obj_delay=millis() - actual_time;
  if(obj_delay >= dist){
    
    actual_time=millis(); 
    obj_delay=0;
    dist=250*random(3, speed);
    
    switch(random(0,2)){
       case 0:
          struct Obst new_ptero;
          new_ptero.type = 0;
          new_ptero.x = BEGINNING;
          new_ptero.y = random(0,2);
          new_ptero.image = pteroUp;
          new_ptero.time = millis();
          //move(new_ptero.type, &new_ptero);
          list[counter] = new_ptero;
          counter++;
          time_obst = millis();
          break;
       case 1:
          struct Obst new_cactus;
          new_cactus.type = 2;
          new_cactus.x = BEGINNING;
          new_cactus.y = 1;
          new_cactus.image = choose_cactus();
          new_cactus.time = millis();
          //move(new_cactus.type, &new_cactus);
          list[counter] = new_cactus;
          time_obst = millis();
          counter++;
          break;

    }
  }
  for(int i = 0; i < counter; i++){
    dino.check();
   	move(&list[i]);
  }
  update_menu();
  delay(render_time);
      
}