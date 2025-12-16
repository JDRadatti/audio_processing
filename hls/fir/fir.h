/*
	Filename: fir.h
		Header file
*/
#ifndef FIR_H_
#define FIR_H_

#include "hls_stream.h"
#include "ap_int.h"

typedef ap_int<24> data_t;
typedef ap_int<16> coef_t;
typedef ap_int<48> acc_t;

struct transPkt {
    data_t data;
    ap_uint<1> last;
    ap_uint<8> keep;
    ap_uint<8> strb;
};

void fir (
    hls::stream<transPkt> &in_stream,
    hls::stream<transPkt> &out_stream
  );

#endif
