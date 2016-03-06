const int state_none = -1;
const int state_begining_A = 0;
const int state_begining_B = 1;
const int state_game_A = 2;
const int state_game_B = 3;
const int table_hit = 1;
const int table_hit_none = 0;
const int time_out_millis = 2000;
const int cool_down_time_out = 50;
const int serving_player_NA = 0; // N/A
const int player_A = 1;
const int player_B = 2;

// pins
const int side_sensor_A = 6;
const int side_sensor_B = 7;
const int RGB_led_A = 53;
const int RGB_led_B = 51;
const int pin_button_reset = 30;
const int pin_button_A_add = 31;
const int pin_button_A_remove = 32;
const int pin_button_B_remove = 33;
const int pin_button_ = 34;
const int pin_servo = 8



int state = state_none;


unsigned long last_table_hit = 0;    

int score_A = 0;
int score_B = 0;

int rally_length = 0;

int serving_player = serving_player_NA;
int initial_serving_player = serving_player_NA;

void setup() {
  Serial.begin(9600);
  
  pinMode(RGB_led_A, OUTPUT);
  pinMode(RGB_led_B, OUTPUT);
}

void loop() {
  if(last_table_hit - millis() < cool_down_time_out) return; 
  
  int side_A = sense_table(side_sensor_A, 300);
  int side_B = sense_table(side_sensor_B, 300);
  
  if (side_A == table_hit || side_B == table_hit){
    last_table_hit = millis();
  }
  boolean time_out = (millis() - last_table_hit) > time_out_millis;
  
  scoring_state_machine(side_A, side_B, time_out);
  serving_state();
}

void scoring_state_machine(int side_A, int side_B, boolean time_out) {
  switch (state){
    case state_none:
      Serial.println("state_none: "+ (String) score_A + ":" + (String) score_B);
      transition_state_none(side_A, side_B, time_out);
      break;
    case state_begining_A: 
      Serial.println("state_begining_A: "+ (String) score_A + ":" + (String) score_B);
      transition_state_begining_A(side_A, side_B, time_out);
      break;
    case state_begining_B:
      Serial.println("state_begining_B: "+ (String) score_A + ":" + (String) score_B);
      transition_state_begining_B(side_A, side_B, time_out);
      break;
    case state_game_A:
      Serial.println("state_game_A: "+ (String) score_A + ":" + (String) score_B);
      transition_state_game_A(side_A, side_B, time_out);
      break;
    case state_game_B:
      Serial.println("state_game_B: "+ (String) score_A + ":" + (String) score_B);
      transition_state_game_B(side_A, side_B, time_out);
      break;
    default:
      Serial.println("default: "+ (String) score_A + ":" + (String) score_B);
      break;
  }
}

void scoring_state() {
}

int sense_table(int side, int calibration) {
   int sensor_value = analogRead(side);
   
   if (sensor_value > calibration)
     return  table_hit;
   
   return  table_hit_none;
}

void transition_state_none(int side_A, int side_B, boolean time_out) {
  if (side_A == table_hit)
  {
     state = state_begining_A; 
     return;
  }
  if (side_B == table_hit)
  {
     state = state_begining_B; 
     return;
  }
  
  if (time_out)
  {
     // nothing happens 
     return;
  }
}
void transition_state_begining_A(int side_A, int side_B, boolean time_out) {
  if (side_A == table_hit)
  {
     // stay in the same state
     return;
  }
  if (side_B == table_hit)
  {
     state = state_game_B; 
     return;
  }
  
  if (time_out)
  {
     state = state_none;
     return;
  }
}

void transition_state_begining_B(int side_A, int side_B, boolean time_out) {
  if (side_A == table_hit)
  {
     state = state_game_A; 
     return;
  }
  if (side_B == table_hit)
  {
     // stay in the same state
     return;
  }
  
  if (time_out)
  {
     state = state_none; 
     return;
  }
}

void transition_state_game_A(int side_A, int side_B, boolean time_out) {
  if (side_A == table_hit)
  {
     add_score_to_B(); 
     return;
  }
  if (side_B == table_hit)
  {
     state = state_game_B; 
     return;
  }
  
  if (time_out)
  {
     add_score_to_B();
     return;
  }
}

void transition_state_game_B(int side_A, int side_B, boolean time_out) {
  if (side_A == table_hit)
  {
     state = state_game_A;
     return;
  }
  if (side_B == table_hit)
  {
     add_score_to_A();
     return;
  }
  
  if (time_out)
  {
     add_score_to_A(); 
     return;
  }
}

void add_score_to_A () {
  score_A += 1;
  state = state_none;
  
  update_whos_serving(player_A);
}

void add_score_to_B () {
  score_B += 1;
  state = state_none;
  
  update_whos_serving(player_B);
}

void update_whos_serving(int point_winner) {
  int score_sum = score_A + score_B;

  if (score_sum == 1 && serving_player == serving_player_NA) {
    score_A = 0; score_B = 0;
    
    if (point_winner == player_A) {
      initial_serving_player = player_A;
    } else {
      initial_serving_player = player_B;
    }
  }
  update_serving_player();
}

void update_serving_player() {
  int score_sum = score_A + score_B;

  if(score_sum % 4 < 2) {
    serving_player = initial_serving_player;
  } else {
    if (initial_serving_player == player_A) {
      serving_player = player_B;
    } else if (initial_serving_player == player_B) {
      serving_player = player_A;
    }
  }
}

void add_score_A() {
  soc
}

void reset_game() {
  serving_player = serving_player_NA;
  score_A = 0;
  score_B = 0;
}




