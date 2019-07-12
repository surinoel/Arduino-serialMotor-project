#include <DHT.h>
#define DHTTYPE DHT11

String sCommand = "", sStart, sTmp;
int pinDht = 2, ret;
char buf[30], pwmSpeedBuf[30], cTemp;
int pinPos = 5, pinNeg = 4, pinEn = 3;
int pwmSpeed = 100;
int receiveFlag = 0;

DHT dht(pinDht, DHTTYPE);

void setup() {
  Serial.begin(115200); 
  pinMode(pinPos, OUTPUT); pinMode(pinNeg, OUTPUT); pinMode(pinEn, OUTPUT); // PWM핀 초기화
  digitalWrite(pinPos, HIGH);
  digitalWrite(pinNeg, LOW); 
  analogWrite(pinEn, pwmSpeed);
  dht.begin();
}

void loop() {
  delay(300);
  float fTemp = dht.readTemperature();
  float fHumi = dht.readHumidity();
  
  if (isnan(fTemp) || isnan(fHumi)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  if(receiveFlag) { // DEV1에서 전달한 PWM 값 분석
    sStart = "", sTmp = "";
    ret = sCommand.indexOf(',');
    if(ret != -1) {
      sTmp = sCommand.substring(0, ret);
      sCommand = sCommand.substring(ret+1, sCommand.length());
      if(sTmp == "PWM") {
          ret = sCommand.indexOf(',');
          sTmp = sCommand.substring(0, ret);
          sTmp.toCharArray(pwmSpeedBuf, 30);
          pwmSpeed = atoi(pwmSpeedBuf);                        
          analogWrite(pinEn, pwmSpeed);
      }
    }
    
    receiveFlag = 0; 
    sCommand = "";
  }
  
  // 300ms 동안 온습도 값을 DEV3에 전달
  String msg1 = "TH,";  
  msg1 += (int)fTemp; 
  msg1 += ",";
  msg1 += (int)fHumi;
  msg1 += "\n";
  msg1.toCharArray(buf, 30);
  Serial.write(buf);
}

void serialEvent(void) 
{
  while(Serial.available()) // DEV1에서 데이터가 왔다면
  {
     cTemp = Serial.read();
     if(cTemp == '\n') {  // 개행까지 받는다
        receiveFlag = 1;
        sCommand.toCharArray(buf, 30);
        Serial.write(buf);
        return;
     }
     sCommand += cTemp;
  }
}
