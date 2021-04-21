#include <SPI.h>
#include <Adafruit_INA219.h>
#include <SD.h>
#define MUX_Address 0x70

//Adafruit_INA219 ina219_A1(0x40);
Adafruit_INA219 ina219_B1(0x41);
Adafruit_INA219 ina219_C1(0x44);
//Adafruit_INA219 ina219_A2(0x40);
Adafruit_INA219 ina219_B2(0x41);
//Adafruit_INA219 ina219_C2(0x44);


File PowConsFile;
//File TimingFile;
//File yPredFile;
const int chipSelect = 4;
int alarm = 3;
float t_HT = 0;



// ///////////////Store ML related parameters////////////////

int Nsv = 25;
int vec_size = 3; // Write this manually to avoid unnecessarily complicated code
float gamma = 0.001;

// The intercept
float RO = -0.1363947;

// The coefficients for support vectors
float alpha[25] =  {0.04685, 0.02931, 0.01247, 0.00265, 0.00212, 0.02692, 0.09118, 0.08021, 0.03974, 0.08833, 0.00443, 0.04944, 0.06547, 0.08631, 0.00900, 0.06191, 0.04043, 0.08373, 0.06254, 0.00361, 0.05348, 0.00241, 0.00148, 0.04570, 0.01025};
//float alpha[50] =  {0.15386, 0.03891, 0.09145, 0.09882, 0.14206, 0.13393, 0.19746, 0.01904, 0.47473, 0.13782, 0.39997, 0.02446, 0.07987, 0.05455, 0.04110, 0.08736, 0.02079, 0.02402, 0.40231, 0.16184, 0.52226, 0.27191, 0.25411, 0.08923, 0.18291, 0.45939, 0.38955, 0.10998, 0.46225, 0.19763, 0.31222, 0.04111, 0.17471, 0.2507, 0.3771, 0.15215, 0.44316, 0.00576, 0.17975, 0.27778, 0.07282, 0.30126, 0.07235, 0.11811, 0.27595, 0.27617, 0.20219, 0.00808, 0.46334, 0.2717};


// The support vectors
float Sup_Vec_matrix[25][3] = {{96.0,38.4,74.4},
{96.0,56.0,78.4},
{48.8,32.8, 4.0},
{8.8,34.4,48.8},
{8.8,57.6, 6.4},
{47.2,55.2, 4.0},
{ 8.8,57.6,49.6},
{ 8.8, 6.4,47.2},
{47.2, 5.6, 4.0},
{8.8, 6.4, 4.0},
{51.2,35.2, 8.0},
{100.0,38.4,92.0},
{44.8,56.0,90.4},
{48.8, 6.4,48.8},
{8.8,34.4, 4.0},
{47.2,57.6,48.8},
{92.0,56.0,92.8},
{10.4,57.6, 4.0},
{45.6,36.8,90.4},
{49.6,55.2,31.2},
{47.2,59.2, 8.0},
{51.2,34.4,48.8},
{92.0,35.2,92.0},
{49.6, 6.4, 6.4},
{8.8, 8.0,48.8}};


//float Sup_Vec_matrix[50][3] = {{44.8,36.8,76.0},
//{49.6, 8.0,49.6},
//{40.8,40.8, 2.4},
//{47.2,35.2,51.2},
//{ 8.8,35.2,49.6},
//{53.6, 8.8, 8.0},
//{ 8.8, 6.4,32.8},
//{45.6,36.8, 2.4},
//{98.4,102.4,94.4},
//{14.4,31.2,51.2},
//{51.2,32.8,90.4},
//{92.0,35.2,92.0},
//{43.2,57.6,78.4},
//{12.0,60.0, 6.4},
//{40.8,38.4,31.2},
//{49.6,57.6,78.4},
//{52.8,53.6,34.4},
//{98.4,84.0,88.8},
//{96.0,90.4,78.4},
//{43.2, 8.0,51.2},
//{ 8.8, 6.4, 4.0},
//{96.0,38.4,76.0},
//{92.8,56.0,76.0},
//{96.0,88.8,94.4},
//{12.0,35.2, 2.4},
//{47.2, 6.4, 2.4},
//{12.8,60.0,49.6},
//{40.8,38.4,92.0},
//{ 8.8,59.2, 6.4},
//{ 8.8,34.4, 4.0},
//{53.6,36.8, 6.4},
//{18.4, 6.4, 6.4},
//{48.8,57.6,31.2},
//{49.6, 6.4,31.2},
//{96.8,35.2,92.0},
//{ 8.8,57.6,32.8},
//{45.6,60.0,92.0},
//{43.2,34.4,31.2},
//{12.0,10.4,52.8},
//{ 8.8, 6.4,48.8},
//{12.0, 6.4,30.4},
//{90.4,57.6,92.0},
//{ 8.8,59.2,47.2},
//{44.8,59.2,49.6},
//{53.6,34.4,40.8},
//{48.8, 6.4,48.8},
//{ 6.4,36.8,34.4},
//{44.8, 5.6,31.2},
//{47.2,59.2, 5.6},
//{45.6,60.0,47.2}};



// ///////////////////Define functions////////////////////

// //Check if the point is inlier or outlier.
//bool _is_HT_clean (float a,float b, float c){
//  float new_point[3] = {a,b,c};
//  // Calculate the decision function value: BIGsum
//  float BIGsum =0;
//  for(int j=0; j<Nsv; j++){
//
//    // Find the square of Euclidean distance between the new point and one of the SVs.
//    float Euc_norm_sq = 0;
//    for(int i=0; i<vec_size; i++){
//      float x = Sup_Vec_matrix[j][i] - new_point[i]; // keep other operations outside the sq function
////      float x = Sup_Vec_matrix[j][0] - new_point0; // keep other operations outside the sq function
//      Euc_norm_sq += sq(x);
//    }
//    
//    float kernel_value = exp(- gamma * Euc_norm_sq);
//    BIGsum += alpha[j] * kernel_value;
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
//  ina219_A2.begin();
//  ina219_A2.setCalibration_32V_1A();
  ina219_B2.begin();
  ina219_B2.setCalibration_32V_1A();
//  ina219_C2.begin();
//  ina219_C2.setCalibration_32V_1A();

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

//  yPredFile = SD.open("Pred.txt", FILE_WRITE);
//  if (yPredFile) {
//    yPredFile.println("Starting a new session:");
//    yPredFile.println("");
//    yPredFile.close();
//  } else {
//    Serial.println("couldn't write to SD");
//  }


    
//  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  digitalWrite (alarm, 0);
}

//float data_matrix[1][3] = {{0,0,0}};
//float data_matrix[5][10] = {{0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}, {0,0,0,0,0,0,0,0,0,0}};

//float A1_power_mW;
float B1_power_mW;
float C1_power_mW;
//float A2_power_mW;
float B2_power_mW;
//float C2_power_mW;

int y_pred;


void loop() {

//  digitalWrite (alarm, 220);
//  delay(100);
//  digitalWrite (alarm, 0);
//  delay(50);

// /////////////Shift data values down//////////////
//  for(int column = 0; column < vec_size; column++){
//    for(int row=0; row<4; row++){
//      data_matrix[row+1][column] = data_matrix[row][column];
//    }
//  }
  

// ///////////////Get new datapoint////////////////

  tcaselect(1);

//  A1_power_mW = ina219_A1.getPower_mW();
  B1_power_mW = ina219_B1.getPower_mW();
//  data_matrix[0][0] = ina219_B1.getPower_mW();
  C1_power_mW = ina219_C1.getPower_mW();
//  data_matrix[0][1] = ina219_C1.getPower_mW();

  tcaselect(2);
//  A2_power_mW = ina219_A2.getPower_mW();
//  data_matrix[0][2] = ina219_A2.getPower_mW();
  B2_power_mW = ina219_B2.getPower_mW();
//  data_matrix[0][3] = ina219_B2.getPower_mW();
//  C2_power_mW = ina219_C2.getPower_mW();
//  data_matrix[0][4] = ina219_C2.getPower_mW();


// /////////////Calculate moving averages//////////////
//
//  for(int k=5; k<vec_size; k++){
//    data_matrix[0][k] = (data_matrix[0][k-5]+data_matrix[1][k-5]+data_matrix[2][k-5]+data_matrix[3][k-5]+data_matrix[4][k-5])/5;
//  }


// ///////////////Process new datapoint////////////////

//  float new_data_vector[3] = {data_matrix[0][0], data_matrix[0][1], data_matrix[0][2]};
//  float new_data_vector[10] = {data_matrix[0][0], data_matrix[0][1], data_matrix[0][2], data_matrix[0][3], data_matrix[0][4], data_matrix[0][5], data_matrix[0][6], data_matrix[0][7], data_matrix[0][8], data_matrix[0][9]};
  
//  float HTnoHT = _is_HT_clean(data_matrix[0][0], data_matrix[0][1], data_matrix[0][2]);

  float new_point[3] = {B1_power_mW, C1_power_mW, B2_power_mW};
  // Calculate the decision function value: BIGsum
  float BIGsum =0;
  for(int j=0; j<Nsv; j++){

    // Find the square of Euclidean distance between the new point and one of the SVs.
    float Euc_norm_sq = 0;
    for(int i=0; i<vec_size; i++){
      float x = Sup_Vec_matrix[j][i] - new_point[i]; // keep other operations outside the sq function
      Euc_norm_sq += sq(x);
    }
    float astichan = - gamma * Euc_norm_sq;
    float kernel_value = exp(astichan);
    BIGsum += alpha[j] * kernel_value;
    
  }
  BIGsum += RO;
//   Return the results
  if (BIGsum <0 ){
    // Ring the HT alarm
    y_pred = -1;
    digitalWrite (alarm, 220);
    t_HT = millis();
  } else{
    y_pred = 1;
    if(millis()- t_HT >50){
      digitalWrite (alarm, 0);
      }
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
    PowConsFile.print(B1_power_mW);
    PowConsFile.print(",  ");
    PowConsFile.print(C1_power_mW);
    PowConsFile.print(",  ");
//    PowConsFile.print(A2_power_mW);
//    PowConsFile.print(",  ");
    PowConsFile.print(B2_power_mW);
    PowConsFile.print(",  ");
//    PowConsFile.println(C2_power_mW);
    PowConsFile.print(y_pred);
    PowConsFile.print(",  ");
    PowConsFile.println(BIGsum);
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
