#ifndef COM_FILTER_H
#define COM_FILTER_H

typedef struct {

float LastP;
float Now_P;
float Q;
float R;
float Kg;
float Out;


}KalmanFiter_struct;

extern KalmanFiter_struct kfs[3];

int16_t CommonFilter_lowpass(int16_t new_data, int16_t last_data);
double KalmanFilter(KalmanFiter_struct* kf, double new_data);



#endif /* COM_FILTER_H */