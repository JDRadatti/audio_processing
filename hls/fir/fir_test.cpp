/*
	Filename: fir_test.h
		Testbench file
		Calls fir() function from fir.cpp
		Compares the output from fir() with out.gold.dat
*/

#include <stdio.h>
#include <stdlib.h>
#include "fir.h"
#include "hls_stream.h"

int main () {
  const int    SAMPLES=600;
  FILE         *fp, *fin;

  hls::stream<data_t> in_stream;
  hls::stream<data_t> out_stream;

  data_t signal, output;
  int i;

  fin = fopen("input.dat", "r");
  fp  = fopen("out.dat", "w");

  // Feed all input samples into the stream
  for (i = 0; i < SAMPLES; i++) {
      int tmp;
      fscanf(fin, "%d", &tmp);
      signal = tmp;
      in_stream.write(signal);
  }

  // Run the FIR Filter
  fir(in_stream, out_stream);

  // Retrieve results from output stream
  for (i = 0; i < SAMPLES; i++) {
      if (!out_stream.empty()) {
          output = out_stream.read();
          fprintf(fp, "%d\n", (int)output);
          printf("%3d  in=%d  out=%d\n", i, (int)signal, (int)output);
      } else {
          printf("Warning: output stream empty at sample %d\n", i);
          break;
      }
  }

  fclose(fp);
  fclose(fin);

  // Compare results with golden output
  printf("Comparing against golden output...\n");
  if (system("diff -w out.dat out.gold.dat")) {
      fprintf(stdout, "*******************************************\n");
      fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
      fprintf(stdout, "*******************************************\n");
      return 1;
  } else {
      fprintf(stdout, "*******************************************\n");
      fprintf(stdout, "PASS: Output matches the golden output!\n");
      fprintf(stdout, "*******************************************\n");
      return 0;
  }

}
