#include <SPI.h>
#include <Adafruit_INA219.h>
#include <SD.h>
#define MUX_Address 0x70

//Adafruit_INA219 ina219_A1(0x40);
Adafruit_INA219 ina219_B1(0x41);
Adafruit_INA219 ina219_C1(0x44);
Adafruit_INA219 ina219_A2(0x40);
Adafruit_INA219 ina219_B2(0x41);
Adafruit_INA219 ina219_C2(0x44);


File PowConsFile;
File debugFile;
//File TimingFile;
//File yPredFile;
const int chipSelect = 4;
int alarm = 3;




// ///////////////Store ML related parameters////////////////

int Nsv = 233;
int vec_size = 5; // Write this manually to avoid unnecessarily complicated code
float gamma = 0.0035;

// The intercept
float RO = -0.0239212;

// The coefficients for support vectors
const PROGMEM uint16_t alpha[] =  {
   145,  5267,  4095,   991,  7718,  4263,  4242,  4256, 10000,  3713, 10000,  1436,  4655,  1905,  4122,  3345,   102,  6230,  5351,  8694,   
   985,  3300,  3404,  2998,    26,  3369,  3255,  3418,  5791,  3869,  6896,  6527,  9244,  2912,  1424,  6331,  2696,  7016,  3863,  2435,  
   2685,  4009, 10000,  4271,  5430,  5228,  3059,  2303,  2425, 10000,  4814,  2742,  6603,  2195,  8213,  3491,  5031,   788,  1103,  3633,  
   6183,  3060,  6518,    75,  5989,   526,  2314,   279,   678,   731,  3817,  4324,  2740,   290,  6852,  6183,  1255,  6347, 10000,  6009,  
   5933,   497,  9867, 10000,  1755,  6045,    66,  4157,  9653,  3031,  2507,  5114,  3106,  3394,  5755,  1648,  6858,  5678,  4088,  1117,  
   1659,  3399,   718,  2604,  2302,  1092, 10000,  4679,  1267,  8902,  2730,  5851,  4299,   651,  4789, 10000,  5665,  8481,  1026,  5476,   
   350,  6712, 10000,  2514,  8248,  9152,  7169, 10000,  4831,   823,    22,  2217,  6011,  3585,  9398, 10000,  4418,  1234,  5926,  4488,  
   8776,   415,  2594,  4296,  4023,  1865,  1681,  5817,  2052,  8328,  5450,  1191,  9090,   535,  8271,   174,  2873,  6760,  9772,  4954,  
   3757,    44, 10000,  1482,  3345,   387,   778,  6523, 10000,  1152,  6188,  7648,  5731,  4941,  5000,  1382,  3043, 10000,  8218, 10000,   
   173,   132,  5187,  5848,   678,  2631, 10000,   573,  2757,  9281,  2357,  1260,   497,  6067, 10000,  6608, 10000,  2945,  2660,  1590,  
   5637,  1450,  7210,  1438,  2816,  3381, 10000,     3,  1655,  1888,  3377,  1856,  6064,  2684,  3880,  1261, 10000,  4794,  2052,   103,  
   1626,   563,  6482,  1023,  4605,  5352,  2522,  2885,  1236,  5461,  5870,  1411,  4195};


// The data to be saved...
const PROGMEM uint16_t SupportVectors[] = {
    4880,    4960,   26720,    5120,     800,
    1200,     640,   26080,     560,     400,
     880,    3440,   27040,    4480,    3040,
    4480,     880,   26640,     800,    4880,
     880,     640,   26080,    3120,    2720,
    1440,     800,   27040,    3280,     400,
    4320,    3680,   26240,    4160,    7200,
    4880,    3440,   26880,     560,    4560,
   10320,    8640,   26640,    9680,    9280,
    4560,    5920,   26720,    4960,    3840,
    4960,    3280,   25920,    4720,    4560,
    4160,    3680,   26480,    3120,    2480,
    4960,    3280,   26880,    4480,    8000,
    4720,    5760,   26880,    3280,     560,
    4560,     800,   26080,    3280,    2480,
    4320,    5760,   26880,     400,    4080,
    4880,    3440,   26720,    4720,    2880,
    9440,    3680,   26080,    9040,    9200,
    4720,    5520,   27040,    3120,    2880,
    4960,    3680,   26320,    7600,    7200,
    1200,    4160,   25920,    4880,     400,
    4880,    3520,   25920,    4880,     640,
    4320,     640,   26880,     640,    2320,
    4320,    3680,   26880,    7840,    7200,
    4320,    1040,   27040,    3280,    2640,
    4320,    1040,   26080,    4880,     400,
    1280,     800,   26080,    3440,    4480,
    4880,    3440,   26240,    4480,    7600,
    1280,    5600,   25920,     880,     560,
    4720,    5280,   27040,     400,    2640,
    4720,    4080,   26880,    3280,    4560,
    4080,     880,   27040,    3280,    4080,
    4320,    5920,   26880,     560,     640,
    5120,    3520,   26320,    4480,    9200,
    4480,    1040,   26880,    4720,     240,
    1040,    3920,   26880,    3120,    4320,
    4160,     800,   26880,    3280,     400,
    1040,     800,   25920,     560,    2880,
    4320,    5760,   26880,    4880,     800,
    1280,     800,   26240,     880,    4880,
    4880,    3440,   26240,     400,    4720,
    1440,    3520,   27040,     640,     400,
    4960,    3680,   25920,     880,    4880,
    4720,     640,   26880,     560,     400,
    5360,    3280,   26640,     880,     800,
    1200,    3440,   25920,     800,    2880,
    1280,    3280,   26320,    4960,     400,
    4880,    3520,   26480,    3040,    7440,
    4560,    6000,   26080,    4720,     880,
    4560,    5600,   26880,    9040,    8640,
    1280,     640,   26880,    4960,     640,
    1040,    3680,   27040,    4880,     560,
    4560,    5920,   26880,     640,    4080,
    4560,    3520,   26320,    3040,    7360,
    1280,    5920,   26080,    4880,     560,
    4160,     800,   25920,     640,    4080,
    4560,    3120,   27040,     800,     400,
    4880,     560,   26080,    4880,    4160,
    4960,    5760,   26720,    3440,    3120,
    4160,    4480,   27040,    4880,    4160,
    4720,    5760,   25920,     800,     800,
    4720,    3280,   26880,    4560,    2720,
    4720,    5760,   26320,    4960,     400,
    4560,    5760,   26240,     400,    4080,
    4720,    5520,   26880,    2880,     560,
    4960,    4080,   27040,    3040,    7440,
    1280,    3440,   26880,    3440,     240,
    1200,    1040,   25920,     800,    2480,
     800,    5360,   26320,    3120,     400,
     880,     560,   26880,     640,    2720,
     880,    3280,   25920,     880,     800,
    1280,    3680,   25920,     560,     400,
    8800,    4080,   26480,    7840,    8800,
    1040,     640,   26880,    4960,    3840,
     880,    3920,   27040,     640,     240,
    4880,    1040,   26080,     640,    4720,
    1040,    5920,   26640,     800,    2880,
    4880,     880,   26880,     240,    2880,
     880,    1040,   25920,     880,    4720,
    1200,    3920,   25920,     400,    4480,
     880,    5600,   26080,    3280,    3120,
    1040,    1040,   25920,    4960,     800,
    9440,    9040,   26240,    8000,    8800,
    9600,    3680,   26080,    7600,    7360,
    1040,    3840,   26720,    2880,    2480,
    1040,    3440,   25920,    3440,     400,
    9680,    3840,   26880,    8880,    7360,
    4320,    3680,   26080,     400,     560,
    4880,     640,   26880,    2880,     400,
    4720,     800,   26080,    3440,     240,
    1200,     800,   26080,    4880,    3040,
    4480,     640,   26880,    4960,     800,
    4720,    5600,   26080,     640,    2880,
    4320,    3440,   26080,     880,    2720,
    9840,    9200,   26080,    9680,    9040,
    9600,    8480,   26480,    8640,    7760,
    1040,    5760,   27040,    3280,     640,
    4160,    3680,   26080,    3440,     400,
    1280,    4160,   26080,    5120,     640,
    4320,     880,   27040,    4720,    4480,
    4480,     640,   26480,    4880,    3040,
     880,    5760,   26080,    3440,     640,
    4720,    3520,   26720,    4480,    9280,
    4720,    3520,   26480,    4880,    3040,
    4160,     640,   26080,    4880,    3920,
    4720,    3680,   26080,    3120,    2720,
    1200,    6000,   26720,     400,     640,
    4720,    3280,   25920,    4880,     800,
    4160,    4080,   26880,    3280,    4320,
    4480,    5520,   26720,    7600,    7440,
    4480,     880,   25920,    4880,    4480,
    4160,    3920,   26640,    2880,    7440,
    4480,     800,   27040,     400,    4480,
    9600,    5360,   26080,    8000,    7360,
    4160,    3440,   26880,     560,    2880,
    4960,     880,   26640,    4880,    4720,
     880,    1040,   27040,    3280,     400,
    1040,     880,   27040,     800,     240,
    1040,    5920,   26480,    4880,    4080,
    4720,     800,   25920,     880,    2640,
    1280,    1280,   25920,    3280,    2880,
    1040,     880,   26240,    5280,    4080,
    1280,     880,   26080,    5280,     560,
    4720,    5600,   26320,    7840,    7440,
    1280,     800,   27040,     560,    4480,
    1200,    5920,   27040,    4880,     640,
    5120,     640,   26240,    4880,     640,
    9600,    5760,   26880,    7600,    7600,
    4480,    1040,   26720,    4880,    2720,
    1280,    3520,   26080,    4560,    4720,
    4720,     800,   27040,    3120,    2880,
    4720,    3440,   26480,    4880,    4720,
    4080,    6000,   26080,    4320,     400,
    1440,    3840,   26080,    3520,    4480,
    1040,    5920,   26640,     800,    4160,
    1280,    3520,   26880,     640,    4720,
    4480,    5360,   26880,    3120,    7600,
     880,    3440,   26080,    3280,     400,
    9440,    5520,   26080,    9200,    8800,
    9440,    4080,   26880,    7840,    9040,
    4480,    5920,   26320,    4880,    4080,
    4560,    4080,   25920,     560,     400,
    4720,    5600,   26240,    7760,    7600,
    9440,    5760,   26240,    7760,    7600,
    4880,    3520,   26080,     800,     240,
    4480,    3680,   26640,    9040,    9200,
    1200,    1040,   27040,    4880,    4320,
    4560,     880,   25920,    3520,    4480,
    4720,    3920,   26720,    7840,    9040,
    4880,    5520,   26320,    3280,    7600,
    4720,    5600,   26240,    3120,    2880,
    4320,    6000,   26240,    4720,    3840,
    4720,    3680,   26640,    9040,    7200,
    1200,    3120,   26880,     400,     800,
    1040,    5760,   26880,    4880,    4080,
    1840,     880,   26480,     560,     640,
    9600,    3840,   26640,    9200,    9280,
    4480,    4080,   26240,    3120,    9040,
    1040,     880,   26080,    4720,    4880,
    4880,    5760,   26880,    4480,    8640,
    1280,    5760,   26080,    4880,    3840,
    4320,    4160,   26880,    4880,     240,
    9600,    8640,   26640,    8160,    7440,
    1680,    3440,   26480,    4560,    4720,
     880,     640,   27040,    3440,    2640,
    1280,    5760,   26880,    3440,    2880,
     880,    1040,   27040,     640,     400,
    1200,     640,   26080,    3440,     400,
    4960,    3520,   27040,    4880,     400,
    4720,    5760,   26720,    4960,     640,
    9280,    3520,   26480,    9440,    8160,
    9600,    3520,   26880,    8240,    7200,
    4160,    4320,   25920,    9040,    8880,
    4720,    3680,   26880,    3040,     400,
    1040,     560,   26080,     800,     400,
    1200,    5520,   26080,    3280,     400,
    1200,    3840,   27040,    3120,     560,
    5120,    3440,   26640,    8880,    9040,
     880,    3840,   27040,     400,    2720,
    4560,    5920,   26720,    4560,    8880,
    1200,    3520,   26640,    3120,     240,
   10080,    8480,   26080,    9280,    8800,
    4960,    3440,   26880,    3120,    2720,
    4160,     800,   26880,     400,     640,
    4960,    3440,   26880,    4880,    3920,
    4160,    4080,   27040,     400,    4160,
    5120,    3680,   26720,    4320,    9440,
    4720,     800,   25920,     560,    2880,
    4320,    5760,   27040,    4720,    4080,
    1200,     880,   27040,     400,    2480,
    4880,    3440,   26640,     400,    2880,
    1040,     640,   26720,    4880,     400,
    1200,    5920,   26320,     640,     400,
    1280,    5760,   26080,     560,    4160,
    9280,   10080,   26640,    9200,    8640,
    1200,    3680,   26080,    4880,    3040,
    4880,    1040,   25840,     800,     400,
    4080,    4480,   27040,     640,     400,
    4480,     880,   27040,     800,     240,
    4720,     640,   26240,     880,    4720,
    1200,    5920,   26320,    4560,    4320,
    4880,    5760,   26240,    4720,    4080,
    4560,    5920,   26240,     800,    4080,
    4720,    5600,   26880,    8800,    8640,
    1280,    1200,   26240,     400,     240,
    4080,    4080,   26240,    7840,    8800,
    9200,    5920,   26880,    9040,    8800,
    4480,     880,   25920,    4960,     880,
    4720,     640,   26880,    3120,    2720,
    4880,    5520,   26080,    3280,     560,
    1440,    5360,   26080,     400,    2720,
    4720,    3280,   25920,    3840,    2880,
    5360,    5520,   26640,     560,     800,
    9600,    3440,   26880,    9200,    8800,
    5280,     880,   26720,    3440,    3040,
     880,    3440,   26080,     640,    4480,
    1040,    3440,   26720,    4880,    4960,
    1040,    3440,   26080,    3280,    2640,
    1040,    3440,   25920,     800,     400,
    1040,    3520,   27040,     640,    4480,
    1280,    1040,   27040,    3040,    2880,
    1280,    5760,   26880,     640,    4160,
    1200,     880,   27040,    3440,    4320,
    4480,    3920,   26080,     400,    2720,
    1280,     880,   26880,    4720,    2480,
     880,    3120,   26880,    4880,     800,
    4720,     800,   25920,    4960,     800,
    1040,    5760,   26880,     640,    2880,
    4880,    1040,   27040,    3120,    2880,
    1280,    5600,   26880,    3120,    2880,
    4960,     800,   27040,     800,    4560,
    1280,     560,   26880,     800,    4560,
     880,    6000,   26880,     800,     880
    };

// ///////////////////Define functions////////////////////

// //Check if the point is inlier or outlier.
//bool _is_HT_clean (float a,float b, float c){
//  float new_point[3] = {a,b,c};
//  // Calculate the decision function value: BIGsum
//  float BIGsum =0;
//  for(int row=0; row<Nsv; row++){
//
//    // Find the square of Euclidean distance between the new point and one of the SVs.
//    float Euc_norm_sq = 0;
//    for(int i=0; i<vec_size; i++){
//      float x = Sup_Vec_matrix[row][i] - new_point[i]; // keep other operations outside the sq function
////      float x = Sup_Vec_matrix[row][0] - new_point0; // keep other operations outside the sq function
//      Euc_norm_sq += sq(x);
//    }
//    
//    float kernel_value = exp(- gamma * Euc_norm_sq);
//    BIGsum += alpha[row] * kernel_value;
//    
//  }
//  BIGsum -= RO;
////   Return the results
//  if (BIGsum >=0 ){
//    return 1;
//  }
//  else{
//    return 0;
//  }
//
//}


// To run the I2C MUX.
void tcaselect ( uint8_t i2c_bus) {
  if (i2c_bus > 7 ) return;
  Wire.beginTransmission(MUX_Address);
  Wire.write(1 << i2c_bus);
  Wire.endTransmission();
  //delay(10);
}



void setup() {
  Serial.begin(9600);
  //  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //  ina219.setCalibration_32V_1A();
  //  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
  //  //ina219.setCalibration_16V_400mA();

  tcaselect(1);
//  ina219_A1.begin();
//  ina219_A1.setCalibration_32V_1A();
  ina219_B1.begin();
  ina219_B1.setCalibration_32V_1A();
  ina219_C1.begin();
  ina219_C1.setCalibration_32V_1A();

  tcaselect(2);
  ina219_A2.begin();
  ina219_A2.setCalibration_32V_1A();
  ina219_B2.begin();
  ina219_B2.setCalibration_32V_1A();
  ina219_C2.begin();
  ina219_C2.setCalibration_32V_1A();

  pinMode(chipSelect, OUTPUT);
  pinMode(alarm, OUTPUT);
  
//  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (!SD.begin(chipSelect)) {
      digitalWrite (alarm, 220);
      delay(50);
      digitalWrite (alarm, 0);
      delay(500);
    }
  }
  PowConsFile = SD.open("PowCons.txt", FILE_WRITE);
  if (PowConsFile) {
    PowConsFile.println("Starting a new session:");
    PowConsFile.println("");
    PowConsFile.close();
  } else {
    Serial.println("couldn't write to SD");
  }

    
//  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  digitalWrite (alarm, 0);
}

//float data_matrix[5][10] = {{0,0,0,0,0,0,0,0,0,0},  //Row 0
//                            {0,0,0,0,0,0,0,0,0,0},  //Row 1
//                            {0,0,0,0,0,0,0,0,0,0},  //Row 2
//                            {0,0,0,0,0,0,0,0,0,0},  //Row 3
//                            {0,0,0,0,0,0,0,0,0,0}}; //Row 4
float data_matrix[1][5];


//float A1_power_mW;
//float B1_power_mW;
//float C1_power_mW;
//float A2_power_mW;
//float B2_power_mW;
//float C2_power_mW;

int y_pred;


void loop() {

// /////////////Shift data values down//////////////
//  for(int column = 0; column < vec_size; column++){
//    for(int row=3; row>=0; row--){
//      data_matrix[row+1][column] = data_matrix[row][column];
//    }
//  }
  

// ///////////////Get new datapoint////////////////

  tcaselect(1);

//  A1_power_mW = ina219_A1.getPower_mW();
//  B1_power_mW = ina219_B1.getPower_mW();
  data_matrix[0][0] = ina219_B1.getPower_mW();
//  C1_power_mW = ina219_C1.getPower_mW();
  data_matrix[0][1] = ina219_C1.getPower_mW();

  tcaselect(2);
//  A2_power_mW = ina219_A2.getPower_mW();
  data_matrix[0][2] = ina219_A2.getPower_mW();
//  B2_power_mW = ina219_B2.getPower_mW();
  data_matrix[0][3] = ina219_B2.getPower_mW();
//  C2_power_mW = ina219_C2.getPower_mW();
  data_matrix[0][4] = ina219_C2.getPower_mW();


// /////////////Calculate moving averages//////////////

//  for(int k=5; k<vec_size; k++){
//    data_matrix[0][k] = (data_matrix[0][k-5]+data_matrix[1][k-5]+data_matrix[2][k-5]+data_matrix[3][k-5]+data_matrix[4][k-5])/5.0;
//  }


// ///////////////Process new datapoint////////////////

//  float new_point[10] = {data_matrix[0][0], data_matrix[0][1], data_matrix[0][2], data_matrix[0][3], data_matrix[0][4], data_matrix[0][5], data_matrix[0][6], data_matrix[0][7], data_matrix[0][8], data_matrix[0][9]};
  float new_point[10] = {data_matrix[0][0], data_matrix[0][1], data_matrix[0][2], data_matrix[0][3], data_matrix[0][4]};


  // Calculate the decision function value: BIGsum
  float BIGsum = 0.0;
  unsigned int displayInt;
  int counter = 0;
  float x;
  
  for(int row=0; row<Nsv; row++){ //  AAA

    // Find the square of Euclidean distance between the new point and one of the SVs.
    float Euc_norm_sq = 0.0;
    
//    for(int i=0; i<vec_size; i++){// BBB
    for (int k = row*vec_size; k < (row*vec_size)+vec_size; k++) {// BBB
      displayInt = pgm_read_word_near(SupportVectors + k);
      
      x = displayInt/100.00 - new_point[counter]; // keep other operations outside the sq function
      Euc_norm_sq += sq(x);
      counter++;
      
    }
//    debugFile = SD.open("debug.txt", FILE_WRITE);         //REMOVE
//    if (debugFile) {                                      //REMOVE
//      debugFile.println("");                              //REMOVE
//      debugFile.close();                                  //REMOVE
//    }                                                     //REMOVE
    
    
    counter = 0;
    float astichan = - gamma * Euc_norm_sq;
    float kernel_value = exp(astichan);
    float alf = pgm_read_word_near(alpha + row)/1000000.0;
    BIGsum += alf * kernel_value;

    debugFile = SD.open("debug.txt", FILE_WRITE);         //REMOVE
    if (debugFile) {                                      //REMOVE
      debugFile.print(Euc_norm_sq);                       //REMOVE
      debugFile.print(",  ");                             //REMOVE
      debugFile.print(astichan);                          //REMOVE
      debugFile.print(",  ");                             //REMOVE
      debugFile.print(kernel_value, 6);                      //REMOVE
      debugFile.print(",  ");                             //REMOVE
      debugFile.print(alf, 6);                               //REMOVE
      debugFile.print(",  ");                             //REMOVE
      debugFile.print(BIGsum, 6);                            //REMOVE
      debugFile.println(";     ");                        //REMOVE
      debugFile.close();                                  //REMOVE
    }                                                     //REMOVE
    
  }

  debugFile = SD.open("debug.txt", FILE_WRITE);         //REMOVE
  if (debugFile) {                                      //REMOVE
    debugFile.println("============================="); //REMOVE
    debugFile.close();                                  //REMOVE
  }                                                     //REMOVE


  
  BIGsum = BIGsum + RO;
  // Return the results
  if (BIGsum <0 ){
    // Ring the HT alarm
    y_pred = -1;
    digitalWrite (alarm, 220);
    } else{
    y_pred = 1;
    digitalWrite (alarm, 0);
    
  }
  





    
//////////////////////////////////////////////////////


  //  TimingFile = SD.open("Timefile.txt", FILE_WRITE);
  //  if(TimingFile){
  //    //TimingFile.print(t1);
  //    //TimingFile.print(",  ");
  //    //TimingFile.print(t2);
  //    //TimingFile.print(",  ");
  //    //TimingFile.print(t2 - t1);
  //    //TimingFile.print(",  ");
  //    //TimingFile.print(t3 - t2);
  //    //TimingFile.print(",  ");
  //    TimingFile.println(t4 - t1);
  //  }
  //  TimingFile.close();


  //  // see if the card is present and can be initialized:
  //  if (!SD.begin(chipSelect)) {
  //    Serial.println("Card failed, or not present");
  //    // don't do anything more:
  //    while (!SD.begin(chipSelect)){
  //      digitalWrite (alarm, 220);
  //      delay(50);
  //      digitalWrite (alarm, 0);
  //      delay(500);
  //      }
  //  }


// ///////////////Save info to .txt files////////////////

  PowConsFile = SD.open("PowCons.txt", FILE_WRITE);
  if (PowConsFile) {
//    PowConsFile.print(A1_power_mW);
//    PowConsFile.print(",  ");
    PowConsFile.print(data_matrix[0][0]);
    PowConsFile.print(",  ");
    PowConsFile.print(data_matrix[0][1]);
    PowConsFile.print(",  ");
    PowConsFile.print(data_matrix[0][2]);
    PowConsFile.print(",  ");
    PowConsFile.print(data_matrix[0][3]);
    PowConsFile.print(",  ");
    PowConsFile.print(data_matrix[0][4]);
    PowConsFile.print(",  ");
//    PowConsFile.print(data_matrix[0][5]);
//    PowConsFile.print(",  ");
//    PowConsFile.print(data_matrix[0][6]);
//    PowConsFile.print(",  ");
//    PowConsFile.print(data_matrix[0][7]);
//    PowConsFile.print(",  ");
//    PowConsFile.print(data_matrix[0][8]);
//    PowConsFile.print(",  ");
//    PowConsFile.print(data_matrix[0][9]);
//    PowConsFile.print(",  ");
    PowConsFile.print(y_pred);
    PowConsFile.print(",  ");
    PowConsFile.println(BIGsum, 6);
    PowConsFile.close();
  } else {
    Serial.println("couldn't write to SD");
  }

// ///////////////Store info to SD card////////////////
//  yPredFile = SD.open("Predictions.txt", FILE_WRITE);
//  if (yPredFile) {
//    yPredFile.print(y_pred);
//    yPredFile.println(", ");
//    
//    yPredFile.close();
//  } else {
//    Serial.println("couldn't write to SD");
//  }
  
}
