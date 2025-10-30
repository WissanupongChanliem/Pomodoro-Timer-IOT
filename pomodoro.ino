#include <LiquidCrystal_I2C.h>
#include "secrets.h"
#include <Firebase.h>
#include <WiFiS3.h>

void  init();
void  print_timer();

LiquidCrystal_I2C lcd(0x27,  16, 2);
Firebase fb(REFERENCE_URL, AUTH_TOKEN);
WiFiClient network;
String   stored_round = String();
String   stored_session = String();
int      session;

int status = 0;
int respond_code;

/*-- emoji-- */

byte pencil[8] = {
  B11111,
  B11111,
  B10001,
  B10001,
  B10001,
  B10101,
  B01010,
  B00100 
  };

byte cross[8] = {
  B00000,
  B10001,
  B01010,
  B00100,
  B01010,
  B10001,
  B00000,
  B00000
} ;

byte coffee[8] = {
  B00000,
  B10100,
  B01010,
  B00000,
  B11111,
  B10001,
  B10001,
  B01110
};

byte moon[8] = {
  B00000,
  B00000,
  B01110,
  B10011,
  B00011,
  B10011,
  B01110,
  B00000
};

byte wifi[8] = {
  B00000,
  B01110,
  B10001,
  B00000,
  B01110,
  B10001,
  B00000,
  B00100
};

byte smile[8] = {
  B11011,
  B10001,
  B01010,
  B01010,
  B00000,
  B10001,
  B10001,
  B01110
};

/*--------*/
void setup() {
  // put your setup code here, to run once:
  
  // button pin
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  // lcd pin
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  // ultrasonic
  pinMode(4, OUTPUT); // trig
  pinMode(5, INPUT); // echo

  
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  // wifi
  lcd.createChar(1, wifi);
  lcd.setCursor(0, 0);
  lcd.print("Connecting... ");
  lcd.setCursor(15, 0);
  lcd.write(byte(1));
  #if !defined(ARDUINO_UNOWIFIR4)
        WiFi.mode(WIFI_STA);
  #endif
   while (status != WL_CONNECTED) {
        status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.print(status);
        Serial.println(WIFI_SSID);
        delay(5000);
    }
    Serial.println("WiFi Connected");
  
  
  fb.getInt("Session", session);
  stored_round = String("session") + session;
  stored_session = stored_round + "/status";
  Serial.println(stored_round);
  Serial.println(stored_session);
  respond_code = fb.setString(stored_session, "finished");
  if (respond_code != 200)
  {
            Serial.print("Respond Code: ");
            Serial.println(respond_code);
  }
  session++ ;

  stored_round = String("session") + session;
  respond_code = fb.setInt("Session", session);
  if (respond_code != 200)
  {
            Serial.print("Respond Code: ");
            Serial.println(respond_code);
  }

  stored_round = String("session") + session;
  stored_session = stored_round + "/status";
  stored_round = String("session") + session;
  stored_session = stored_round + "/status";
  respond_code = fb.setString(stored_session, "study");
  if (respond_code != 200)
  {
            Serial.print("Respond Code: ");
            Serial.println(respond_code);
  }

  /* emoji */
  lcd.createChar(1, pencil);
  lcd.createChar(2, cross);
  lcd.createChar(3, coffee);
  lcd.createChar(4, moon);
  lcd.createChar(5, smile);
   init();
}

const int LONG_BREAK = 3;
const int SHORT_BREAK = 2;
const int WORKING = 1;

int   state;
int   button3_state;
int   button3_old_state;
int   button4_state;
int   button4_old_state;
unsigned long old_millis;
unsigned long restart = 0;
int   minute;
int   second;
int   time_interval;
int   read_round;
int   afk_second;
int   is_open = 1;

int trigPin = 4;
int echoPin = 5;


int get_round = 0;
int loss_focus_round = 0;

void loop() {
  button3_old_state = button3_state;
  button3_state = digitalRead(3);
  button4_old_state = button4_state;
  button4_state = digitalRead(2);

  if (button3_old_state == 1 && button3_state == 0)
  {
    switch (state)
    {
    case (1):
      state = 0;
      break;
    case (0):
      state = 1;
      break;
    }
    state_change();
  }

  // restart button
  if (button4_old_state == 1 && button4_state == 0 && ((restart != 0 && millis() - restart >= 1000) || restart == 0))
  {
    restart = millis();
    if (is_open)
    {
      is_open = 0;
      init();
      lcd.clear();
      digitalWrite(9, HIGH);
      digitalWrite(10, HIGH);
      digitalWrite(11, HIGH);
      lcd.noBacklight();
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("Starting...");
      lcd.setCursor(15, 0);
      lcd.write(byte(5));
      lcd.backlight();
      fb.getInt("Session", session);
      stored_round = String("session") + session;
      stored_session = stored_round + "/status";
      respond_code = fb.setString(stored_session, "finished");
      if (respond_code != 200)
      {
                Serial.print("Respond Code: ");
                Serial.println(respond_code);
      }
      session++ ;

      stored_round = String("session") + session;
      respond_code = fb.setInt("Session", session);
      if (respond_code != 200)
      {
                Serial.print("Respond Code: ");
                Serial.println(respond_code);
      }

      stored_round = String("session") + session;
      stored_session = stored_round + "/status";
      respond_code = fb.setString(stored_session, "study");
      if (respond_code != 200)
      {
                Serial.print("Respond Code: ");
                Serial.println(respond_code);
      }
      init();
      Serial.println(read_round);
      is_open = 1;
    }
  }
  
  if (state == 1)
  {
    if (millis() - old_millis >= 1000)
    {
      second += 1;
      if (second == 60)
      {
        minute += 1;
        second = 0;
      }
      print_timer();
      if (minute == time_interval)
      {
        state = 0;
        state_change();
        if (read_round % 2 == 1)
        {
          respond_code = fb.setInt(stored_round + "/round_studied", get_round + 1);
          get_round += 1;
          if (respond_code != 200)
          {
            Serial.print("Respond Code: ");
            Serial.println(respond_code);
          }
        }
        read_round += 1;
        lcd.setCursor(12, 1);
        lcd.print((read_round + 1) / 2);
        minute = 0;
        second = 0;

        if (read_round % 8 == 0)
          time_interval = LONG_BREAK;
        else if (read_round % 2 == 1)
          time_interval = WORKING;
        else
          time_interval = SHORT_BREAK;

        lcd.setCursor(0, 1);
        if (time_interval < 10)
          lcd.print(0);
        lcd.print(time_interval);
        lcd.print(":00");

        
      }
      old_millis = millis();
      if (time_interval == WORKING)
      {
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);

        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        int duration = pulseIn(echoPin, HIGH) * 0.034 / 2;
        Serial.print(afk_second);
        Serial.print(" ");
        Serial.println(duration);
        if (duration > 60)
        {
          afk_second += 1;
          if (afk_second > 5 && afk_second % 2)
          {
            digitalWrite(9, HIGH);
            digitalWrite(10, HIGH);
            digitalWrite(11, LOW);
          }
          else
          {
            digitalWrite(9, HIGH);
            digitalWrite(10, HIGH);
            digitalWrite(11, HIGH);
          }
        }
        else
        {
          afk_second = 0;
          digitalWrite(9, HIGH);
          digitalWrite(10, HIGH);
          digitalWrite(11, HIGH);
        }
        if (afk_second >= 10)
        {
          state = 0;
          respond_code = fb.setInt(stored_round + "/lose_focus", loss_focus_round + 1);
          loss_focus_round += 1;
          if (respond_code != 200)
          {
            Serial.print("Respond Code: ");
            Serial.println(respond_code);
          }
          state_change();
        }
      }
    }
  }
}

void  init()
{
  

  lcd.clear();
  state = 0;
  button3_state = 1;
  button3_old_state = 1;
  button4_state = 1;
  button4_old_state = 1;
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  minute = 0;
  second = 0;
  time_interval = WORKING;
  read_round = 1;
  lcd.print("Stopped");
  lcd.setCursor(15, 0);
  lcd.write(byte(2));
  lcd.setCursor(0, 1);
  lcd.print("25:00 ");
  lcd.print("Round 1");
  afk_second = 0;
  get_round = 0;
}

void  state_change()
{
  lcd.setCursor(0, 0);
    switch (state)
    {
      
      case (0): // Stop timer
        lcd.print("Stopped    ");
        lcd.setCursor(15, 0);
          lcd.write(byte(2));
        digitalWrite(9, HIGH);
        digitalWrite(10, HIGH);
        digitalWrite(11, LOW);
        afk_second = 0;
        break;
      case (1): // run timer
        if (read_round % 8 == 0)
        {
          lcd.print("Long Break ");
          lcd.setCursor(15, 0);
          lcd.write(byte(4));
          digitalWrite(9, LOW);
          digitalWrite(10, HIGH);
          digitalWrite(11, HIGH);
        } 
        else if (read_round % 2) 
        {
          lcd.print("Working    ");
          lcd.setCursor(15, 0);
          lcd.write(byte(1));
          digitalWrite(9, HIGH);
          digitalWrite(10, HIGH);
          digitalWrite(11, HIGH);
        }
        else
        {
          lcd.print("Short Break");
          lcd.setCursor(15, 0);
          lcd.write(byte(3));
          digitalWrite(9, HIGH);
          digitalWrite(10, LOW);
          digitalWrite(11, HIGH);
        }
        old_millis = millis();
        break;
    }
}

void  print_timer()
{
  lcd.setCursor(0, 1);
  if (second == 0)
  {
    if ((time_interval - minute) < 10)
      lcd.print(0);
    lcd.print((time_interval - minute));
    lcd.print(":00");
    return ;
  }
  if (time_interval - minute -1 < 10)
    lcd.print(0);
  lcd.print(time_interval - minute - 1);
  lcd.print(":");
  if (60 - second < 10)
    lcd.print(0);
  lcd.print(60 - second);
}
