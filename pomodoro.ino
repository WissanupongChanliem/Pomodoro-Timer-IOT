#include <LiquidCrystal_I2C.h>
#include "secrets.h"
#include <Firebase.h>
#include <WiFiS3.h>

void  init();
void  print_timer();

LiquidCrystal_I2C lcd(0x27,  16, 2);
Firebase fb(REFERENCE_URL, AUTH_TOKEN);
WiFiClient network;

/* for on in firebase*/
String   stored_round = String();
String   stored_session = String();
int      session;

// for wifi connecting
int status = 0;
int respond_code;

// time interval
const int LONG_BREAK = 3;
const int SHORT_BREAK = 2;
const int WORKING = 1;

/* for button state checking */
int   state;
int   stop_button_state;
int   stop_button_old_state;
int   restart_button_state;
int   restart_button_old_state;

/* for counting time*/
unsigned long old_millis;
unsigned long restart = 0;
int   minute;
int   second;
int   time_interval;
int   read_round;

// check afk
int   afk_second;

// for simulate open device
int   is_open = 1;

/* for saving data in db*/
int get_round = 0;
int loss_focus_round = 0;


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

/* ultra sonic*/
const int trigPin = 4;
const int echoPin = 5;

/* button */
const int stopButton = 3;
const int restartButton = 2;

/* LED ( set low to open led) */
const int RED = 11;
const int GREEN = 10;
const int BLUE = 9;

/* A4 -> SDA/ A5 -> SC*/

void setup() {
  // put your setup code here, to run once:
  
  // button pin
  pinMode(restartButton, INPUT);
  pinMode(stopButton, INPUT);

  // lcd pin
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);

  // ultrasonic
  pinMode(trigPin, OUTPUT); // trig
  pinMode(echoPin, INPUT); // echo

  
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
  
  // setting new session and old session status
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

  /* emoji set up*/
  lcd.createChar(1, pencil);
  lcd.createChar(2, cross);
  lcd.createChar(3, coffee);
  lcd.createChar(4, moon);
  lcd.createChar(5, smile);
   init();
}



void loop() {
  /* update for state*/
  stop_button_old_state = stop_button_state;
  stop_button_state = digitalRead(stopButton);
  restart_button_old_state = restart_button_state;
  restart_button_state = digitalRead(restartButton);

  // stop button
  if (stop_button_old_state == 1 && stop_button_state == 0)
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
  if (restart_button_old_state == 1 && restart_button_state == 0 && ((restart != 0 && millis() - restart >= 1000) || restart == 0))
  {
    restart = millis();
    /* toggle to not open close all the lcd*/
    if (is_open)
    {
      is_open = 0;
      init();
      lcd.clear();
      digitalWrite(BLUE, HIGH);
      digitalWrite(GREEN, HIGH);
      digitalWrite(RED, HIGH);
      lcd.noBacklight();
    }
    else
    {
      lcd.setCursor(0, 0);
      lcd.print("Starting...");
      lcd.setCursor(15, 0);
      lcd.write(byte(5));
      lcd.backlight();

      /* reset session like close and real open*/
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
  
  // if time not stop
  if (state == 1)
  {
    // count time when pass 1 second
    if (millis() - old_millis >= 1000)
    {
      second += 1;
      if (second == 60)
      {
        minute += 1;
        second = 0;
      }
      print_timer();

      // if timer is over
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

        // set new time interval
        if (read_round % 8 == 0)
          time_interval = LONG_BREAK;
        else if (read_round % 2 == 1)
          time_interval = WORKING;
        else
          time_interval = SHORT_BREAK;

        // print new time interval on LCD
        lcd.setCursor(0, 1);
        if (time_interval < 10)
          lcd.print(0);
        lcd.print(time_interval);
        lcd.print(":00");
        afk_second = 0;
        return ;
      }
      old_millis = millis();

      // Check that you're still sitting
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

        // if you far from desk more than 60 cm
        if (duration > 60)
        {
          afk_second += 1;

          // when you far for 5 second
          if (afk_second > 5 && afk_second % 2)
          {
            digitalWrite(BLUE, HIGH);
            digitalWrite(GREEN, HIGH);
            digitalWrite(RED, LOW);
          }
          else
          {
            digitalWrite(BLUE, HIGH);
            digitalWrite(GREEN, HIGH);
            digitalWrite(RED, HIGH);
          }
        }
        else
        {
          afk_second = 0;
          digitalWrite(BLUE, HIGH);
          digitalWrite(GREEN, HIGH);
          digitalWrite(RED, HIGH);
        }
        // if you leave for 10 second stop the timer
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

// set state and many variable

void  init()
{
  lcd.clear();
  state = 0;
  stop_button_state = 1;
  stop_button_old_state = 1;
  restart_button_state = 1;
  restart_button_old_state = 1;
  digitalWrite(BLUE, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(RED, LOW);
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

/* for changing state*/
void  state_change()
{
  lcd.setCursor(0, 0);
    switch (state)
    {
      
      case (0): // Stop timer
        lcd.print("Stopped    ");
        lcd.setCursor(15, 0);
          lcd.write(byte(2));
        digitalWrite(BLUE, HIGH);
        digitalWrite(GREEN, HIGH);
        digitalWrite(RED, LOW);
        afk_second = 0;
        break;
      case (1): // run timer
        if (read_round % 8 == 0)
        {
          lcd.print("Long Break ");
          lcd.setCursor(15, 0);
          lcd.write(byte(4));
          digitalWrite(BLUE, LOW);
          digitalWrite(GREEN, HIGH);
          digitalWrite(RED, HIGH);
        } 
        else if (read_round % 2) 
        {
          lcd.print("Working    ");
          lcd.setCursor(15, 0);
          lcd.write(byte(1));
          digitalWrite(BLUE, HIGH);
          digitalWrite(GREEN, HIGH);
          digitalWrite(RED, HIGH);
        }
        else
        {
          lcd.print("Short Break");
          lcd.setCursor(15, 0);
          lcd.write(byte(3));
          digitalWrite(BLUE, HIGH);
          digitalWrite(GREEN, LOW);
          digitalWrite(RED, HIGH);
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
