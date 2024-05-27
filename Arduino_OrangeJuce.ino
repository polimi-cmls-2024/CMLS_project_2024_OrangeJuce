
byte knob[]={0, 1 , 2, 3, 4, 5, 6, 7};
byte led[] = {3, 5, 7};
int last_value[] = {0,0,0,0,0,0,0,0}, value, print, choice = 1;

void setup(){
  Serial.begin(9600);
  for (int x=0; x<3; x++)
    pinMode(led[x], OUTPUT);
  digitalWrite(led[0],1);
  for (int x=0; x<8; x++){
    value = analogRead(knob[x])/10;
    last_value[x]=value;
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  for (int x=0; x<8; x++){
    value = analogRead(knob[x])/10;
    if (x == 7){
      if (value > last_value[x]+1 || value < last_value[x]){ 
        last_value[x]=value;
        value = 100 - value;
        if (value <= 33 && choice != 1){
          choice = 1;
          digitalWrite(led[0],1);
          digitalWrite(led[1],0);
          digitalWrite(led[2],0);
          Serial.print(8500);
          Serial.print('a');
        }
        if (value >= 66 && choice != 3){
          choice = 3;
          digitalWrite(led[0],0);
          digitalWrite(led[1],0);
          digitalWrite(led[2],1);
          Serial.print(9500);
          Serial.print('a');
        }
        if (value < 66 && value>33 && choice != 2){
          choice = 2;
          digitalWrite(led[0],0);
          digitalWrite(led[1],1);
          digitalWrite(led[2],0);
          Serial.print(9000);
          Serial.print('a');
        }
      }
    } else {
      if (value > last_value[x]+1 || value < last_value[x]){ 
        last_value[x]=value;
        if (value < 0)
          print = 0;
        else if (value > 97)
          print = 100;
        else print = value;
        Serial.print(((x+1)*1000)+(100-print));
        Serial.print('a');
      }
    }
    
      
  }

}
