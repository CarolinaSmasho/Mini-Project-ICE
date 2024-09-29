#include <SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);
int bt[] = {5,4,3,2};
int btst[4];
int l_btst[4];
int PLV1 = 0;
int PLV2 = 1;
int PLV3 = 2;
int PLV4 = 3;
int PLV5 = 4;
int MLV1 = 5;
int MLV2 = 6;
int MLV3 = 7;
int MLV4 = 8;
int MLV5 = 9;
int DLV1 = 10;
int DLV2 = 11;
int DLV3 = 12;
int DLV4 = 13;
int DLV5 = 14;
int TLV1 = 15;
int TLV2 = 16;
int TLV3 = 17;
int TLV4 = 18;
int TLV5 = 19;
int rdnum1 = 0;
int rdnum2 = 0;
int ans[4];
char text;
String tyans = "";
int linput;
int input;
int yans;
int lpoint = 0;
int score = 0;
int check = 0;
bool Oinput[] = {0,0,0,0};
unsigned long ltime = 0;
bool answering = false;
bool start = false;
bool dieqp = true;
bool rd = false;
bool complete = false;
struct Level{
  unsigned long operation;
  unsigned long time;
  unsigned long LV;
  unsigned long ssmtime;
  unsigned long firstnum;
  unsigned long lastnum;
  unsigned long nextLV[4];
};
typedef const struct Level Ltype;
  Ltype FSM[20] = {
  // +
  {0,7000,1,7,1,10,{PLV2,MLV1,DLV1,TLV1}},
  {0,7000,2,7,10,50,{PLV3,MLV2,DLV2,TLV2}},
  {0,5000,3,5,50,100,{PLV4,MLV3,DLV3,MLV3}},
  {0,5000,4,5,100,500,{PLV5,MLV4,DLV4,TLV4}},
  {0,5000,5,5,500,1000,{PLV5,MLV5,DLV5,TLV5}},
  // -
  {1,7000,1,7,1,10,{PLV1,MLV2,DLV1,TLV1}},
  {1,7000,2,7,10,50,{PLV2,MLV3,DLV2,TLV2}},
  {1,5000,3,5,50,100,{PLV3,MLV4,DLV3,TLV3}},
  {1,5000,4,5,100,500,{PLV4,MLV5,DLV4,TLV4}},
  {1,5000,5,5,500,1000,{PLV5,MLV5,DLV5,TLV5}},
  // หาร
  {2,7000,1,7,1,10,{PLV1,MLV1,DLV2,TLV1}}, // DLV1
  {2,7000,2,7,1,50,{PLV2,MLV2,DLV3,TLV2}}, // DLV2
  {2,5000,3,5,1,100,{PLV3,MLV3,DLV4,TLV3}}, // DLV3
  {2,5000,4,5,1,500,{PLV4,MLV4,DLV5,TLV4}},  // DLV4
  {2,5000,5,5,1,1000,{PLV5,MLV5,DLV5,TLV5}}, // DLV5
  // x
  {3,9000,1,9,2,10,{PLV1,MLV1,DLV1,TLV2}}, // TLV1
  {3,9000,2,9,2,20,{PLV2,MLV2,DLV2,TLV3}}, // TLV2
  {3,7000,3,7,2,30,{PLV3,MLV3,DLV3,TLV4}}, // TLV3
  {3,7000,4,7,2,100,{PLV4,MLV4,DLV4,TLV5}}, // TLV4
  {3,7000,5,7,2,1000,{PLV5,MLV5,DLV5,TLV5}} // TLV5
};
int ST = 0;
void setup()
{
  mySerial.begin(4800);
  Serial.begin(4800);
  for(int i=0;i<4;i++){
    pinMode(bt[i],INPUT_PULLUP);
  }

}

void loop()
{
  //toggle button
  for(int i=0;i<4;i++){
    btst[i] = !digitalRead(bt[i]);
    if(btst[i] != l_btst[i]){
      delay(50);
      if(btst[i] == 1){
        Oinput[i] = !Oinput[i];A
      }
      l_btst[i] = btst[i];
    }
  }
  //input-start
  int nST = Oinput[0]*0+Oinput[1]*5+Oinput[2]*15+Oinput[3]*10;
  linput = Oinput[0]+Oinput[1]*2+Oinput[2]*4+Oinput[3]*3;
  if(linput>0){
    mySerial.println("Start!!!");
    // mySerial.println(linput);
    ltime = millis();
    ST = nST;
    start = true;
    rd = true;
  }

  //start
  while(start){
    //input read
    for(int i=0;i<4;i++){
      btst[i] = !digitalRead(bt[i]);
      if(btst[i] != l_btst[i]){
        delay(50);
        if(btst[i] == 1){
          for(int j=0;j<4;j++){
            if(i==j){
              Oinput[j] = 1; 
            }else{
              Oinput[j] = 0;
            }
          }
        }
      l_btst[i] = btst[i];
      }
    }
    //change input
    input = Oinput[0]+Oinput[1]*2+Oinput[2]*4+Oinput[3]*3;
    //randomnum
    randomSeed(analogRead(A0));
    while(rd){
      while(dieqp){
        rdnum1 = random(FSM[ST].firstnum,FSM[ST].lastnum);
        rdnum2 = random(FSM[ST].firstnum,FSM[ST].lastnum);
        //ans in each operation
        ans[0] = rdnum1+rdnum2;
        ans[1] = rdnum1-rdnum2;
        ans[2] = rdnum1/rdnum2;
        ans[3] = rdnum1*rdnum2;
        //operation "/"
        if(FSM[ST].operation == 2){
          //กันคำตอบติดทศนิยม && ไม่ให้เลขเดียวกันหารกันเดี๋ยวง่ายเกิน && ไม่ให้หารแล้วคำตอบน้อยกว่า1 && ไม่ให้หารแล้วได้ตัวมันเอง
          if(rdnum1%rdnum2 == 0 && rdnum1 != rdnum2 && rdnum1 > rdnum2 && rdnum2 != 1){
            rd = false;
            dieqp = false;
          }
        }
        //operation "-"
        else if(FSM[ST].operation == 1){
          //ไม่ให้เลขตัวเดียวกันลบกันเดี๋ยวง่ายเกิน
          if(rdnum1 != rdnum2){
            rd = false;
            dieqp = false;
          }
        }
        //operation "x"
        else if(FSM[ST].operation == 3){
          //ไม่ให้เลขตัวแรกเกินหลักหน่วยเดี๋ยวยากไป
          if(rdnum1 < 10){
            if(FSM[ST].LV==3){
              //level3 ให้เลขตัวหลังมากกว่า10
              if(rdnum2 > 10){
                rd = false;
                dieqp = false;
              }
            }
            else if(FSM[ST].LV==4){
              //level4 ให้เลขตัวหลังมากกว่า50
              if(rdnum2 > 50){
                rd = false;
                dieqp = false;
              }
            }
            else if(FSM[ST].LV==5){
              //level5 ให้เลขตัวหลังมีค่าเกิน50
              if(rdnum2 > 100){
                rd = false;
                dieqp = false;
              }
            }
            else{
              rd = false;
              dieqp = false;
            }
          }
        }
        //non of above
        else{
          rd = false;
          dieqp = false;
        }
      }
      //print question
      mySerial.print(rdnum1);
      if(FSM[ST].operation == 0){
        mySerial.print(" + ");
      }
      if(FSM[ST].operation == 1){
        mySerial.print(" - ");
      }
      if(FSM[ST].operation == 2){
        mySerial.print(" / ");
      }
      if(FSM[ST].operation == 3){
        mySerial.print(" x ");
      }
      mySerial.print(rdnum2);
      mySerial.println(" = ?");
      mySerial.println(ans[FSM[ST].operation]);
      if(FSM[ST].time == 9000){
        mySerial.println("Duration: A");
      }
      else if(FSM[ST].time == 7000){
        mySerial.println("Duration: B");
      }
      else if(FSM[ST].time == 5000){
        mySerial.println("Duration: C");
      }
    }
    //read ans
    while(Serial.available()){
      //read
      text = Serial.read();
      if(text == '\n'){
        complete = true;
      }else{
        tyans += text;
      }
      //convert str to int
      if(complete){
        answering = true;
        yans = tyans.toInt();
        mySerial.println(tyans);
        tyans = "";
        complete = false;
      }
    }
    //if ans correct
    if(yans == ans[FSM[ST].operation] && answering == true){
      //if input changed
      if(input != linput){
        ST = FSM[ST].nextLV[input-1];
        mySerial.println("-----------");
        mySerial.print("Switch to");
        if(FSM[ST].operation == 0){
          mySerial.print(" + ");
        }
        if(FSM[ST].operation == 1){
          mySerial.print(" - ");
        }
        if(FSM[ST].operation == 2){
          mySerial.print(" / ");
        }
        if(FSM[ST].operation == 3){
        mySerial.print(" x ");
        }
        mySerial.print("Level : ");
        mySerial.println(FSM[ST].LV);
        mySerial.println("-----------");
        linput = input;
      }
      //correct ans!!!
      answering = false;
      ltime = millis();
      mySerial.println("Correct!!");
      yans = 0;
      score ++;
      lpoint ++;
      rd = true;
      dieqp = true;
    }
    //level intcrease
    if(lpoint == 5){
      ST = FSM[ST].nextLV[input-1];
      mySerial.println("----------");
      mySerial.print("Next Level : Level ");
      mySerial.println(FSM[ST].LV);
      mySerial.println("----------");
      lpoint = 0;
    }

    //check time's up
    if(millis()-ltime >= FSM[ST].time){
      mySerial.println("time's up!!");
      check = 1;
    }
    //check if ans wrong
    if(yans != ans[FSM[ST].operation] && answering == true){
      mySerial.println("Game Over!!");
      check = 1;
    }
    //end game
    if(check != 0){
      mySerial.print("The Answer is ");
      mySerial.println(ans[FSM[ST].operation]);
      mySerial.print("Score : ");
      mySerial.println(score);
      lpoint = 0;
      score = 0;
      start = false;
      rd = true;
      dieqp = true;
      for(int i=0;i<4;i++){
        Oinput[i] = 0;
      }
      check = 0;
      answering = false;
    }
  }
}
