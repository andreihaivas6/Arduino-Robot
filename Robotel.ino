#include <Servo.h>
int trig_pin = 6; // sonar; trig
int echo_pin = 5; // sonar; echo
int misc_pin = 4;// senzor miscare
int button_pin = 2;// buton
int buzzer_pin = 3;// buzzer
int motors_pin = 9;//motor stanga
int motord_pin = 10;//motor dreapta
int ledon_pin = 11;//led da senzor
int ledoff_pin = 12;//led nu senzor
int led_pin = 13;//LED
long dist = 0, timp = 0, miscare = 0, sb = 0, ok = 0, p = 0, q = 0;
int dd = 0, df = 100, ds = 0;
Servo servo;

void setup() {
    Serial.begin(9600);
    pinMode(motors_pin, OUTPUT);
    pinMode(motord_pin, OUTPUT);
    pinMode(trig_pin, INPUT);
    pinMode(trig_pin, OUTPUT);
    pinMode(misc_pin, INPUT);
    pinMode(buzzer_pin, OUTPUT);
    pinMode(button_pin, INPUT);
    pinMode(ledon_pin, OUTPUT);
    pinMode(ledoff_pin, OUTPUT);
    pinMode(led_pin, OUTPUT);
    servo.attach(7); //servomotor la 3.3V
}

void loop() {
    ok = analogRead(A0);
    Serial.println(ok);
    if (ok >= 500) {
        digitalWrite(led_pin, LOW);
        digitalWrite(ledon_pin, LOW);
        digitalWrite(ledoff_pin, LOW);

        //opreste motoare
        Serial.println("Opreste tot");
        digitalWrite(motors_pin, LOW);
        digitalWrite(motord_pin, LOW);

    }
    else {
        digitalWrite(led_pin, HIGH);
        Serial.println("LED");
        if (!q)
            sb = digitalRead(button_pin);
        if (sb == HIGH) {
            q = 1;
            Serial.println("Porneste robot");
        }
        while (q) {

            //pornesc motoarele
            Serial.println("Porneste motoare");
            digitalWrite(motors_pin, HIGH);
            digitalWrite(motord_pin, HIGH);

            delay(500);
            servo.write(82);
            delay(1);
            ok = analogRead(A0);
            if (ok >= 500)
                q = 0;
            sb = digitalRead(button_pin);
            if (sb == HIGH)
                if (!p)
                    p = 1;
                else
                    p = 0;
            if (p) {
                digitalWrite(ledon_pin, HIGH);
                delay(1);
                digitalWrite(ledoff_pin, LOW);
            }
            else {
                digitalWrite(ledoff_pin, HIGH);
                delay(1);
                digitalWrite(ledon_pin, LOW);
            }
            delay(500);
            if (q) {  // daca nu s-a schimb potentiometru (sa se opreaasca mai repede robotu')
                df = sdist();
                Serial.print("Merge fata "); Serial.print(df); Serial.print("cm   ");
                if (df <= 45 || df >= 1000)
                    move();
                paza();
            }
        }
    }
}

int sdist() {
    digitalWrite(trig_pin, LOW);
    delay(1);
    digitalWrite(trig_pin, HIGH);
    delay(1);
    digitalWrite(trig_pin, LOW);
    timp = pulseIn(echo_pin, HIGH);
    dist = (timp / 2) / 29, 1;
    Serial.print(dist);
    Serial.println("cm");
    return dist;
}
void paza() {
    miscare = digitalRead(misc_pin);
    if (miscare == HIGH && p == 1) {

        //opreste motoare
        Serial.println("Opreste motoare");
        digitalWrite(motors_pin, LOW);
        digitalWrite(motord_pin, LOW);

        delay(500);
        while (p) {
            int n = 0;
            for (int z = 800; z <= 1500 && p; z++) {
                tone(buzzer_pin, z);
                delay(5);
                n = n + 5;
                if (n % 250 == 0) {
                    digitalWrite(ledon_pin, HIGH);
                    digitalWrite(ledoff_pin, LOW);
                }
                else
                    if (n % 125 == 0) {
                        digitalWrite(ledon_pin, LOW);
                        digitalWrite(ledoff_pin, HIGH);
                    }
                sb = digitalRead(button_pin);
                if (sb == HIGH)
                    p = 0;
            }
            for (int z = 1500; z >= 800 && p; z--) {
                tone(buzzer_pin, z);
                delay(5);
                n = n + 5;
                if (n % 250 == 0) {
                    digitalWrite(ledon_pin, HIGH);
                    digitalWrite(ledoff_pin, LOW);
                }
                else
                    if (n % 125 == 0) {
                        digitalWrite(ledon_pin, LOW);
                        digitalWrite(ledoff_pin, HIGH);
                    }
                sb = digitalRead(button_pin);
                if (sb == HIGH)
                    p = 0;
            }
            noTone(buzzer_pin);
        }
        digitalWrite(ledon_pin, LOW);
        digitalWrite(ledoff_pin, HIGH);

        delay(1000);

        //porneste motoare
        Serial.println("Porneste motoare");
        digitalWrite(motors_pin, HIGH);
        digitalWrite(motord_pin, HIGH);
    }

}
void move() {

    //opreste motoare
    Serial.println("Opreste motoare");
    digitalWrite(motors_pin, LOW);
    digitalWrite(motord_pin, LOW);

    servo.write(13);
    delay(1000);
    dd = sdist();
    Serial.print(dd); Serial.print(" cm si ");
    servo.write(170);
    delay(1000);
    ds = sdist();
    Serial.print(ds); Serial.println(" cm");
    if (dd > 1000)
        dd = 0;
    if (ds > 1000)
        ds = 0;
    while ((!dd && !ds) || (dd <= 35 && ds <= 35)) {

        Serial.println("Cu spatele");
        //motoare spate 

        servo.write(13);
        delay(1000);
        dd = sdist();
        Serial.print(dd); Serial.print(" cm si ");
        servo.write(170);
        delay(1000);
        ds = sdist();
        Serial.print(ds); Serial.println(" cm");
        if (dd > 1000)
            dd = 0;
        if (ds > 1000)
            ds = 0;
    }
    servo.write(82);
    if (dd > ds) {
        if (dd >= 60) { // 50 + 20?   sa se merite
          //motor s on
          //motor d off
            digitalWrite(motors_pin, HIGH);
            digitalWrite(motord_pin, LOW);
            Serial.println("Face dreapta");
            delay(1000);
        }
        else {
            //2x
            //motor s on
            //motor d off
            digitalWrite(motors_pin, HIGH);
            digitalWrite(motord_pin, LOW);
            Serial.println("180 la dreapta");
            delay(1000);
        }
    }
    else {
        if (ds >= 60) {
            //motor s off
            //motor d on
            digitalWrite(motors_pin, LOW);
            digitalWrite(motord_pin, HIGH);
            Serial.println("Face stanga");
            delay(1000);
        }
        else {
            //2x
            //motor s off
            //motor d on
            digitalWrite(motors_pin, LOW);
            digitalWrite(motord_pin, HIGH);
            Serial.println("180 la stanga");
            delay(1000);
        }
    }

}
