# Arduino-serialMotor-project
> control motors by connecting three Arduino via serial communication. etc ) DTH11, 1602clcd

기능
---------------
1. 아두이노 3개는 tx-rx핀으로 서로 연결되어 있다
2. 키패드 혹은 시리얼 통신으로 3개의 모터를 동일한 PWM으로 제어할 수 있다 (형식은 (PWM(TH),값))
3. 온습도 센서와 모터의 PWM값을 실시간으로 1602LCD에 띄울 수 있다

구성
---------------
1. Arduino Uno R3 3개
2. 5V DC-motor 3개
3. 1602CLCD using HD44780 LCD Driver 1개 (https://www.sparkfun.com/datasheets/LCD/HD44780.pdf)
4. DTH11 1개

![blockDiagram](./blockDiagram.JPG)
![keypadLayout](./keypadLayout.JPG)

구현영상
---------------
(https://youtu.be/HF257k9DpXg) 
