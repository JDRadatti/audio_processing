/*
	Filename: fir_test.cpp
		Testbench file
		Calls fir() function from fir.cpp repeatedly
		Compares the output from fir() with out.gold.dat
*/

#include <stdio.h>
#include <stdlib.h>
#include "fir.h"
#include "hls_stream.h"

int main () {
  const int SAMPLES = 600;
  FILE *fp, *fin;

  hls::stream<data_t> in_stream;
  hls::stream<data_t> out_stream;

  fin = fopen("input.dat", "r");
  fp  = fopen("out.dat", "w");
  if (!fin || !fp) {
      printf("ERROR: Cannot open input/output file.\n");
      return 1;
  }

  // Process each input sample
  for (int i = 0; i < SAMPLES; i++) {
      int tmp;
      if (fscanf(fin, "%d", &tmp) != 1) {
          printf("ERROR: unexpected EOF in input.dat\n");
          fclose(fin);
          fclose(fp);
          return 1;
      }

      // Write input sample
      in_stream.write((data_t)tmp);

      // Call FIR filter once per sample
      fir(in_stream, out_stream);

      // Read output sample
      data_t output = out_stream.read();
      fprintf(fp, "%d\n", (int)output);
      printf("%3d out=%d\n", i, (int)output);
  }

  // Run the FIR Filter
  fir(in_stream, out_stream);

  // Retrieve results from output stream
  for (int i = 0; i < SAMPLES; i++) {
      if (!out_stream.empty()) {
          transPkt output = out_stream.read();
          fprintf(fp, "%d\n", (int)output.data);
          if (output.last) { break; }
      } else {
          printf("Warning: output stream empty at sample %d\n", i);
          break;
      }
  }

  fclose(fin);
  fclose(fp);

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
