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

int Nsv = 18;
int vec_size = 5; // Write this manually to avoid unnecessarily complicated code
float gamma = 0.0041;

// The intercept
float RO = -3.04603;

// The support vector coefficients multiplied by 10,000. Note, all elements of the array should be max 65535, otherwise uint_16_t will overflow
const uint16_t alpha[] PROGMEM =  {2259, 9351, 1793, 2093,10000,10000, 4789, 3879,10000,  291, 5866,10000, 3624,10000, 3060, 9402,  413, 3180};


// The support vectors. Note, all elements of the array should be max 65535, otherwise uint_16_t will overflow
const uint16_t SupportVectors[] PROGMEM = {
    8160,    7760,   24240,    6960,    8400,
    8240,    9040,   24000,    8160,    8000,
    8240,    9200,   24880,    6800,    6560,
    8000,    7440,   24000,    8160,    7840,
    8480,    7440,   24800,    7840,    6320,
    8400,    7440,   24240,    8160,    8400,
    8240,    7600,   24240,    8000,    6400,
    8640,    7600,   24880,    7840,    8400,
    8400,    9200,   24080,    6800,    6560,
    8640,    7440,   24080,    8000,    8160,
    8240,    9040,   24800,    8160,    8160,
    7760,    8160,   25040,    6800,    8160,
    7840,    9040,   24640,    8240,    7840,
    8640,    7440,   24080,    6720,    6560,
    7760,    7600,   24880,    6960,    8160,
    8480,    9200,   24880,    6800,    6560,
    8640,    7600,   24240,    6720,    6320,
    8400,    7600,   25040,    6720,    6560
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
float iteration = 0;                                                                 //REMOVE

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
  float new_point[5] = {data_matrix[0][0], data_matrix[0][1], data_matrix[0][2], data_matrix[0][3], data_matrix[0][4]};


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
//    Euc_norm_sq = sqrt(Euc_norm_sq); // This is the (final) calculated distance!!! OR IS IT??
    
//    debugFile = SD.open("debug.txt", FILE_WRITE);         //REMOVE
//    if (debugFile) {                                      //REMOVE
//      debugFile.println("");                              //REMOVE
//      debugFile.close();                                  //REMOVE
//    }                                                     //REMOVE
    
    
    counter = 0;
    float astichan = - gamma * Euc_norm_sq;
    float kernel_value = exp(astichan);
    float alf = pgm_read_word_near(alpha + row)/10000.0; // Fetch from Flash memory location: address(alpha) + row
    BIGsum += alf * kernel_value;

    debugFile = SD.open("debug.txt", FILE_WRITE);         //REMOVE
    if (debugFile) {                                      //REMOVE
      debugFile.print(Euc_norm_sq);                       //REMOVE
      debugFile.print(",  ");                             //REMOVE
      debugFile.print(astichan);                          //REMOVE
      debugFile.print(",  ");                             //REMOVE
      debugFile.print(kernel_value, 4);                   //REMOVE
      debugFile.print(",  ");                             //REMOVE
      debugFile.print(alf, 4);                            //REMOVE
      debugFile.print(",  ");                             //REMOVE
      debugFile.print(BIGsum, 4);                         //REMOVE
      debugFile.println(";     ");                        //REMOVE
      debugFile.close();                                  //REMOVE
    }                                                     //REMOVE
    
  }
  iteration++;                                          //REMOVE
  debugFile = SD.open("debug.txt", FILE_WRITE);         //REMOVE
  if (debugFile) {                                      //REMOVE
    debugFile.println("============================="); //REMOVE
    debugFile.print("               ");                 //REMOVE
    debugFile.println(iteration);                       //REMOVE
    debugFile.println("-----------------------------"); //REMOVE
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
    PowConsFile.print(iteration);                       //REMOVE
    PowConsFile.print(") ");                            //REMOVE
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
