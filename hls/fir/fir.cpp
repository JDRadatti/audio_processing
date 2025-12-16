/*
	Filename: fir.cpp
	INPUT:
		in_stream: hls::stream<data_t>

	OUTPUT:
		out_stream: hls::stream<data_t>

*/

#include "fir.h"

void fir(hls::stream<data_t> &in_stream, hls::stream<data_t> &out_stream) {

#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    coef_t c[N] = {10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};

    static
		data_t shift_reg[N];
		acc_t acc;
		int i;

    if (!in_stream.empty()) {
        data_t x = in_stream.read();

        TDL:
        for (int i = N - 1; i > 0; i--) {
            shift_reg[i] = shift_reg[i - 1];
        }
        shift_reg[0] = x;

        MAC:
        acc = 0;
        for (int i = 0; i < N; i++) {
            acc += shift_reg[i] * c[i];
        }

        out_stream.write((data_t)(acc >> 10));
    }
}
