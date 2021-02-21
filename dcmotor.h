#ifndef DCMOTOR_DCMOTOR_H_
#define DCMOTOR_DCMOTOR_H_
#endif

// The Driver class implements mapping with one, or two, N-segment interpolation curves.
// The second curve, if supplied, is applied depending on whether the secondary measurement supplied
//   to the driver is above or below the threshold. For example:
//   yourDriver.threshold = 0.5;
//   yourDriver.useSecondCurveBelowThreshold = true;
// 
// For a secondary value of 0.2, the second curve is applied. For a secondary value of 0.6,
//   The first curve is applied.
//


class Driver {
  
  /* Structure definition */
  struct table_1d {
    unsigned int x_length;
    float *x_values;
    float *y_values;
  };

  table_1d curve0, curve1;
  
  float interpolate_table_1d(struct table_1d *table, float x);
  float interpolate_segment(float x0, float y0, float x1, float y1, float x);
  
 public:
  float threshold = 0; //set to zero for no friction compensation
  bool useSecondCurveBelowThreshold = true; //set to false for second curve when secondary ABOVE threshold
  Driver(float *x, float *y, unsigned int size);
  void updatePrimaryCurve(float *x, float *y, unsigned int size);
  void addSecondCurve(float *x, float *y, unsigned int size);
  float drive(float primary, float secondary);
};

