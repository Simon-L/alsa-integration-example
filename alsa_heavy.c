//
//  alsa_heavy.c
//  HeavyAlsa
//
//  Created by Simon Larcher on 07/08/2016
//  Copyright (c) 2016 Simon Larcher. All rights reserved.
//

/*
 * This is an example program using Heavy and Alsa that processes a very simple Pd patch.
 * The patch simply generates a tone from the [osc~] object and can receive messages to "freq"
 * to change the frequency of the tone.
 *
 * Heavily (!!) inspired from the Heavy documentation (https://enzienaudio.com/docs/index.html#8.c#processing-samples)
 * and from an Alsa playback example by user ghedo on Github: https://gist.github.com/ghedo/963382
 *
 * Expected data I/O format here are interleaved short values.
 */


#include <stdio.h>
#include "heavy/Heavy_alsa_heavy1.h"
#include "heavy/HvUtils.h"

#include <alsa/asoundlib.h>

#define PCM_DEVICE "default"
#define BLOCKSIZE 128

int main(int argc, const char *argv[]) {

  // #### Heavy
  double sampleRate = 44100.0;

  Hv_alsa_heavy1 *context = hv_alsa_heavy1_new(sampleRate);

  int numOutputChannels = hv_getNumOutputChannels(context);
  int numIterations = 100;
  int blockSize = BLOCKSIZE; // ideally a multiple of 8

  short *outBuffers = (short *) hv_malloc(numOutputChannels * blockSize * sizeof(short));
  // #### Heavy

  // #### Alsa
  unsigned int pcm, tmp, dir;
  int rate, channels, seconds;
  snd_pcm_t *pcm_handle;
  snd_pcm_hw_params_t *params;
  snd_pcm_uframes_t frames;
  char *buff;
  int buff_size, loops;

  if (argc < 4) {
    printf("Usage: %s <sample_rate> <channels> <seconds>\n",
                argv[0]);
    return -1;
  }

  rate 	 = atoi(argv[1]);
  channels = atoi(argv[2]);
  seconds  = atoi(argv[3]);

  /* Open the PCM device in playback mode */
  if (pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
          SND_PCM_STREAM_PLAYBACK, 0) < 0)
    printf("ERROR: Can't open \"%s\" PCM device. %s\n",
          PCM_DEVICE, snd_strerror(pcm));

  /* Allocate parameters object and fill it with default values*/
  snd_pcm_hw_params_alloca(&params);

  snd_pcm_hw_params_any(pcm_handle, params);

  /* Set parameters */
  if (pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
          SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
    printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

  if (pcm = snd_pcm_hw_params_set_period_size(pcm_handle, params, BLOCKSIZE, 0) < 0)
    printf("ERROR: Can't set period size. %s\n", snd_strerror(pcm));

  if (pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
            SND_PCM_FORMAT_S16_LE) < 0)
    printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

  if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, channels) < 0)
    printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

  if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0) < 0)
    printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

  /* Write parameters */
  if (pcm = snd_pcm_hw_params(pcm_handle, params) < 0)
    printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

  /* Resume information */
  printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));

  printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

  snd_pcm_hw_params_get_channels(params, &tmp);
  printf("channels: %i ", tmp);

  if (tmp == 1)
    printf("(mono)\n");
  else if (tmp == 2)
    printf("(stereo)\n");

  snd_pcm_hw_params_get_rate(params, &tmp, 0);
  printf("rate: %d bps\n", tmp);

  printf("seconds: %d\n", seconds);

  snd_pcm_hw_params_get_period_size(params, &frames, 0);
  printf("Period size: %u\n", frames);

  /* Allocate buffer to hold single period */

  buff_size = frames * channels * 2 /* 2 -> sample size */;
  buff = (char *) malloc(buff_size);

  snd_pcm_hw_params_get_period_time(params, &tmp, NULL);
  printf("Period time: %uµs\n", tmp);
  // #### Alsa

  // Audio processing loop starts here
  for (loops = (seconds * 1000000) / tmp; loops > 0; loops--) {

    // Use the short (int16) interleaved version of process() to match the requirements of snd_pcm_writei()
    hv_alsa_heavy1_process_interleaved_short(context, NULL, outBuffers, blockSize);

    // Optionally uncomment this line to send a message to change frequency
    // if (loops == ((seconds * 1000000) / tmp)/2 ) hv_sendFloatToReceiver(context, HV_ALSA_HEAVY1_PARAM_FREQ, 880.0f);

    if (pcm = snd_pcm_writei(pcm_handle, outBuffers, frames) == -EPIPE) {
      printf("XRUN.\n");
      snd_pcm_prepare(pcm_handle);
    } else if (pcm < 0) {
      printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
    }
  }

  hv_alsa_heavy1_free(context);
  snd_pcm_drain(pcm_handle);
  snd_pcm_close(pcm_handle);
  free(buff);

  return 0;
}
