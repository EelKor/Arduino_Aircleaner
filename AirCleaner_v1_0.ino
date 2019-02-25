/*******************************************************
 * 작성자 : 이승신
 * 작성일 : 2019-02-25
 * 버  전 : v1.0  (최종수정일 2019-02-25)
 * 목  적 : DIY공기청정기 작동코드
 * M C U : ATmega328p Arduino NANO
 * 센  서 : GP2Y1010AU0F 미세먼지 센서
 * 릴레이 : 5V 1ch 릴레이 
 * ******************* 개    요 *************************
 * A0  = 미세먼지 센서 출력
 * D12 = 미세먼지 센서 측정 LED 제어 
 * D2  = Blue LED 제어
 * D3  = Green LED 제어
 * D4  = Yellow LED 제어
 * D5  = Red LED 제어
 * D6  = AC모터 제어용 릴레어 제어
 * 
 *******************************************************/


/***********함 수 선 언 부************/
int Dust_density();
int init_density();
int error_LEDstatus();


/***********미 세 먼 지 센 서 **********/
int dustPin = A0;
int ledPower = 12;


/*** 미 세 먼 지 L E D 상 태 표 시 등 ***/
int ledBlue = 2;
int ledGreen = 3;
int ledYellow = 4;
int ledRed = 5;


/******* 릴 레 이 A C 모 터 제 어 *******/
int fan = 6;


/***** 평 균 미 세 먼 지 량 [ug/m3] *****/
int dustAvg = 0;


/*==================================================================================================================================================*/

void setup() 
{

//미세먼지 센서 초기화//
pinMode(ledPower,OUTPUT);
pinMode(dustPin, INPUT);

//LED 상태표시등 초기화
pinMode(ledBlue,OUTPUT);
pinMode(ledGreen,OUTPUT);
pinMode(ledYellow,OUTPUT);
pinMode(ledRed,OUTPUT);
pinMode(fan,OUTPUT);

//fan작동
digitalWrite(fan,HIGH);

//초기먼지데이터 
 dustAvg = init_density();
}

/*========================================================================================================================================================*/

void loop() 
{
  
  if(dustAvg <= 20.0)
  {
    digitalWrite(ledBlue,HIGH);
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledYellow,LOW);
    digitalWrite(ledRed,LOW);
   
    digitalWrite(fan , LOW);
  }

  else if(dustAvg > 20.0 && dustAvg <= 50.0)
  {
    digitalWrite(ledBlue,HIGH);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledYellow,LOW);
    digitalWrite(ledRed,LOW);
    digitalWrite(fan , HIGH);
    
  }

  else if(dustAvg > 50.0 && dustAvg <= 80.0)
  {
    digitalWrite(ledBlue,HIGH);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledYellow,HIGH);
    digitalWrite(ledRed,LOW);
    digitalWrite(fan , HIGH);
  }

  else if(dustAvg > 80.0)
  {
     digitalWrite(ledBlue,HIGH);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledYellow,HIGH);
    digitalWrite(ledRed,HIGH);
    digitalWrite(fan , HIGH);  
  }
  
  else
  {
    error_LEDstatus();   
  }
  
  dustAvg = Dust_density();
  
}

/*================================================================================================================================================================*/

/******미세먼지 측정, 1분평균값 리턴 함수******/

int Dust_density()
{
  float dustVal = 0;          //A0핀에서 나오는 출력값
  float dustDensity = 0;      //먼지값 ug/m3
  float calcVoltage = 0;      //dustVal를 Voltage로 환산
  int delayTime = 280;        
  int delayTime2 = 40;
  float offTime = 9600;
  float total = 0;

  int i = 0;

  while(i<60)
  {
    digitalWrite(ledPower,LOW);
    delayMicroseconds(delayTime);
    dustVal = analogRead(dustPin);
    delayMicroseconds(delayTime);
    digitalWrite(ledPower,HIGH);
    delayMicroseconds(offTime);
  
    calcVoltage = dustVal * 5.0 / 1024.0;
    dustDensity = (calcVoltage - 0.5)/0.005;
    total += dustDensity;
    
    i++;
    delay(1000);
    
  }

  
  return total/60;
}

/*===============================================================================================================================================*/

/* setup()에서 실행할 초기 미세먼지 측정 함수 *****
 * LED 시각효과 기능포함 ************************/

int init_density()
{
float dustVal = 0;          //A0핀에서 나오는 출력값
  float dustDensity = 0;      //먼지값 ug/m3
  float calcVoltage = 0;      //dustVal를 Voltage로 환산
  int delayTime = 280;        
  int delayTime2 = 40;
  float offTime = 9600;
  float total = 0;

  int i = 0;

  while(i<60)
  {
    digitalWrite(ledPower,LOW);
    delayMicroseconds(delayTime);
    dustVal = analogRead(dustPin);
    delayMicroseconds(delayTime);
    digitalWrite(ledPower,HIGH);
    delayMicroseconds(offTime);
  
    calcVoltage = dustVal * 5.0 / 1024.0;
    dustDensity = (calcVoltage - 0.5)/0.005;
    total += dustDensity;

    i++;
    
/******여기서 부터 LED 시각효과*******/
    digitalWrite(ledBlue,HIGH);
    delay(124);
    digitalWrite(ledGreen,HIGH);
    delay(124);
    digitalWrite(ledYellow,HIGH);
    delay(124);
    digitalWrite(ledRed,HIGH);
    delay(124);
    digitalWrite(ledRed,LOW);
    delay(124);
    digitalWrite(ledYellow,LOW);
    delay(124);
    digitalWrite(ledGreen,LOW);
    delay(124);
    digitalWrite(ledBlue,LOW);
    delay(124);
/************시각효과 끝************/
   
  
    
  }
  
  return total/60;
}


/*==================================================================================================================================*/

/* Error 발생시 LED 지시등*/

int error_LEDstatus()
{
  for(int i=0; i<20; i++)
  {
     digitalWrite(ledBlue,HIGH);
     digitalWrite(ledGreen,HIGH);
     digitalWrite(ledYellow,HIGH);
     digitalWrite(ledRed,HIGH);
     delay(250);
     digitalWrite(ledBlue,LOW);
     digitalWrite(ledGreen,LOW);
     digitalWrite(ledYellow,LOW);
     digitalWrite(ledRed,LOW);
     delay(250);

     return 0;
  }



  
}
