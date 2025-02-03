#define SIG_GEN_PIN 4
constexpr int SIG_READ_PIN = A0;

unsigned long prev_millis_out = 0;
unsigned long prev_millis_in = 0;
const unsigned long out_interval = 500;
const unsigned long in_interval = 16;
bool sig_state = true;

void setup() {
  Serial.begin(115200);
  pinMode(SIG_GEN_PIN, OUTPUT);
  digitalWrite(SIG_GEN_PIN, sig_state);
  pinMode(SIG_READ_PIN, INPUT);
}

void loop() {
  unsigned long current_millis = millis();

  if (current_millis - prev_millis_out >= out_interval) {
    prev_millis_out = current_millis;
    sig_state = !sig_state;
    digitalWrite(SIG_GEN_PIN, sig_state);
  }

  if (current_millis - prev_millis_in >= in_interval) {
    int analog_val = analogRead(SIG_READ_PIN);
    float voltage = (analog_val / 1023.0f) * 5.0f;
    Serial.print("Voltage:"); Serial.println(voltage);
  }
}
