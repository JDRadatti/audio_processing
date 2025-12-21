/*
	Filename: fir.cpp
	INPUT:
		in_stream: hls::stream<data_t>

	OUTPUT:
		out_stream: hls::stream<data_t>

*/

#include "fir.h"
#include "fir_coeffs.h"

void fir(hls::stream<data_t> &in_stream, hls::stream<data_t> &out_stream) {

#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL

    static
		data_t shift_reg[N];
		acc_t acc;
		int i;

    data_t x = in_stream.read();

    TDL:
    for (i = N - 1; i > 0; i--) {
        shift_reg[i] = shift_reg[i - 1];
    }
    shift_reg[0] = x;

    MAC:
    acc = 0;
    for (i = 0; i < N; i++) {
        acc += shift_reg[i] * coeffs[i];
    }

    // Normalize output to 24 bits
    data_t y = (data_t)(acc >> 10);

    out_stream.write(y);

}
