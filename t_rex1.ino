#define NOTE_E6  1319
#define NOTE_D8  4699
//Import the Liquid Crystal Library
#include <LiquidCrystal.h>
//Initialize the LCD with the arduino
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//time to make our charcaters and scenario

byte selector[8]= {0b00000, 0b00000, 0b01110,0b11111, 0b11111, 0b11111, 0b01110, 0b00000};

byte dinoA[8]={0b00000, 0b00000, 0b10000, 0b10111, 0b10101, 0b01111, 0b01100, 0b01100};
byte dinoB[8]={0b00000, 0b00000, 0b10000, 0b10111, 0b10101, 0b01111, 0b01100, 0b01010};
byte dinoC[8]={0b00000, 0b00000, 0b10000, 0b10111, 0b10101, 0b01111, 0b01100, 0b10010};
byte dinoD[8]={0b00000, 0b00000, 0b10000, 0b10111, 0b10101, 0b01111, 0b01100, 0b10100};

byte DowndinoA[8]={0b00000, 0b00000, 0b00000, 0b00000, 0b10111, 0b11101, 0b01111, 0b10100};
byte DowndinoB[8]={0b00000, 0b00000, 0b00000, 0b00000, 0b10111, 0b11101, 0b01111, 0b01100};
byte up[8]={0b00000, 0b00100,  0b01110, 0b11111, 0b00100, 0b00100, 0b00100, 0b00000}; 
byte down[8]={0b00000, 0b00100, 0b00100, 0b00100, 0b11111, 0b01110, 0b00100, 0b00000};



void setup(){
  	//Serial.begin(9600);
  	pinMode(9, INPUT); //up button
    pinMode(8, INPUT); //down button
  	pinMode(7, INPUT); //ok button
  	pinMode(10, OUTPUT);
  	pinMode(6, OUTPUT);
	lcd.begin(16,2);
  	digitalWrite(6, LOW);
  	lcd.createChar(1, dinoA);
  	lcd.createChar(2, dinoB);
  	lcd.createChar(3, dinoC);
  	lcd.createChar(4, dinoD);
  	lcd.createChar(5, DowndinoA);
  	lcd.createChar(6, DowndinoB);
  	lcd.createChar(7, up);
  	lcd.createChar(8, down);
  	lcd.createChar(0, selector);
  
  	Menu();
                   
}
                   
void erasechar(int x, int y){
  lcd.setCursor(x, y);
  lcd.print(" ");
  lcd.setCursor(x, y);
  
}

void Menu(){
  	lcd.clear();
    lcd.setCursor(0, 0);
  	lcd.write(byte(0));
  	lcd.print("Play!");
  	lcd.setCursor(1, 1);
  	lcd.print("Instructions");
}

void play(){
  	int endgame=0;
	lcd.clear();
  	lcd.setCursor(7,0);
    lcd.print("GO!");
  	delay(500);
  	lcd.clear();
  	lcd.setCursor(4, 1);
  	int up=0;
  	int down=0;
  while(!endgame){
    
    up=0; down=0;
    digitalWrite(6, LOW);
    
    while(!up && !down){
      
      if(up || down){
		break;
      }
      lcd.write(byte(1));
      delay(100);
      erasechar(4, 1);
      
      up=digitalRead(9);
      down=digitalRead(8);
      if(up || down){
		break;
      }
      lcd.write(byte(2));
      delay(100);
      erasechar(4, 1);
      
      up=digitalRead(9);
      down=digitalRead(8);
      if(up || down){
		break;
      }
      
      lcd.write(byte(3));
      delay(100);
      erasechar(4, 1);
      
      up=digitalRead(9);
      down=digitalRead(8);
      if(up || down){
		break;
      }
      
      lcd.write(byte(4));
      delay(100);
      erasechar(4, 1);
    }
    
    if(up){
      digitalWrite(6, HIGH);
      erasechar(4, 1);
      lcd.setCursor(4, 0);
      lcd.write(byte(3));
      tone(10, NOTE_E6, 100);
      delay(500);
      erasechar(4, 0);
      lcd.setCursor(4, 1);
    }
    if(down){
      digitalWrite(6, HIGH);
      erasechar(4, 1);
      while(down){
      	lcd.write(byte(5));
      	delay(100);
        
        down=digitalRead(8);
      if(!down){
        erasechar(4, 1);
		break;
      }
      	erasechar(4, 1);
      	lcd.write(byte(6));
      	delay(100);
        
        down=digitalRead(8);
      if(!down){
        erasechar(4, 1);
		break;
      }
        
      	erasechar(4, 1);
      }
	}
  }
}
void instructions(){
	lcd.clear();
    lcd.print("Left:"); lcd.write(byte(7));
    lcd.print(" ");
    lcd.print("Middle:"); lcd.write(byte(8));
    lcd.setCursor(0, 1);
    lcd.print("Right: Esc/Ok"); 
}
int flag;

void loop(){ 
  	flag=0;
  	int up=digitalRead(9);
  	int down=digitalRead(8);
  	int act=digitalRead(7);
    if(up && flag){
    	digitalWrite(6, LOW);
  		erasechar(0, 1);
    	lcd.setCursor(0, 0);
    	lcd.write(byte(0));
      	flag=0;
    }
    if(down && !flag){
        digitalWrite(6, HIGH);
      	erasechar(0, 0);
    	lcd.setCursor(0, 1);
    	lcd.write(byte(0));
      	flag=1;
  	}
  	if(act){
      if(!flag){
		play();
      }
      if(flag){
        instructions();
        /*while(1){
			act=digitalRead(7);
          if(act){
            Menu();
			break;
          }
        }*/
      }
    }
  }
