#include <SoftwareSerial.h>           // 소프트웨어 시리얼 라이브러리
#include <DFRobot_TFmini.h>           // 라이다 센서 라이브러리
#include <Servo.h>                    // 서보 모터 라이브러리  
#include <Adafruit_TCS34725.h>        // 컬러 센서 사용을 위한 라이브러리8



SoftwareSerial SerialL45(10,11);          // 라이다 왼쪽 45 // Serial Left 45
SoftwareSerial SerialL30(12,13);          // 라이다 왼쪽 30 // Serial Left 30
SoftwareSerial SerialR45(50,51);          // 라이다 오른쪽 45 // Serial Right 45
SoftwareSerial SerialR30(52,53);          // 라이다 오른쪽 30 // Serial Right 30
SoftwareSerial SerialF(A8,A9);            // 라이다 전면 // Serial Front



DFRobot_TFmini TFminiL45;            // Left 45
DFRobot_TFmini TFminiL30;            // Left 30
DFRobot_TFmini TFminiR45;            // Right 45
DFRobot_TFmini TFminiR30;            // Right 30
DFRobot_TFmini TFminiF;              // Front


Servo servo; 


uint16_t distF;                      // Front Distance
uint16_t distL45,distL30;            // Left Distance
uint16_t distR45,distR30;            // Right Distance
uint16_t dist;                       // 거리 상수, 거리값
uint16_t a[20];                      // 길이가 10인 a배열 - 오른쪽
uint16_t b[20];                      // 길이가 10인 b배열 - 왼쪽


int servoPin = 8;                    // 서보 모터 통신 핀
int angle = 0;                       // 서보 모터 각도 
int but01 = 9;                       // 탐지 버튼
int butSignal;                       // 버튼 신호 // 버튼이 눌렸을때 HIGH값 출력
int motorL = 4;                      // 왼쪽 진동모터
int motorR = 5;                      // 오른쪽 진동모터
int motorT = 6;                      // 상단 진동모터
int buzzer = 7;                      // 부저
int trig = 3;                        // 초음파 트리거
int echo = 2;
int distU;                           // 초음파 탐지거리 // Ultra sound Distance
int x;                               // 배열 변수
int standL2, standL1;                // 왼쪽 2의 갯수, 1의 갯수
int standR2, standR1;                // 오른쪽 2의 갯수, 1의 갯수
int sum, sum01, sum02;               // 거릿값 차이, map()함수 변형값
int highR, highL;                    // 라이다 오른쪽, 왼쪽 거리차이 
float duration;


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); // 객체선언



void setup() 
{ 
  Serial.begin(115200);                  //아두이노 통신속도 
  TFminiF.begin(SerialF);              // 라이다 라이브러리 시작 (소프트 시리얼) // 라이브러리와 소프트 시리얼 구속
  TFminiL45.begin(SerialL45); 
  TFminiL30.begin(SerialL30);  
  TFminiR45.begin(SerialR45);   
  TFminiR30.begin(SerialR30);

  pinMode(but01, INPUT);
  pinMode(trig,OUTPUT); 
  pinMode(echo, INPUT);    
  pinMode(motorR, OUTPUT);  
  pinMode(motorL, OUTPUT);  
  pinMode(motorT, OUTPUT);  
  pinMode(buzzer, OUTPUT);

}





void loop() 
{  
  butSignal = digitalRead(but01);


  if(butSignal == HIGH)               // HIGH값은 버튼이 눌렸을 때
  {
    Serial.println("Press down");
    
    servo.attach(servoPin);           // 서보 조작 선언

    delay(40);



    x = 0;

    dist = 0;

    angle = 0;
    

  
    for(angle = 0; angle < 180; angle = angle + 3) 
    {
      servo.write(angle); 

      if(angle <= 60)
      {
        SerialF.begin(115200); 

        delay(40);
        

        TFminiF.measure();
          distF = TFminiF.getDistance();


        SerialF.end();

        delay(40); 



        if(distF == 0)
        {
          distF = 1200;
        }



        sum01 = distF - dist;

        dist = distF;

        sum02 = map(abs(sum01), 0, 1200, 0, 24);

      
        if(sum02 >= 6)
        {
          a[x] = 2;
        }
        else
        {
          if(sum02 >= 1)
          {
            a[x] = 1;            
          }
          else
          {
            a[x] = 0;
          }
        }


        a[0] = 0;
        
        a[20] = 0;


        if(a[x] == 2)
        {
          standR2 = standR2 + 1;
        }
        else
        {
          if(a[x] == 1)
          {
            standR1 = standR1 + 1;            
          }
        }


      
        Serial.print("distF = ");
        Serial.print(distF);
       
        Serial.print(" sum01 = ");
        Serial.print(sum01);
        
        Serial.print(" sum02 = ");
        Serial.print(sum02);
        
        Serial.print(" Array No. ");
        Serial.print(x);
        
        Serial.print(" a[x] = ");
        Serial.println(a[x]);



        x = x + 1;
        
        sum01 = 0;

        sum02 = 0;



        delay(40);
      }
    }



    x = 0;

    dist = 0;


  
    for(angle = 180; angle > 90; angle = angle - 3) 
    {     
      servo.write(angle); 

      if(angle >= 120)
      {
        SerialF.begin(115200); 

        delay(40);


        TFminiF.measure();
          distF = TFminiF.getDistance();

        SerialF.end();

        delay(40); 



        if(distF == 0)
        {
          distF = 1200;
        }

        

        sum01 = distF - dist;

        dist = distF;

        sum02 = map(abs(sum01), 0, 1200, 0, 24);


      
        if(sum02 >= 6)
        {
          b[x] = 2;
        }
        else
        {
          if(sum02 >= 1)
          {
            b[x] = 1;            
          }
          else
          {
            b[x] = 0;
          }
        }

        b[0] = 0;
        
        b[20] = 0;


        if(b[x] == 2)
        {
          standL2 = standL2 + 1;
        }
        else
        {
          if(b[x] == 1)
          {
            standL1 = standL1 + 1;            
          }
        }


      
        Serial.print("distF = ");
        Serial.print(distF);
        
        Serial.print(" sum01 = ");
        Serial.print(sum01);
        
        Serial.print(" sum02 = ");
        Serial.print(sum02);
        
        Serial.print(" Array No. ");
        Serial.print(x);
        
        Serial.print(" b[x] = ");
        Serial.println(b[x]);



        x = x + 1;

        sum01 = 0;

        sum02 = 0;


              
        delay(40);
      }
    }


    delay(800);                      // 서보 모터가 다 돌때까지 딜레이
    
    servo.detach();



    Serial.print(" standR2 = ");
    Serial.print(standR2);
  
    Serial.print(" standR1 = ");
    Serial.println(standR1);
  
    Serial.print(" standL2 = ");
    Serial.print(standL2);
  
    Serial.print(" standL1 = ");
    Serial.println(standL1);



    if(standR2 <= 6)
    {
      Serial.println(" Not Detacted for Right");
    }
    else
    {
      if(standR2 >= 1)
      {
        if(standR1 >= 1)
        {
          tone(buzzer,1500,200);
          digitalWrite(motorR, HIGH);
          delay(300);
          digitalWrite(motorR, LOW);
          noTone(buzzer);
        }
      }
      else
      {
        Serial.println(" Not Detacted for Right ");
      }
    }


    if(standL2 <= 6)
    {
      Serial.println(" Not Detacted for Left ");
    }
    else
    {
      if(standL2 >= 1)
      {
        if(standL1 >= 1)
        {
          tone(buzzer,900,200);
          digitalWrite(motorL, HIGH);
          delay(300);
          digitalWrite(motorL, LOW);
          noTone(buzzer);
        }
      }
      else
      {
        Serial.println(" Not Detacted for Left ");
      }
    }

    
  }
  else
  {
    Serial.println("Do not press");


    uint16_t clear, red, green, blue;


    tcs.getRawData(&red, &green, &blue, &clear);


    int r = map(red, 0, 21504, 0, 1025);
    int g = map(green, 0, 21504, 0, 1025);
    int b = map(blue, 0 ,21504, 0 ,1025);


    Serial.print("R : "); 
    Serial.print(r); 
    Serial.print(" / ");
  
    Serial.print("G : "); 
    Serial.print(g); 
    Serial.print(" / ");
  
    Serial.print("B : "); 
    Serial.println(b);


    if(r >= 500 && g >= 500 && b <= 400)
    {
      digitalWrite(motorT,HIGH);
      delay(100);
      digitalWrite(motorT, LOW);
    }




    digitalWrite(trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(trig,LOW);

    duration = pulseIn(echo, HIGH);

    distU = ((340 * duration) / 10000) / 2;
    
    Serial.print("dist Ultra Sonic = "); 
    Serial.print(distU); 
    Serial.println(" cm");

  


    if(distU >= 30)
    {
      tone(buzzer,1000,100);
    }
    else
    {
      noTone(buzzer);
    }




    SerialL45.begin(115200);             // 라이다 통신 시작 (라이다 통신속도)

    delay(20);   


  TFminiL45.measure();                // 라이브러리를 이용하여 라이다 센서 작동
  
    distL45 = TFminiL45.getDistance();


  Serial.print("Dist L45 = ");
  Serial.print(distL45);
  Serial.println("cm");  
  

    SerialL45.end();                     // 라이다 통신 종료

    delay(20);



    SerialL30.begin(115200);             // 라이다 통신 시작 (라이다 통신속도)

    delay(20);   


  TFminiL30.measure();                // 라이브러리를 이용하여 라이다 센서 작동
  
    distL30 = TFminiL30.getDistance();


  Serial.print("Dist L30 = ");
  Serial.print(distL30);
  Serial.println("cm");  

    SerialL30.end();                     // 라이다 통신 종료

    delay(20);



    SerialR45.begin(115200);             // 라이다 통신 시작 (라이다 통신속도)

    delay(20);   


   TFminiR45.measure();                // 라이브러리를 이용하여 라이다 센서 작동
  
    distR45 = TFminiR45.getDistance();


  Serial.print("Dist R45 = ");
  Serial.print(distR45);
  Serial.println("cm");  

    SerialR45.end();                     // 라이다 통신 종료

    delay(20);



    SerialR30.begin(115200);             // 라이다 통신 시작 (라이다 통신속도)

    delay(20);   


  TFminiR30.measure();                // 라이브러리를 이용하여 라이다 센서 작동
  
    distR30 = TFminiR30.getDistance();


  Serial.print("Dist R30 = ");
  Serial.print(distR30);
  Serial.println("cm");  

    SerialR30.end();                     // 라이다 통신 종료

    delay(20);



    SerialF.begin(115200);             // 라이다 통신 시작 (라이다 통신속도)

    delay(20);   


  TFminiF.measure();                // 라이브러리를 이용하여 라이다 센서 작동
  
    distF = TFminiF.getDistance();


  Serial.print("Dist F = ");
  Serial.print(distF);
  Serial.println("cm");  

    SerialF.end();                     // 라이다 통신 종료

    delay(20);

  


    highR = (0.866*distR45 - 30) - (0.7071*distR30);       // 0, 1번 라이다 센서 측정값으로 높이차 추출
      Serial.print("highR = ");                       // 높이차 시리얼 모니터에 출력
      Serial.print(highR); 
      Serial.print(" / ");


    highL = (0.866*distL45 - 30) - (0.7071*distL30);       // 2, 3번 라이다 센서 측정값으로 높이차 추출
      Serial.print("highL = ");                       // 높이차 시리얼 모니터에 출력
      Serial.print(highL); 
      Serial.println(" / ");



    
    if(10 <= highR)                                   // 높이차 추출값 조건문
    {
      digitalWrite(motorR, HIGH);                     // 모터 강도 200으로 작동
    }

    

    if(10 <= highL)                                   // 높이차 추출값 조건문
    {
      digitalWrite(motorL, HIGH);                     // 모터 강도 200으로 작동
    }



    if(distF < 10) // 전면 센서값 조건문
    {
      analogWrite(motorT, LOW); // 모터 강도 0으로 작동 = 모터 미작동
      noTone(buzzer);
    }
    else
    {
      if(distF <= 150)
        {
          tone(buzzer,800,100); // 부저 강도 700으로 0.1초 간격으로 작동
          analogWrite(motorT, 255); // 모터 강도 255으로 작동
        }
        else
        {
          if(distF <= 200)
            {
              tone(buzzer,550,100); // 부저 강도 500으로 0.1초 간격으로 작동
              analogWrite(motorT, 205); // 모터 강도 180으로 작동
            }
            else
            {
              if(distF <= 250)
                {
                  tone(buzzer,350,100); // 부저 강도 300으로 0.1초 간격으로 작동
                  analogWrite(motorT, 155); // 모터 강도 100으로 작동
                }
                else
                {
                  if(distF <= 300)
                    {
                      tone(buzzer,200,100); // 부저 강도 500으로 0.1초 간격으로 작동
                      analogWrite(motorT, 105); // 모터 강도 180으로 작동
                    }
                    else
                    {
                      if(distF <= 350)
                        {
                          tone(buzzer,70,100); // 부저 강도 300으로 0.1초 간격으로 작동
                          analogWrite(motorT, 70); // 모터 강도 100으로 작동
                        }
                    }
                }
            }
        }
    }   
  }


  Serial.print(" motorL = ");
  Serial.print(digitalRead(motorL));
  
  Serial.print(" motorR = ");
  Serial.print(digitalRead(motorR));
  
  Serial.print(" motorT = ");
  Serial.print(digitalRead(motorT));
  
  Serial.print(" Buzzer = ");
  Serial.print(digitalRead(buzzer));


  digitalWrite(motorR, LOW);
  digitalWrite(motorL, LOW);
  digitalWrite(motorT, LOW);
//  noTone(buzzer);




  

  
//  delay(1000);
}
