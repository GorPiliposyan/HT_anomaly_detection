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
//File debugFile;
//File TimingFile;
//File yPredFile;
const int chipSelect = 4;
int alarm = 3;
int alarm2 = 9;




// ///////////////Store ML related parameters////////////////

int Nsv = 35;
int vec_size = 10; // Write this manually to avoid unnecessarily complicated code
float gamma = 0.0041;

// The intercept
float RO = -0.93821;

// The support vector coefficients multiplied by 10,000. Note, all elements of the array should be max 65535, otherwise uint_16_t will overflow
const uint16_t alpha[] PROGMEM =  {
    2205,  632,  953, 2238,  164, 3123,  511, 2919, 1793, 1523,  375,   92,  417,  629, 1001, 1287, 2006, 1749,
    1801,  334, 4182, 4272, 1011, 1946,  611, 1442, 3152,   63, 1177, 1588, 1386, 1912,  477,  439,  590
    };


// The support vectors. Note, all elements of the array should be max 65535, otherwise uint_16_t will overflow
const uint16_t SupportVectors[] PROGMEM = {
    9040,    9840,   26320,    7440,    7200,    9168,    9152,   26480,    7392,    7136,
    9040,    8240,   26640,    7200,    7040,    9104,    8272,   26368,    7696,    7168,
    9200,    8480,   26320,    7440,    6960,   10234,    8384,   26512,    7888,    8240,
    9200,    8800,   26080,    7200,    7360,    8832,    9584,   26384,    8176,    8112,
    9040,    8160,   26640,    8400,    7040,    8784,    8816,   26384,    8512,    8144,
    8640,    8480,   26080,    7440,    8800,    8672,    8400,   26352,    7632,    8704,
    8640,    8240,   26880,    7440,    8800,    8832,    8512,   26448,    8064,    8480,
    9040,    8480,   26480,    7840,    7040,   10528,    8624,   26400,    7648,    7184,
    9280,    8240,   26720,    8480,    8880,    9168,    8416,   26496,    7648,    7408,
    8880,    8000,   25920,    8480,    7200,    9120,    8240,   26288,    7664,    7168,
    9040,    8000,   26320,    8240,    9200,    9200,    8160,   26224,    8192,    7632,
    8800,    7840,   26640,    8640,    8480,    8736,    8592,   26480,    8368,    8512,
    8880,    8240,   26640,    7360,    8800,    8864,    8400,   26576,    7552,    7840,
    8880,    8160,   26080,    8400,    6960,    9024,    8192,   26128,    8432,    8352,
    9040,    7840,   26640,    8160,    6800,    8976,    8320,   26544,    7984,    7488,
    8880,    7840,   26480,    7360,    7040,    8944,    8432,   26400,    8080,    8224,
    9280,    8240,   25920,    8400,    6800,    9008,    8464,   26272,    8480,    8192,
    9200,    8000,   25920,    8240,    6960,   10211,    8240,   26368,    8112,    7488,
    8800,    7840,   25920,    8800,    8800,    8944,    8224,   26240,    8144,    8064,
    9280,    8160,   26640,    7360,    7040,    9248,    8256,   26512,    7664,    7088,
    8640,    9840,   26320,    8640,    8400,   10308,    8640,   26272,    8016,    7872,
    8640,    9680,   25840,    8640,    8400,    8752,    9488,   26240,    8480,    8416,
    9040,    9440,   26640,    7440,    7200,    9040,    9360,   26560,    7408,    7136,
    8640,    9840,   26640,    8800,    8240,    8768,    8816,   26608,    8288,    8368,
    9200,    9840,   26640,    7440,    7040,    9008,    8720,   26464,    7600,    8000,
    9200,    9680,   26640,    7360,    7040,    8896,    8592,   26416,    8160,    8096,
    8880,    8480,   26640,    7600,    8800,   10416,    8320,   26480,    7952,    8112,
    9200,    8240,   26640,    7440,    6800,    9136,    8288,   26368,    8096,    7136,
    9040,    8240,   26080,    8640,    8480,   10428,    8320,   26496,    7920,    8064,
    8880,    8160,   26640,    8480,    9040,    8800,    8528,   26304,    8576,    8608,
    9280,    8240,   25920,    8400,    9200,    8880,    8560,   26240,    8320,    8592,
    9200,    9680,   25920,    7200,    7200,    9008,    8848,   26304,    7648,    7136,
    9040,    8400,   26640,    8400,    8480,    8928,    9520,   26464,    8496,    8336,
    9200,    8480,   26480,    7440,    6800,    9104,    8384,   26576,    7456,    7040,
    8880,    9680,   26640,    8640,    8400,    9104,    8544,   26464,    7872,    7296
    };



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
  pinMode(alarm2, OUTPUT);
  
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
//  SD.begin(chipSelect);
  PowConsFile = SD.open("PowCons.txt", FILE_WRITE);
  if (PowConsFile) {
    PowConsFile.println("New session:");
    PowConsFile.println("");
    PowConsFile.close();
  } else {
    Serial.println("No SD");
  }

    
//  //''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
  digitalWrite (alarm, 0);
}

float data_matrix[5][10] = {{0,0,0,0,0,0,0,0,0,0},  //Row 0
                            {0,0,0,0,0,0,0,0,0,0},  //Row 1
                            {0,0,0,0,0,0,0,0,0,0},  //Row 2
                            {0,0,0,0,0,0,0,0,0,0},  //Row 3
                            {0,0,0,0,0,0,0,0,0,0}}; //Row 4
//float data_matrix[1][5];


//float A1_power_mW;
//float B1_power_mW;
//float C1_power_mW;
//float A2_power_mW;
//float B2_power_mW;
//float C2_power_mW;

int y_pred;
int iteration = 0;                                                                 //REMOVE
int Stability_agent[5] = {-1,-1,-1,-1,-1};
float Agent_output;
int Agent_pred;



void loop() {

 /////////////Shift data values down//////////////
  for(int column = 0; column < vec_size; column++){
    for(int row=3; row>=0; row--){
      data_matrix[row+1][column] = data_matrix[row][column];
    }
  }
  

// ///////////////Get new datapoint////////////////

  tcaselect(1);
//  A1_power_mW = ina219_A1.getPower_mW();
  data_matrix[0][0] = ina219_B1.getPower_mW(); //  B1_power_mW = ina219_B1.getPower_mW();
  data_matrix[0][1] = ina219_C1.getPower_mW(); //  C1_power_mW = ina219_C1.getPower_mW();

  tcaselect(2);
  data_matrix[0][2] = ina219_A2.getPower_mW(); //  A2_power_mW = ina219_A2.getPower_mW();
  data_matrix[0][3] = ina219_B2.getPower_mW(); //  B2_power_mW = ina219_B2.getPower_mW();
  data_matrix[0][4] = ina219_C2.getPower_mW(); //  C2_power_mW = ina219_C2.getPower_mW();


// /////////////Calculate moving averages//////////////

  for(int k=5; k<vec_size; k++){
    data_matrix[0][k] = (data_matrix[0][k-5]+data_matrix[1][k-5]+data_matrix[2][k-5]+data_matrix[3][k-5]+data_matrix[4][k-5])/5.0;
  }


// ///////////////Process new datapoint////////////////

  float new_point[10] = {data_matrix[0][0], data_matrix[0][1], data_matrix[0][2], data_matrix[0][3], data_matrix[0][4], data_matrix[0][5], data_matrix[0][6], data_matrix[0][7], data_matrix[0][8], data_matrix[0][9]};
//  float new_point[5] = {data_matrix[0][0], data_matrix[0][1], data_matrix[0][2], data_matrix[0][3], data_matrix[0][4]};


  // Calculate the decision function value: BIGsum
  float BIGsum = 0.0;
  unsigned int displayInt;
  int counter = 0;
  float x;

  iteration++;                                          //REMOVE
//  debugFile = SD.open("debug.txt", FILE_WRITE);         //REMOVE
//  if (debugFile) {                                      //REMOVE
//    debugFile.println("==============");                //REMOVE
////    debugFile.print("       Iteration: ");              //REMOVE
//    debugFile.println(iteration);                       //REMOVE
////    debugFile.println("                             "); //REMOVE
//    debugFile.close();                                  //REMOVE
//  }                                                     //REMOVE

  
  for(int row=0; row<Nsv; row++){ //  AAA

    // Find the square of Euclidean distance between the new point and one of the SVs.
    float Euc_norm_sq = 0.0;
    
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


  

//    debugFile = SD.open("debug.txt", FILE_WRITE);         //REMOVE
//    if (debugFile) {                                      //REMOVE
//      debugFile.print(Euc_norm_sq);                       //REMOVE
//      debugFile.print(", ");                              //REMOVE
//      debugFile.print(astichan);                          //REMOVE
//      debugFile.print(", ");                              //REMOVE
//      debugFile.print(kernel_value, 4);                   //REMOVE
//      debugFile.print(",  ");                             //REMOVE
//      debugFile.print(alf, 4);                            //REMOVE
//      debugFile.print(", ");                              //REMOVE
//      debugFile.print(BIGsum, 4);                         //REMOVE
//      debugFile.println(";");                             //REMOVE
//      debugFile.close();                                  //REMOVE
//    }                                                     //REMOVE
    
  }


  
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
  
  /////////////////// Stex avelacru stability agent-i array-@ //////////////////////
  Stability_agent[0] = Stability_agent[1];
  Stability_agent[1] = Stability_agent[2];
  Stability_agent[2] = Stability_agent[3];
  Stability_agent[3] = Stability_agent[4];
  Stability_agent[4] = y_pred;
  Agent_output = Stability_agent[0] + Stability_agent[1] + Stability_agent[2] + Stability_agent[3] + Stability_agent[4];
  
  // Agent prediction result
  if (Agent_output <0 ){
    Agent_pred = -1;  digitalWrite (alarm2, 220);
    } else{
    Agent_pred = 1;  digitalWrite (alarm2, 0);
  }
  
  //////////////////////////////////////////////////////////////////////////////////




    
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
    PowConsFile.print(iteration);            PowConsFile.print("), ");    //REMOVE
//    PowConsFile.print(A1_power_mW);    PowConsFile.print(", ");
    PowConsFile.print(data_matrix[0][0]);    PowConsFile.print(", ");
    PowConsFile.print(data_matrix[0][1]);    PowConsFile.print(", ");
    PowConsFile.print(data_matrix[0][2]);    PowConsFile.print(", ");
    PowConsFile.print(data_matrix[0][3]);    PowConsFile.print(", ");
    PowConsFile.print(data_matrix[0][4]);    PowConsFile.print(", ");
    PowConsFile.print(data_matrix[0][5]);    PowConsFile.print(", ");
    PowConsFile.print(data_matrix[0][6]);    PowConsFile.print(", ");
    PowConsFile.print(data_matrix[0][7]);    PowConsFile.print(", ");
    PowConsFile.print(data_matrix[0][8]);    PowConsFile.print(", ");
    PowConsFile.print(data_matrix[0][9]);    PowConsFile.print(", ");
    PowConsFile.print(y_pred);               PowConsFile.print(", ");
    PowConsFile.print(BIGsum, 6);            PowConsFile.print(", ");
    PowConsFile.println(Agent_pred); 
    PowConsFile.close();
  } else {
    Serial.println("No SD");
  }


}
