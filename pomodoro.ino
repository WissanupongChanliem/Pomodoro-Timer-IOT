#include <LiquidCrystal_I2C.h>

void  init();
void  print_timer();

LiquidCrystal_I2C lcd(0x27,  16, 2);

void setup()
{
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
    init();
}

const int LONG_BREAK = 15;
const int SHORT_BREAK = 5;
const int WORKING = 1;
int state;
int button3_state;
int button3_old_state;
int button4_state;
int button4_old_state;
unsigned long old_millis;
int minute;
int second;
int time_interval;
int read_round;
int afk_second;
int is_open = 1;

int trigPin = 4;
int echoPin = 5;

void loop()
{
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

    if (button4_old_state == 1 && button4_state == 0)
    {
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
            is_open = 1;
            lcd.backlight();
            init();
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
                    state_change();
                }
            }
        }
    }
}

void init()
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
    lcd.setCursor(0, 1);
    lcd.print("25:00 ");
    lcd.print("Round 1");
    afk_second = 0;
    return;
}

void state_change()
{
    lcd.setCursor(0, 0);
    switch (state)
    {
        case (0): // Stop timer
            lcd.print("Stopped    ");
            digitalWrite(9, HIGH);
            digitalWrite(10, HIGH);
            digitalWrite(11, LOW);
            afk_second = 0;
            break;
        case (1): // run timer
            if (read_round % 8 == 0)
            {
                lcd.print("Long Break ");
                digitalWrite(9, LOW);
                digitalWrite(10, HIGH);
                digitalWrite(11, HIGH);
            } 
            else if (read_round % 2) 
            {
                lcd.print("Working    ");
                digitalWrite(9, HIGH);
                digitalWrite(10, HIGH);
                digitalWrite(11, HIGH);
            }
            else
            {
                lcd.print("Short Break");
                digitalWrite(9, HIGH);
                digitalWrite(10, LOW);
                digitalWrite(11, HIGH);
            }
            old_millis = millis();
            break;
    }
    return;
}

void print_timer()
{
    lcd.setCursor(0, 1);
    if (second == 0)
    {
        if ((time_interval - minute) < 10)
        {
            lcd.print(0);
        }
        lcd.print((time_interval - minute));
        lcd.print(":00");
        return;
    }
    if (time_interval - minute -1 < 10)
    {
        lcd.print(0);
    }
    lcd.print(time_interval - minute - 1);
    lcd.print(":");
    if (60 - second < 10)
    {
      lcd.print(0);
    }
    lcd.print(60 - second);
    return;
}
