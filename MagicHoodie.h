#ifndef MagicHoodie_h
#define MagicHoodie_h
#endif

// Define possible Frequency scalings:
#define FREQ_PD 0x00; //S0 S1
#define FREQ_2 0x01; //S0 S1
#define FREQ_20 0x02; //S0 S1
#define FREQ_100 0x03; //S0 S1

// Define hoodie class
class Hoodie {

  public:
    Hoodie();
    void setRGBPins(int R, int G, int B);
    void setSensorPins(int S0, int S1, int S2, int S3, int OE, int OUT);
    void measureFrequencies(unsigned long  freqs[4]);
    void computePercentages(int percentages[3], unsigned long freqs[4]);
    void generateRGB(int rgb[3], unsigned long values[4]);
    void setColorRGB(int r, int g, int b);
    void getColor(int rgb[3]);
    void setFreqScaling(int scaling);
    void getTenSamples(unsigned long rgb[3]);

  private:
    int PIN_S0, PIN_S1, PIN_S2, PIN_S3, PIN_OE, PIN_OUT, PIN_LED_R, PIN_LED_G,
         PIN_LED_B;
    int freq_scaling; // Default scaling is 20%

};
