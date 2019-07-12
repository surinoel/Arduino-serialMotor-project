int exFlag = 0;
int pinPos = 4, pinNeg = 2, pinEn = 3;
int tmpSpeed, pwmSpeed = 100;
int oldkey, key;
const int sw_row[4] = {13, 12, 11, 10};
const int sw_col[4] = {9, 8, 7, 6};
const int sw_gnd = 5;
char buf[30], pwmSpeedBuf[30], cTemp;
String sCommand = "", sCommandTmp, sTmp;
int ret;

int getPWMSpeed(const int);
void incsMtSpeed(void);
void descMtSpeed(void);
void serialWriteCall(int);

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) {       // 키패드 입출력 설정
    pinMode(sw_row[i], OUTPUT);
    pinMode(sw_col[i], INPUT);
  }
  pinMode(sw_gnd, OUTPUT);
  digitalWrite(sw_gnd, LOW);

  pinMode(pinPos, OUTPUT); pinMode(pinNeg, OUTPUT); pinMode(pinEn, OUTPUT); // PWM 핀 입출력 설정
  digitalWrite(pinPos, HIGH);
  digitalWrite(pinNeg, LOW);
  analogWrite(pinEn, pwmSpeed);

  memset(buf, 0, sizeof(buf));        // 변수 초기화
  oldkey = -1;
}

void loop() {
  key = -1;   // key는 처음에 -1
  for (int i = 0; i < 4; i++) {
    digitalWrite(sw_row[i], HIGH);
    for (int j = 0; j < 4; j++) {
      if ((i == 2 && j == 3) || (i == 3 && j == 0) || (i == 3) && (j == 2)) continue;
      if (oldkey == -1 && digitalRead(sw_col[j])) {  // 입력의 변화가 생길 때만
        delay(200);   // delay   
        key = i * 4 + j;
        break;
      }
    }
    digitalWrite(sw_row[i], LOW); 
  }
  if(key != -1) {
    tmpSpeed = getPWMSpeed(key); // 실제 입력된 숫자로 변환
    if(tmpSpeed != -1) {  // 숫자판 혹은 '='을 눌렀다면
      if(exFlag)  // '='을 입력했을 때
      { 
        if(pwmSpeed >= 0 && pwmSpeed <= 255) {
          analogWrite(pinEn, pwmSpeed);
          serialWriteCall(pwmSpeed);  // DEV2번으로 값 전달
        }
        pwmSpeed = 0;
        exFlag = 0;
      }
      else {  // 누적
        pwmSpeed = pwmSpeed * 10 + tmpSpeed;    
      }    
    }
    Serial.println(tmpSpeed);  
  }
  oldkey = key; // key 전환
  while(Serial.available()) // 시리얼 입력을 받을 때
  {
     cTemp = Serial.read();
     if(cTemp == '\n') {  // 개행까지 받는다
        sCommandTmp = sCommand;
        ret = sCommandTmp.indexOf(',');
        if(ret != -1) {
          sTmp = sCommandTmp.substring(0, ret);
          sCommandTmp = sCommandTmp.substring(ret+1, sCommand.length());
          if(sTmp == "PWM") {
              ret = sCommandTmp.indexOf(',');
              sTmp =sCommandTmp.substring(0, ret);
              sTmp.toCharArray(pwmSpeedBuf, 30);
              pwmSpeed = atoi(pwmSpeedBuf);                        
              analogWrite(pinEn, pwmSpeed);
          }
        }
        sCommand += '\n'; 
        sCommand.toCharArray(buf, 30);
        Serial.write(buf);
        memset(buf, 0, sizeof(buf));
        sCommand = "";
        break;     
     }
     sCommand += cTemp;
  }
}

void incsMtSpeed() { // +를 눌렀을 때
  pwmSpeed = (pwmSpeed + 10 > 255) ? 0 : pwmSpeed += 10;
  analogWrite(pinEn, pwmSpeed);
  serialWriteCall(pwmSpeed);
}

void descMtSpeed() { // -를 눌렀을 때
  pwmSpeed = (pwmSpeed - 10 <= 0) ? 0 : pwmSpeed -= 10;
  analogWrite(pinEn, pwmSpeed);
  serialWriteCall(pwmSpeed);
}

int getPWMSpeed(const int keyValue) {
  switch(keyValue) {
    case 3: incsMtSpeed(); return -1;
    case 7: descMtSpeed(); return -1;
    case 0: return 7;
    case 1: return 8;
    case 2: return 9;
    case 4: return 4;
    case 5: return 5;
    case 6: return 6;
    case 8: return 1;
    case 9: return 2;
    case 10: return 3;
    case 13: return 0;
    case 15: exFlag = 1;
  }
}

void serialWriteCall(int pwmSpeed)
{
  String msg = "PWM,"; 
  msg += pwmSpeed; 
  msg += "\n";
  msg.toCharArray(buf, 30);
  Serial.write(buf);
  memset(buf, 0, sizeof(buf));
}
