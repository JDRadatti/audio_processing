/*
	Filename: fir.h
		Header file
*/
#ifndef FIR_H_
#define FIR_H_

#include "hls_stream.h"
#include "ap_int.h"

const int N=128;

typedef ap_int<24> data_t;
typedef ap_int<16> coef_t;
typedef ap_int<48> acc_t;

void fir (
    hls::stream<data_t> &in_stream,
    hls::stream<data_t> &out_stream
  );

#endif
