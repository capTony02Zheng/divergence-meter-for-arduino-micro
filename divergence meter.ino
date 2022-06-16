// By capTOZ.
// These are code worked for the divergence meter that published in github.
// For use, the ds3231 module need to be updated, which is not included in these code.
// All you need to update is the translateIR() function, you need to update the case to the corresponding button.
// 用中文就长话短说了。这是给世界线变动率探测仪的代码，ds3231时钟模块的更改时间设定请另寻代码来更新。
// 同时translateIR()这个function需要更新才可以使变动率探测仪正常接收遥控器信号。

#include <SPI.h>
#include "RTClib.h"
#include <Wire.h>
#include "IRremote.h"

#define num1 0x8000
#define num2 0x4000
#define num3 0x2000
#define num4 0x1000
#define num5 0x0800
#define num6 0x0400
#define num7 0x0200
#define num8 0x0100
#define num9 0x0080
#define num0 0x0040
#define decimalpoint 0x0020 
#define no_display 0x0000

#define num1dot 0x8020
#define num2dot 0x4020
#define num3dot 0x2020
#define num4dot 0x1020
#define num5dot 0x0820
#define num6dot 0x0420
#define num7dot 0x0220
#define num8dot 0x0120
#define num9dot 0x00A0
#define num0dot 0x0060

#define latchPin A5
#define SDA 2
#define SCL 3
#define IRdata 7


#define tDelay 1000
RTC_DS3231 rtc;
IRrecv irrecv(IRdata);
decode_results results;
int index_of_tube;
short mode;
float Worldline[65];
String todisplay;
bool decimalbind;
bool randomdisplay;
bool isopen;
bool displayopen;
bool inmenu;
bool onebyone;
short worldlineindex;
char secondforcompare; //mode 1 flag
bool dayflag; //mode1 flag
String tempforcompare; //mode2 flag
int worldlineindexforcompare; //mode3 flag
String mode4flag;


void shutdisplay() {
  spistart();
  for (int i = 0; i < 8; i++) {
      updatetube(chartoint(String(i).charAt(0)));
  }
  spiend();
  delay(tDelay);
  cleardisplay();
}

void opendisplay() {
  cleardisplay();
  delay(tDelay);
  spistart();
  for (int i = 7; i >= 0; i--) {
      updatetube(chartoint(String(i).charAt(0)));
  }
  spiend();
  delay(2000);
  cleardisplay();
  mode = 0;
  index_of_tube = 0;
  todisplay = "";
  decimalbind = false;
  randomdisplay = false;
  inmenu = false;
  secondforcompare = 'x';
  worldlineindex = 45;
  dayflag = false;
  tempforcompare = "";
  worldlineindexforcompare = -1;
  mode4flag = "null";
  isopen = true;
  onebyone = false;
}

void cleardisplay() {
  spistart();
    for (int i = 0; i < 8; i++) {
      updatetube(no_display);
    }
  spiend();
}

void blinkdisplay() {
  spistart();
    for (int i = 0; i < 8; i++) {
      updatetube(decimalpoint);
    }
  spiend();
  delay(1000);
  cleardisplay();
  dayflag = false;
  tempforcompare = "";
  worldlineindexforcompare = -1;
  mode4flag = "null";
  secondforcompare =  'x';
}

void displayoutcome() {
  if (todisplay.length() > 8) {
    spistart();
    if (decimalbind == false) {
      for (int i = todisplay.length() - 1; i > todisplay.length() - 9; i--) {
        updatetube(chartoint(todisplay.charAt(i)));
      }
    } else {
      int index = 0;
      bool havedecimal = false;
      for (int i = todisplay.length() - 1; index < 8; index++) {
        if (!havedecimal) {
          updatetube(chartoint(todisplay.charAt(i)));
        } else {
          i--;
          updatetube(chartointwithdot(todisplay.charAt(i)));
          havedecimal = false;
        }
        //check prev one
        if (i != 0) {
          if (todisplay.charAt(i - 1) == '.') {
            havedecimal = true;
          }
        }
        //end of check
        i--;
      }
    }
    spiend();
    Serial.println(todisplay);
  } else {
    //only without bind mode;
    spistart();
    int count = 0;
    for (int i = todisplay.length() - 1; i >= 0; i--) {
      updatetube(chartoint(todisplay.charAt(i)));
      count++;
    }
    for (int i = count; i < 8; i++) {
      updatetube(no_display);
    }
    spiend();
    Serial.println(todisplay);
  }
}

void translateIR() {
  switch(results.value) {
    case 16753245: Serial.println("Power");
    if (isopen) {
      isopen = !isopen;
      shutdisplay();
    } else {
      isopen = !isopen;
      opendisplay();
    }
    break;
    
    case 16769565: Serial.println("Menu"); 
    // for mode change
    inmenu = !inmenu;
    blinkdisplay();
    break;
    case 16720605: Serial.println("test"); break;
    case 16712445: Serial.println("+"); break;
    case 16761405: Serial.println("return");
    if (mode == 4 && todisplay.length() > 0) {
      todisplay = "";
    }
    break;
    case 16769055: Serial.println("<--");
      if (inmenu) {
        mode--;
        if(mode == -1) {
          mode = 5;
        }
        blinkdisplay();
        break;
      }
      if (mode == 3 && randomdisplay == false) {
        worldlineindex -= 1;
        if(worldlineindex == -1) {
          worldlineindex = 64;
        }
      }
    break;
    case 16754775: Serial.println("play");
      if (mode == 4 && todisplay.length() > 0) {
        displayopen = !displayopen;
      }
      if (mode == 3) {
        randomdisplay = !randomdisplay;
        if (randomdisplay) {
          worldlineindexforcompare = -1;
        }
      }
    break;
    case 16748655: Serial.println("-->");
      if (inmenu) {
        mode++;
        if(mode == 6) {
          mode = 0;
        }
        blinkdisplay();
        break;
      }
      if (mode == 3 && randomdisplay == false) {
        worldlineindex += 1;
        if(worldlineindex == 65) {
          worldlineindex = 0;
        }
      }
    break;
    case 16738455: Serial.println("0");
      if (inmenu) {
        mode = 0;
        inmenu = false;
        blinkdisplay();
        break;
      }
      if (mode == 4) {
        todisplay += "0";
        blinkdisplay();
      }
    break;
    case 16750695: Serial.println("-");
      if (mode == 4) {
        todisplay += ".";
        blinkdisplay();
      }
    break;
    case 16756815: Serial.println("C"); 
    if (mode == 3) {
      onebyone = !onebyone;
      blinkdisplay();
      break;
    }
    if (mode == 4) {
      decimalbind = !decimalbind;
      blinkdisplay();
    }
    break;
    case 16724175: Serial.println("1");
      if (inmenu) {
          mode = 1;
          inmenu = false;
          blinkdisplay();
          break;
        }
      if (mode == 4) {
        todisplay += "1";
        blinkdisplay();
      }
      break;
    case 16718055: Serial.println("2");
      if (inmenu) {
        mode = 2;
        inmenu = false;
        blinkdisplay();
        break;
      }
      if (mode == 4) {
        todisplay += "2";
        blinkdisplay();
      }
      break;
    case 16743045: Serial.println("3");
      if (inmenu) {
        mode = 3;
        inmenu = false;
        blinkdisplay();
        break;
      }
      if (mode == 4) {
        todisplay += "3";
        blinkdisplay();
      }
      break;
    case 16716015: Serial.println("4");
      if (inmenu) {
        mode = 4;
        inmenu = false;
        blinkdisplay();
        break;
      }
      if (mode == 4) {
        todisplay += "4";
        blinkdisplay();
      }
      break;
    case 16726215: Serial.println("5");
      if (inmenu) {
        mode = 5;
        inmenu = false;
        blinkdisplay();
        break;
      }
      if (mode == 4) {
        todisplay += "5";
        blinkdisplay();
      }
      break;
    case 16734885: Serial.println("6");
    if (mode == 4) {
      todisplay += "6";
      blinkdisplay();
    }
    break;
    case 16728765: Serial.println("7");
    if (mode == 4) {
      todisplay += "7";
      blinkdisplay();
    }
    break;
    case 16730805: Serial.println("8");
    if (mode == 4) {
      todisplay += "8";
      blinkdisplay();
    }
    break;
    case 16732845: Serial.println("9");
    if (mode == 4) {
      todisplay += "9";
      blinkdisplay();
    }
    break;

    default: 
    Serial.println(" other button : ");
    Serial.println(results.value);
    break;
  }
}

uint16_t chartoint(char i) {
  switch(i) {
    case '0': return num0;
    case '1': return num1;
    case '2': return num2;
    case '3': return num3;
    case '4': return num4;
    case '5': return num5;
    case '6': return num6;
    case '7': return num7;
    case '8': return num8;
    case '9': return num9;
    case '.': return decimalpoint;
    case 'x': return no_display;
    default: 
    return no_display;
  }
}

uint16_t chartoint(long i) {
  switch(i) {
    case 0: return num0;
    case 1: return num1;
    case 2: return num2;
    case 3: return num3;
    case 4: return num4;
    case 5: return num5;
    case 6: return num6;
    case 7: return num7;
    case 8: return num8;
    case 9: return num9;
    case 10: return decimalpoint;
    case 11: return no_display;
    default: 
    return no_display;
  }
}

uint16_t chartointwithdot(char i) {
  switch(i) {
    case '0': return num0dot;
    case '1': return num1dot;
    case '2': return num2dot;
    case '3': return num3dot;
    case '4': return num4dot;
    case '5': return num5dot;
    case '6': return num6dot;
    case '7': return num7dot;
    case '8': return num8dot;
    case '9': return num9dot;
    case '.': return decimalpoint;
    case 'x': return no_display;
    default: 
    return no_display;
  }
}

void updatetube(uint16_t data) {
   SPI.transfer16(data);
}

void spistart() {
  SPI.begin();
  digitalWrite(latchPin, LOW);
  SPI.beginTransaction(SPISettings(80000, LSBFIRST, SPI_MODE1));
}

void spiend() {
  SPI.endTransaction();
  digitalWrite(latchPin, HIGH);
  SPI.end();
}

void Initilizeworldline() {
  //https://sci-adv.cc/wiki/世界线收束范围一览
  //大部分数字后面加了个三是为了后面代码进位时不出错，当作垃圾代码就得了。
  Worldline[0] = 0.5710240;
  Worldline[1] = 0.5710150;
  Worldline[2] = 0.5232993;
  Worldline[3] = 0.4569033;
  Worldline[4] = 0.4094203;
  Worldline[5] = 0.3371873;
  Worldline[6] = 0.4094313;
  Worldline[7] = 0.4569143;
  Worldline[8] = 0.5233073;
  Worldline[9] = 0.5710463;
  Worldline[10] = 0.5710823;
  Worldline[11] = 0.5710463;
  Worldline[12] = 0.5097363;
  Worldline[13] = 0.4569233;
  Worldline[14] = 0.3373373;
  Worldline[15] = 0.3371993;
  Worldline[16] = 0.3371613;
  Worldline[17] = 0.3284033;
  Worldline[18] = 0.3345813;
  Worldline[19] = 0.0000000;
  Worldline[20] = 1.1304263;
  Worldline[21] = 1.1302053;
  Worldline[22] = 1.1302383;
  Worldline[23] = 1.1298483;
  Worldline[24] = 1.3827333;
  Worldline[25] = 1.1299543;
  Worldline[26] = 1.1436883;
  Worldline[27] = 1.0647503;
  Worldline[28] = 1.0811633;
  Worldline[29] = 1.0536493;
  Worldline[30] = 1.0647563;
  Worldline[31] = 1.0558213;
  Worldline[32] = 1.1298483;
  Worldline[33] = 1.0973023;
  Worldline[34] = 1.1235813;
  Worldline[35] = 1.1298483;
  Worldline[36] = 1.1302123;
  Worldline[37] = 1.1302053;
  Worldline[38] = 1.1302063;
  Worldline[39] = 1.1302073;
  Worldline[40] = 1.1302083;
  Worldline[41] = 1.1302093;
  Worldline[42] = 1.1302103;
  Worldline[43] = 1.1302113;
  Worldline[44] = 2.6150743;
  Worldline[45] = 1.04859613;
  Worldline[46] = 1.04932613;
  Worldline[47] = 1.048728;
  Worldline[48] = 1.048725;
  Worldline[49] = 1.114514;
  Worldline[50] = 3.3860193;
  Worldline[51] = 3.030493;
  Worldline[52] = 3.019430;
  Worldline[53] = 3.667293;
  Worldline[54] = 3.600104;
  Worldline[55] = 3.406288;
  Worldline[56] = 3.372329;
  Worldline[57] = 3.182879;
  Worldline[58] = 3.130238;
  Worldline[59] = 4.456441;
  Worldline[60] = 4.493624;
  Worldline[61] = 4.530805;
  Worldline[62] = 4.4564423;
  Worldline[63] = 4.4936233;
  Worldline[64] = 4.530806;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  pinMode(latchPin, OUTPUT);
  pinMode(IRdata, INPUT);
  //
  if (! rtc.begin()) { // 若果初始化失败，则
    Serial.println("Couldn't find RTC");
    Serial.flush(); 
    abort();  // 程序停止运行
  }
  irrecv.enableIRIn(); // Start the receiver
  blinkdisplay();
  index_of_tube = 0;
  mode = 0;
  decimalbind = false;
  randomdisplay = false;
  worldlineindex = 45;
  isopen = false;
  displayopen = false;
  Initilizeworldline();
  secondforcompare = 'x';
  inmenu = false;
  opendisplay();
  dayflag = false;
  tempforcompare = "";
  worldlineindexforcompare = -1;
  mode4flag = "null";
  onebyone = false;
  delay(tDelay);
}

void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();
  if (irrecv.decode(&results)) {
    translateIR();
    irrecv.resume();
  }
  if (isopen) {
    if (mode == 0) {
      //Serial.print("mode 0");
      //
      //mode 0 display hour minute second
      //
      char h_left;
      char h_right;
      char m_left;
      char m_right;
      char s_left;
      char s_right;
      //set hour
        h_left = String((now.hour() / 10)).charAt(0);
        h_right = String((now.hour() % 10)).charAt(0);
      // set minute
        m_left = String((now.minute() / 10)).charAt(0);
        m_right = String((now.minute() % 10)).charAt(0);
      // set second
        s_left = String((now.second() / 10)).charAt(0);
        s_right = String((now.second() % 10)).charAt(0);
      
      //start transfer
      if (now.hour() == now.minute() && now.second() == now.minute()  && now.hour() % 3 == 0) {
        mode = 5;
      }
      if (secondforcompare != s_right) {
        spistart();
        updatetube(chartoint(s_right));
        updatetube(chartoint(s_left));
        updatetube(chartoint('.'));
        updatetube(chartoint(m_right));
        updatetube(chartoint(m_left));
        updatetube(chartoint('.'));
        updatetube(chartoint(h_right));
        updatetube(chartoint(h_left));
        spiend();
        //time in serial
        Serial.print(h_left);
        Serial.print(h_right);
        Serial.print(':');
        Serial.print(m_left);
        Serial.print(m_right);
        Serial.print(':');
        Serial.print(s_left);
        Serial.println(s_right);
        Serial.print("function time: ");
        Serial.print(now.hour());
        Serial.print('-');
        Serial.print(now.minute());
        Serial.print('-');
        Serial.print(now.second());
        Serial.println();
        secondforcompare = s_right;
      }
    }
    if (mode == 1) {
      //Serial.print("mode 1");
      //
      //mode 1 display year month day
      //
      char y1, y2, y3, y4;
      char mleft, mright, dleft, dright;
         mleft = String((now.month() / 10)).charAt(0);
         mright = String((now.month() % 10)).charAt(0);
         dleft = String((now.day() / 10)).charAt(0);
         dright = String((now.day() % 10)).charAt(0);
         y1 = String((now.year() / 1000)).charAt(0);
         y2 = String((now.year() / 100)).charAt(0);
         y3 = '0';
         y4 = '2';
      if (!dayflag) {
        spistart();
        updatetube(chartoint(dright)); updatetube(chartoint(dleft));
        updatetube(chartointwithdot(mright)); updatetube(chartoint(mleft));
        updatetube(chartointwithdot(y1));
        updatetube(chartoint(y2));
        updatetube(chartoint(y3));
        updatetube(chartoint(y4));
        spiend();
          Serial.print("Month: ");
          Serial.print(mleft);
          Serial.print(mright);
          Serial.println();
          Serial.print("Day: ");
          Serial.print(dleft);
          Serial.print(dright);
          Serial.println();
          Serial.print("Year: ");
          Serial.print(y4);
          Serial.print(y3);
          Serial.print(y2);
          Serial.print(y1);
          Serial.println();
        dayflag = true;
      }
    }
    if (mode == 2) {
      Serial.print("mode 2");
      //
      //mode 2 display temperature
      //
      String temp(rtc.getTemperature());
      if (tempforcompare != temp) {
        spistart();
        for (int i = 4; i >= 0; i--) {
          updatetube(chartoint(temp.charAt(i)));
        }
        updatetube(no_display);
        updatetube(no_display);
        updatetube(no_display);
        spiend();
        tempforcompare = temp;
      }
    } 
    if (mode == 3) {
      Serial.print("mode 3");
      Serial.println();
      //
      //mode 3 display worldline stored
      //
      if (randomdisplay) {
        worldlineindex = random(65);
        spistart();
        for (int i = 5; i >= 0; i--) {
          updatetube(chartoint(random(10)));
        }
        updatetube(chartoint('.'));
        updatetube(chartoint(random(10)));
        spiend();
        delay(30);
      } else if (!onebyone) {
        //display by instant;
        if (worldlineindexforcompare != worldlineindex) {
          String toadd = String(Worldline[worldlineindex]*100000000);
          String currentline;
          if (Worldline[worldlineindex] >= 1.0) {
            currentline += toadd.substring(0,1);
            currentline += ".";
            currentline += toadd.substring(1,7);
          } else if (Worldline[worldlineindex] == 0.0) {
            currentline += "0.000000";
          } else {
            currentline += "0.";
            currentline += toadd.substring(0,6);
          }
          Serial.print("index: ");
          Serial.print(worldlineindex);
          Serial.println();
          Serial.print("currentline : ");
          Serial.print(currentline);
          Serial.println();
          spistart();
          for (int i = 7; i >= 0; i--) {
            updatetube(chartoint(currentline[i]));
          }
          spiend();
          worldlineindexforcompare = worldlineindex;
        }
      } else {
        //display the number from left to right
        if (worldlineindexforcompare != worldlineindex) {
          String toadd = String(Worldline[worldlineindex]*100000000);
          String currentline;
          if (Worldline[worldlineindex] >= 1.0) {
            currentline += toadd.substring(0,1);
            currentline += ".";
            currentline += toadd.substring(1,7);
          } else if (Worldline[worldlineindex] == 0.0) {
            currentline += "0.000000";
          } else {
            currentline += "0.";
            currentline += toadd.substring(0,6);
          }
          Serial.print("index: ");
          Serial.print(worldlineindex);
          Serial.println();
          Serial.print("currentline : ");
          Serial.print(currentline);
          Serial.println();
          
          for (int i = 6; i >= 0; i--) {
            //1.048596
            //01234567
            int numtosave = 7 - i;
            int randomcount = i;
            int r = random(15,50);
            for (int t = 0; t < r; t++) {
              spistart();
              for (int c = 0; c < randomcount; c++) {
                updatetube(chartoint(random(10)));
              }
              for (int rest = numtosave; rest >= 0; rest--) {
                updatetube(chartoint(currentline[rest]));
              }
              spiend();
              delay(30);
            }
          }
          worldlineindexforcompare = worldlineindex;
        }
      }
    }
    if (mode == 4) {
      //
      //mode 4 display input data;
      //
      if (displayopen && todisplay.length() != 0) {
        if (mode4flag != todisplay) {
          displayoutcome();
          Serial.println("mode 4");
          Serial.print("todisplay: ");
          Serial.println(todisplay);
          mode4flag = todisplay;
        }
      } else {
        cleardisplay();
        mode4flag = "null";
      }
    }
    if (mode == 5) { 
    //
    //mode 5 auto anti toxic
    //
    Serial.println("mode 5");
    spistart();
    for (int i = 0; i < 8; i++) {
      String x = String(index_of_tube);
      char input = x.charAt(0);
      updatetube(chartoint(input));
    }
    spiend();
    index_of_tube++;
    if (index_of_tube >= 10) {
      index_of_tube = 0;
      mode = 0;
    }
    delay(tDelay);
    }
  }
}
