#include <SoftwareSerial.h> // 소프트웨어 시리얼을 활용하기 위한 라이브러리
#include <DFRobot_TFmini.h> // 라이다 센서를 사용하기 위한 라이브러리
#include <Adafruit_TCS34725.h> // 컬러 센서 사용을 위한 라이브러리


#define INTERRUPT_PIN 2


SoftwareSerial mySerial0(10,11); // 라이다 오른쪽 0 // 소프트 시리얼 선언
SoftwareSerial mySerial1(12,13); // 라이다 오른쪽 1
SoftwareSerial mySerial2(50,51); // 라이다 왼쪽 2
SoftwareSerial mySerial3(52,53); // 라이다 왼쪽 3
SoftwareSerial mySerial4(A8,A9); // 라이다 전면 - 아날로그 핀 사용


DFRobot_TFmini TFmini0; // 라이다 라이브러리 불러오기
DFRobot_TFmini TFmini1;
DFRobot_TFmini TFmini2;
DFRobot_TFmini TFmini3;
DFRobot_TFmini TFmini4;


uint16_t distance0,distance1,distance2,distance3,distance4; // 라이다 센서 거리값 16진수 선언


double highR; // 라이다 오른쪽 거리차이 // 실수형 변수 선언
double highL; // 라이다 왼쪽 거리차이
double front; // 라이다 전면 거리


bool wave_finished = true; // 초음파 // 부울 함수 선언
unsigned long _start, _end; //


int motor0 = 4; // 오른쪽 진동모터
int motor1 = 5; // 왼쪽 진동모터
int motor2 = 6; // 상단 진동모터
int buzzer = 7; // 부저
int trig = 3; //초음파 트리거
int distance5; // 초음파 탐지거리


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X); // 객체선언


void measure0() // 우측 0번 라이다 작동
  {  
    TFmini0.measure(); // 라이브러리를 이용하여 라이다 센서 작동
      distance0 = TFmini0.getDistance();

    Serial.print("Distance 0 = "); // 시리얼 모니터에 값을 출력
    Serial.print(distance0);
    Serial.print("cm / ");
  }


void measure1() // 우측 1번 라이다 작동
  {
    TFmini1.measure();
      distance1 = TFmini1.getDistance();

    Serial.print("Distance 1 = ");
    Serial.print(distance1);
    Serial.println("cm"); 
  }


void measure2() // 좌측 2번 라이다 작동
  {  
    TFmini2.measure();
      distance2 = TFmini2.getDistance();

    Serial.print("Distance 2 = ");
    Serial.print(distance2);
    Serial.print("cm / ");
  }


void measure3() // 좌측 3번 라이다 작동
  {
    TFmini3.measure();
      distance3 = TFmini3.getDistance();
      
    Serial.print("Distance 3 = ");
    Serial.print(distance3);
    Serial.println("cm"); 
  }


void measure4() // 정면 4번 라이다 작동
  {     
    TFmini4.measure();
      distance4 = TFmini4.getDistance();

    Serial.print("Distance 4 = ");
    Serial.print(distance4);
    Serial.println("cm");
  }


void Out_Wave() // 초음파 센서 초음파 트리거
  {
  wave_finished = false;

  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  }


void ISR_echo() // 초음파 센서 초음파 수신 유뮤 확인
  {
    switch(digitalRead(INTERRUPT_PIN))
    {
      case HIGH:
      _start = micros();
      break;

      case LOW:
      _end = micros();
      wave_finished = true;
      break;
    }
  }



void setup() {


  Serial.begin(9600); //아두이노 시리얼 통신 시작 // 아두이노 통신속도 9600
  TFmini0.begin(mySerial0); // 라이다 라이브러리 시작 (소프트 시리얼) // 라이브러리와 소프트 시리얼 구속
  TFmini1.begin(mySerial1);
  TFmini2.begin(mySerial2);
  TFmini3.begin(mySerial3);
  TFmini4.begin(mySerial4);


  pinMode(trig,OUTPUT);
  pinMode(INTERRUPT_PIN,INPUT);
  pinMode(motor0, OUTPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(buzzer, OUTPUT);


  attachInterrupt(0,ISR_echo, CHANGE); // 


  Out_Wave();


  if(tcs.begin())
    {
      Serial.println("센서 찾음");
    }
    else
    {
      Serial.println("센서를 못찾음... 연결상태를 확인하세요.");
      while(1);
    }
}



void loop() {
  

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


  if(r >= 500 && g >= 500 && b <= 550)
    {
      analogWrite(motor2,200);
    }
    else
    {
      analogWrite(motor2,0);
    }

  
  if(wave_finished)
    {
      distance5 = (_end - _start) * 0.034 / 2;
    
      Serial.print("distance5 = "); 
      Serial.print(distance5); 
      Serial.println(" cm");
    
      Out_Wave();
    }


  if(distance5 >= 20)
    {
      tone(buzzer,1000,100);
    }


  mySerial0.begin(115200); // 라이다 0번 통신 시작 (라이다 통신속도)

    delay(20); // 0.02초간 대기

    measure0(); // 

  mySerial0.end(); // 라이다 0번 통신 종료



  mySerial1.begin(115200); // 라이다 1번 통신 시작 (라이다 통신속도)

    delay(20); // 0.02초간 대기

    measure1();

  mySerial1.end(); // 라이다 1번 통신 종료


  mySerial2.begin(115200); // 라이다 2번 통신 시작 (라이다 통신속도)

    delay(20); // 0.02초간 대기

    measure2();

  mySerial2.end(); // 라이다 2번 통신 종료


  mySerial3.begin(115200); // 라이다 3번 통신 시작 (라이다 통신속도)

    delay(20); // 0.02초간 대기

    measure3();

  mySerial3.end(); // 라이다 3번 통신 종료


  mySerial4.begin(115200); // 라이다 4번 통신 시작 (라이다 통신속도)

    delay(20); // 0.02초간 대기

    measure4();

  mySerial4.end(); // 라이다 4번 통신 종료


  highR = (0.7071*distance1)-(0.866*distance0); // 0, 1번 라이다 센서 측정값으로 높이차 추출
    Serial.print("highR = "); // 높이차 시리얼 모니터에 출력
    Serial.print(highR); 
    Serial.print(" / ");


  highL = (0.7071*distance3)-(0.866*distance2); // 2, 3번 라이다 센서 측정값으로 높이차 추출
    Serial.print("highL = "); // 높이차 시리얼 모니터에 출력
    Serial.print(highL); 
    Serial.print(" / ");


  front = distance4; // 4번 라이다 센서 측정값
    Serial.print("Front = "); // 센서 측정값 시리얼 모니터에 출력
    Serial.println(distance4);

    
  if(10 <= highR) // 높이차 추출값 조건문
    {
      analogWrite(motor0, 200); // 모터 강도 200으로 작동
    }
    else
    {
      analogWrite(motor0, 0); // 모터 강도 0으로 작동 = 모터 미작동
    }
    

  if(10 <= highL) // 높이차 추출값 조건문
    {
      analogWrite(motor1, 200); // 모터 강도 200으로 작동
    }
    else
    {
      analogWrite(motor1, 0); // 모터 강도 0으로 작동 = 모터 미작동
    }


  if(distance4 < 10) // 전면 센서값 조건문
    {
      analogWrite(motor2, 0); // 모터 강도 0으로 작동 = 모터 미작동
    }
    else
    {
      if(distance4 <= 100)
        {
          tone(buzzer,800,100); // 부저 강도 700으로 0.1초 간격으로 작동
          analogWrite(motor2, 255); // 모터 강도 255으로 작동
        }
        else
        {
          if(distance4 <= 200)
            {
              tone(buzzer,550,100); // 부저 강도 500으로 0.1초 간격으로 작동
              analogWrite(motor2, 180); // 모터 강도 180으로 작동
            }
            else
            {
              if(distance4 <= 300)
                {
                  tone(buzzer,350,100); // 부저 강도 300으로 0.1초 간격으로 작동
                  analogWrite(motor2, 100); // 모터 강도 100으로 작동
                }
                else
                {
                  if(distance4 <= 400)
                    {
                      tone(buzzer,200,100); // 부저 강도 500으로 0.1초 간격으로 작동
                      analogWrite(motor2, 180); // 모터 강도 180으로 작동
                    }
                    else
                    {
                      if(distance4 <= 500)
                        {
                          tone(buzzer,70,100); // 부저 강도 300으로 0.1초 간격으로 작동
                          analogWrite(motor2, 100); // 모터 강도 100으로 작동
                        }
                    }
                }
            }
        }
    }
}