// include the library code:
#include <LiquidCrystal.h>;

// initialize the library
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//casas do arduino 
const int buzzer = 10;
const int button = 8; 
int buttonState = 0;

//variaveis auxiliares para o jogo 
int cactoInicio = 0; 
int cactoFinal = 0; 
boolean flagCacto = false; 


//outras variaveis 
int i; 


byte cacto1[8] = {
	0b00100,
	0b00100,
	0b10101,
	0b10101,
	0b11101,
	0b00111,
	0b00100,
	0b00100
};

byte cacto2[8] = {
	0b00000,
	0b00100,
	0b10101,
	0b10101,
	0b10111,
	0b11100,
	0b00100,
	0b00100
};

byte cacto3[8] = {
  	0b00000,
	0b00000,
	0b00000,
	0b00010,
	0b01010,
	0b11011,
	0b11111,
  	0b01010
};

void setup() 
{
	//novo elemento de jogo 
	lcd.createChar(2, cacto1);
  	lcd.createChar(1, cacto2); 

	//colunas e linhas 
	lcd.begin(16, 2);

	//desenhar o elemento na tela
	lcd.write((uint8_t)2);
  	lcd.write((uint8_t)1);
  	
  
  	lcd.setCursor(0,1);
}

void loop()
{
	lcd.clear();
  	start(); 
  	
}

void start(){
  
  cactoInicio = random(4,9);  
  cactoFinal = random(4,9);
  
  //movimento cacto
  for(i = 16; i >=-(cactoInicio + cactoFinal); i--){
    
    //define a nova posição do cacto 
 	lcd.setCursor(i,1); 
    //desenha o cacto 
  	lcd.write(1);    
    //anda uma unidade
  	lcd.setCursor(i+1,1);    
    //para evitar que os cactos saiam grudados 
  	lcd.print(" ");
	
    delay(10);
    
    //nova posição do cacto 
  	lcd.setCursor(i + cactoInicio,2);   
    //desenha o cacto 
  	lcd.write(2);    
    //anda uma unidade
  	lcd.setCursor(i + cactoInicio + 1,2);    
    //para evitar que os cactos saiam grudados
  	lcd.print(" ");
    
	//define a velocidade do movimento dos cactos 
    delay(150);
    
   	lcd.setCursor(i + cactoInicio + cactoFinal,1);
    //desenha o cacto 
  	lcd.write(1);
    //anda uma unidade
  	lcd.setCursor(i + cactoInicio + cactoFinal + 1,1);
    //para evitar que os cactos saiam grudados
  	lcd.print(" ");
    
    delay(10);

 	//quando o cacto chegar ao final da tela
  	if((i+ cactoInicio + cactoFinal)< 0 ){
     //setar o a nova posição do cacto 
   	 i = 16;                    
  	}
    
  }
}














