#include <EEPROM.h>

/*
 Simple 24h clock/watchdog
 No calendar/date
 Up to 10 events
 
 This is simple 24h clock base on watchdog timer. 
 The output is in 24 hour format. 
 It can't be used as a calendar because it is unable to count over 24 hours. 
 If you want a fully-aimed calendar, then move to use RTC module.
 It supports up to 10 events, you control it through UART for both input and output.
 The function serialEvent()'s name might be adjusted depended on Arduino board, some of them only support serialEvent0() while other support serialEvent1().
 You can add more job at '// do sth here' in the function 'loop', feel free to modified it.
 Syntax
     [set_time] [get_time] [set_trigger_] [show_triggers] [help] time
  Options
      help          Display this readme
      set_time      Set the current time, if not, it will count from 00:00
          Example: set_time 15:25
      get_time      Get the current time
          Example: get_time
      set_trigger_  Set events to do, have to be specific the event's location
          Example: set_trigger_0 15:25
      show_triggers View all events
          Example: show_triggers
      save          Save all records to EEPROM
          Example: save      
		  
*/

String setting = "";
unsigned long seconds = 0;
unsigned long triggers[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
unsigned long _millis = millis();
unsigned long local_millis = _millis;
int eeAddress = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect.
  }
  Serial.println( "loading triggers from eeprom ..." );
  EEPROM.get( eeAddress, triggers );
}

void help()
{
  Serial.println("This is simple 24h clock base on watchdog timer and supports up to 10 events.");
  Serial.flush();
  Serial.println("The output is in 24 hour format.");
  Serial.println("You can add more job in the function 'loop', feel free to modified it.");
  Serial.flush();
  Serial.println("Syntax");
  Serial.println("    [set_time] [get_time] [set_trigger_] [show_triggers] [help] time");
  Serial.println("Options");
  Serial.println("    help          Display this help");
  Serial.println("    set_time      Set the current time, if not, it will count from 00:00");
  Serial.println("        Example: set_time 15:25");
  Serial.println("    get_time      Get the current time");
  Serial.println("        Example: get_time");
  Serial.println("    set_trigger_  Set events to do, have to be specific the event's location");
  Serial.println("        Example: set_trigger_0 15:25");
  Serial.println("    show_triggers View all events");
  Serial.println("        Example: show_triggers");
  Serial.println("    save          Save all records to EEPROM");
  Serial.println("        Example: save");
  Serial.flush();
}

bool isNumberic(String input)
{
  String numberic = "0123456789";
  for (int i=0; i<input.length(); i++){
    for (int j=0; j<numberic.length(); j++){
      if (!(input[i] ^ numberic[j]))
        break;
      if (j == numberic.length()-1)
        return false;
    }
  }
  return true;
}

unsigned long str_processing()
{
  int SpaceIndex, CommaIndex;
  String time, stime_h, stime_m, tmp;
  int time_h, time_m;
  SpaceIndex = setting.indexOf(" ");
  time = setting.substring(SpaceIndex+1);
  CommaIndex = time.indexOf(":");
  stime_h = time.substring(0, CommaIndex);
  stime_m = time.substring(CommaIndex+1, 5);
  time_h = stime_h.toInt();
  time_m = stime_m.toInt();
  return (time_h*3600UL + time_m*60UL);
}

void serialEvent() {
  setting = Serial.readString();
  Serial.print("I received: ");
  Serial.println(setting);
  if ( setting.length() > 20 ){
    Serial.println("wrong format");
  }
  else{
    setting.toLowerCase();
    if ( setting.startsWith("set_time") ){
      Serial.println("setting time ...");
      if ( setting.indexOf(":") == -1 )
        Serial.println("time not found");
      else{
        seconds = str_processing();
        return;
      }
    }
    
    if ( setting.startsWith("get_time") ){
      Serial.println("getting time ...");
      Serial.print("current time: ");
      Serial.println(String(seconds/3600) + ":" + String(seconds%3600/60));
      return;
    }
    
    if ( setting.startsWith("set_trigger") ){
      
      if ( setting.indexOf(":") == -1 )
        Serial.println("time not found");
      else{
        unsigned long trigger;
        unsigned int DashIndex, SpaceIndex, position;
        String sposition;
        
        trigger = str_processing();
        DashIndex = setting.lastIndexOf("_");
        SpaceIndex = setting.indexOf(" ");
        
        sposition = setting.substring(DashIndex+1, SpaceIndex);
        if (isNumberic(sposition)){
          position = sposition.toInt();
          Serial.println("setting trigger " + sposition + " ...");
          triggers[position] = trigger;
          return;
        }
      }
    }
    
    if ( setting.startsWith("show_triggers") ){
      Serial.println("displaying trigger ...");
      for (int i = 0; i < 10; i++) {
        Serial.println( "event_" + String(i) + " " + String(triggers[i]/3600) + ":" + String(triggers[i]%3600/60));
      }
      return;
    }
    
    if ( setting.startsWith("save") ){
      Serial.println("saving triggers on eeprom ...");
      EEPROM.put(eeAddress, triggers);
      return;
    }
    
    if ( setting.startsWith("help") ){
      help();
      return;
    }
  }
  help();
  return;
}

void loop()
{
  for (int i = 0; i < 10; i++) {
    if (triggers[i] == seconds and triggers[i] != 0){
        // do sth here
    }
  } 
  
  _millis = millis();
  if (_millis < local_millis){
    local_millis = _millis;
  }
  
  if ((_millis - local_millis)/1000 >= 1){
    if (seconds >= 86340) // 86340 == 23:59
      seconds = 0;
    else
      seconds+=1;
    local_millis = _millis;
  }
}