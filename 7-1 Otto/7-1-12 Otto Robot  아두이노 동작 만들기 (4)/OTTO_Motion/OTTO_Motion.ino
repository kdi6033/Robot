//----------------------------------------------------------------
//-- OTTO Dance smooth criminal
//-- Released under a GPL licencse
//-- Originally made for Zowi project remake for Otto
//-- Authors:  Javier Isabel:  javier.isabel@bq.com
//--           Juan Gonzalez (obijuan): juan.gonzalez@bq.com
//-----------------------------------------------------------------
// 참조사이트 : https://m.blog.naver.com/PostView.nhn?blogId=otter35&logNo=220998426158&proxyReferer=https:%2F%2Fwww.google.com%2F

#include "motion.h"

void setup()
{
  Serial.begin(115200);
  setupMotor();
}


void loop()
{
  walk(1,500);
  //turnLeft(1,1000);
  //turnRight(1,1000);
}
