/*
	Filename: fir.cpp
	INPUT:
		in_stream: hls::stream<data_t>

	OUTPUT:
		out_stream: hls::stream<data_t>

*/

#include "fir.h"
#include "fir_coeffs.h"

void fir(hls::stream<transPkt> &in_stream, hls::stream<transPkt> &out_stream) {

#pragma HLS INTERFACE axis port=in_stream
#pragma HLS INTERFACE axis port=out_stream

    static
		data_t shift_reg[N];
		acc_t acc;
		int i;

    while (!in_stream.empty()) {
        transPkt in_pkt = in_stream.read();

        TDL:
        for (int i = N - 1; i > 0; i--) {
            shift_reg[i] = shift_reg[i - 1];
        }
        shift_reg[0] = in_pkt.data;

        MAC:
        acc = 0;
        for (int i = 0; i < N; i++) {
            acc += shift_reg[i] * coeffs[i];
        }

        // Normalize output to 24 bits
        data_t y = (data_t)(acc >> 10);

        // Prepare output packet
        transPkt out_pkt;
        out_pkt.data = y;
        out_pkt.keep = in_pkt.keep;
        out_pkt.strb = in_pkt.strb;
        out_pkt.last = in_pkt.last;

        // Send result
        out_stream.write(out_pkt);

        if (in_pkt.last)
            break;
    }
}
