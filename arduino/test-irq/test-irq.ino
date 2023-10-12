#define WE 28

bool output_enabled = false;
bool is_triggered = false;

void setup(){
  Serial.begin(115200);
  pinMode(WE, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(WE), on_irq, FALLING);
}

void on_irq(){
  is_triggered = true;
}

void loop(){
  if (!is_triggered)
    return;

  is_triggered = false;
  static long long count = 0;
  Serial.println(count++);
}