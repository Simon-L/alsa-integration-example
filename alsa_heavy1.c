#include <stdio.h>
#include "heavy/Heavy_alsa_heavy1.h"
// #include "heavy/HvUtils.h"

int main(int argc, const char *argv[]) {
  double sampleRate = 44100.0;

  Hv_alsa_heavy1 *context = hv_alsa_heavy1_new(sampleRate);

  int numOutputChannels = hv_getNumOutputChannels(context);
  int numIterations = 100;
  int blockSize = 256; // ideally a multiple of 8

  float *outBuffers = (float *) hv_malloc(numOutputChannels * blockSize * sizeof(float));
  for (int i = 0; i < numIterations; ++i) {
    hv_alsa_heavy1_process_inline(context, NULL, outBuffers, blockSize);
    // do something with output here
  }

  hv_alsa_heavy1_free(context);
  return 0;
}
