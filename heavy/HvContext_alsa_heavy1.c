/**
 * Copyright (c) 2016 Enzien Audio, Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, and/or
 * sublicense copies of the Software, strictly on a non-commercial basis,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * DO NOT MODIFY. THIS CODE IS MACHINE GENERATED BY THE ENZIEN AUDIO HEAVY COMPILER.
 */

#include "HvContext_alsa_heavy1.h"
#include "HvMath.h"



/*
 * Function Declarations
 */
static void cReceive_EqtGI_sendMessage(HvBase *, int, const HvMessage *const);



/*
 * Static Helper Functions
 */

static void ctx_intern_scheduleMessageForReceiver(
    HvBase *const _c, hv_uint32_t receiverHash, HvMessage *m) {
  switch (receiverHash) {
    case 0x345FC008: { // freq
      ctx_scheduleMessage(_c, m, &cReceive_EqtGI_sendMessage, 0);
      break;
    }
    default: return;
  }
}

static struct HvTable *ctx_intern_getTableForHash(HvBase *const _c, hv_uint32_t h) {
  switch (h) {
    default: return NULL;
  }
}



/*
 * Context Include and Implementatons
 */

#ifdef __cplusplus
extern "C" {
#endif
HV_EXPORT Hv_alsa_heavy1 *hv_alsa_heavy1_new_with_options(double sampleRate, int poolKb, int queueKb) {
  hv_assert(sampleRate > 0.0); // can't initialise with sampling rate of 0
  hv_assert(poolKb >= 1); // a message pool of some reasonable size is always needed
  Hv_alsa_heavy1 *const _c = (Hv_alsa_heavy1 *) hv_malloc(sizeof(Hv_alsa_heavy1));
  hv_assert(_c != NULL);

  Base(_c)->numInputChannels = 0;
  Base(_c)->numOutputChannels = 2;
  Base(_c)->sampleRate = sampleRate;
  Base(_c)->blockStartTimestamp = 0;
  Base(_c)->f_scheduleMessageForReceiver = &ctx_intern_scheduleMessageForReceiver;
  Base(_c)->f_getTableForHash = &ctx_intern_getTableForHash;
  Base(_c)->basePath = NULL;
  Base(_c)->printHook = NULL;
  Base(_c)->sendHook = NULL;
  Base(_c)->userData = NULL;
  Base(_c)->name = "alsa_heavy1";

  Base(_c)->numBytes = sizeof(Hv_alsa_heavy1);

  Base(_c)->numBytes += mq_initWithPoolSize(&Base(_c)->mq, poolKb);
  Base(_c)->numBytes += hLp_init(&Base(_c)->msgPipe, queueKb*1024);
  HV_SPINLOCK_RELEASE(Base(_c)->msgLock);
  Base(_c)->numBytes += sPhasor_k_init(&_c->sPhasor_Z1cuA, 440.0f, sampleRate);

  // loadbang

  return _c;
}

HV_EXPORT Hv_alsa_heavy1 *hv_alsa_heavy1_new(double sampleRate) {
  // default to 10KB MessagePool
  // 1KB message queue size
  return hv_alsa_heavy1_new_with_options(sampleRate, 10, 2);
}

HV_EXPORT void hv_alsa_heavy1_free(Hv_alsa_heavy1 *_c) {

  hv_free(Base(_c)->basePath);
  mq_free(&Base(_c)->mq); // free queue after all objects have been freed, messages may be cancelled
  hLp_free(&Base(_c)->msgPipe);

  hv_free(_c);
}
#ifdef __cplusplus
}
#endif



/*
 * Static Function Implementation
 */
static void cReceive_EqtGI_sendMessage(HvBase *_c, int letIn, const HvMessage *const m) {
  sPhasor_k_onMessage(_c, &Context(_c)->sPhasor_Z1cuA, 0, m);
}




/*
 * Context Process Implementation
 */

#ifdef __cplusplus
extern "C" {
#endif
HV_EXPORT int hv_alsa_heavy1_process(Hv_alsa_heavy1 *const _c, float **const inputBuffers, float **const outputBuffers, int nx) {
  while (hLp_hasData(&Base(_c)->msgPipe)) {
    hv_uint32_t numBytes = 0;
    ReceiverMessagePair *p = (ReceiverMessagePair *) hLp_getReadBuffer(&Base(_c)->msgPipe, &numBytes);
    hv_assert(numBytes >= sizeof(ReceiverMessagePair));
    ctx_scheduleMessageForReceiver(Base(_c), p->receiverHash, &p->msg);
    hLp_consume(&Base(_c)->msgPipe);
  }
  const int n4 = nx & ~HV_N_SIMD_MASK; // ensure that the block size is a multiple of HV_N_SIMD

  // temporary signal vars
  hv_bufferf_t Bf0, Bf1, Bf2, Bf3, Bf4;

  // input and output vars
  hv_bufferf_t O0, O1;

  // declare and init the zero buffer
  hv_bufferf_t ZERO; __hv_zero_f(VOf(ZERO));

  hv_uint32_t nextBlock = Base(_c)->blockStartTimestamp;
  for (int n = 0; n < n4; n += HV_N_SIMD) {

    // process all of the messages for this block
    nextBlock += HV_N_SIMD;
    while (mq_hasMessageBefore(&Base(_c)->mq, nextBlock)) {
      MessageNode *const node = mq_peek(&Base(_c)->mq);
      node->sendMessage(Base(_c), node->let, node->m);
      mq_pop(&Base(_c)->mq);
    }

    

    // zero output buffers
    __hv_zero_f(VOf(O0));
    __hv_zero_f(VOf(O1));

    // process all signal functions
    __hv_phasor_k_f(&_c->sPhasor_Z1cuA, VOf(Bf0));
    __hv_var_k_f(VOf(Bf1), 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f);
    __hv_sub_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_abs_f(VIf(Bf1), VOf(Bf1));
    __hv_var_k_f(VOf(Bf0), 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f);
    __hv_sub_f(VIf(Bf1), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf1), 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f, 6.28319f);
    __hv_mul_f(VIf(Bf0), VIf(Bf1), VOf(Bf1));
    __hv_mul_f(VIf(Bf1), VIf(Bf1), VOf(Bf0));
    __hv_mul_f(VIf(Bf1), VIf(Bf0), VOf(Bf2));
    __hv_mul_f(VIf(Bf2), VIf(Bf0), VOf(Bf0));
    __hv_var_k_f(VOf(Bf3), 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f, 0.00784314f);
    __hv_var_k_f(VOf(Bf4), 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f, 0.166667f);
    __hv_mul_f(VIf(Bf2), VIf(Bf4), VOf(Bf4));
    __hv_sub_f(VIf(Bf1), VIf(Bf4), VOf(Bf4));
    __hv_fma_f(VIf(Bf0), VIf(Bf3), VIf(Bf4), VOf(Bf4));
    __hv_var_k_f(VOf(Bf3), 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f);
    __hv_mul_f(VIf(Bf4), VIf(Bf3), VOf(Bf3));
    __hv_add_f(VIf(Bf3), VIf(O1), VOf(O1));
    __hv_add_f(VIf(Bf3), VIf(O0), VOf(O0));

    // save output vars to output buffer
    __hv_store_f(outputBuffers[0]+n, VIf(O0));
    __hv_store_f(outputBuffers[1]+n, VIf(O1));
  }

  Base(_c)->blockStartTimestamp = nextBlock;

  return n4; // return the number of frames processed
}

HV_EXPORT int hv_alsa_heavy1_process_inline(Hv_alsa_heavy1 *c, float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(c != NULL);
  hv_assert(!(n4 & HV_N_SIMD_MASK)); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s)
  float **const bIn = NULL;

  // define the heavy output buffer for 2 channel(s)
  float **const bOut = (float **) hv_alloca(2*sizeof(float *));
  bOut[0] = outputBuffers+(0*n4);
  bOut[1] = outputBuffers+(1*n4);

  int n = hv_alsa_heavy1_process(c, bIn, bOut, n4);
  return n;
}

HV_EXPORT int hv_alsa_heavy1_process_interleaved(Hv_alsa_heavy1 *c, float *inputBuffers, float *outputBuffers, int n4) {
  hv_assert(c != NULL);
  hv_assert(!(n4 & HV_N_SIMD_MASK)); // ensure that n4 is a multiple of HV_N_SIMD

  // define the heavy input buffer for 0 channel(s), uninterleave
  float *const bIn = NULL;

  // define the heavy output buffer for 2 channel(s)
  float *const bOut = (float *) hv_alloca(2*n4*sizeof(float));

  int n = hv_alsa_heavy1_process_inline(c, bIn, bOut, n4);

  // interleave the heavy output into the output buffer
  #if HV_SIMD_SSE
  for (int i = 0, j = 0; j < n4; j += 4, i += 8) {
    __m128 x = _mm_load_ps(bOut+j);                        // LLLL
    __m128 y = _mm_load_ps(bOut+n4+j);                     // RRRR
    __m128 a = _mm_shuffle_ps(x, y, _MM_SHUFFLE(1,0,1,0)); // LLRR
    __m128 b = _mm_shuffle_ps(x, y, _MM_SHUFFLE(3,2,3,2)); // LLRR
    a = _mm_shuffle_ps(a, a, _MM_SHUFFLE(3,1,2,0));        // LRLR
    b = _mm_shuffle_ps(b, b, _MM_SHUFFLE(3,1,2,0));        // LRLR
    _mm_stream_ps(outputBuffers+i, a);
    _mm_stream_ps(outputBuffers+4+i, b);
  }
  _mm_sfence();
  #elif HV_SIMD_NEON
  // https://community.arm.com/groups/processors/blog/2012/03/13/coding-for-neon--part-5-rearranging-vectors
  for (int i = 0, j = 0; j < n4; j += 4, i += 8) {
    float32x4_t x = vld1q_f32(bOut+j);
    float32x4_t y = vld1q_f32(bOut+n4+j);
    float32x4x2_t z = {x, y};
    vst2q_f32(outputBuffers+i, z); // interleave and store
  }
  #else // HV_SIMD_NONE
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < n4; ++j) {
      outputBuffers[i+2*j] = bOut[i*n4+j];
    }
  }
  #endif

  return n;
}

HV_EXPORT int hv_alsa_heavy1_process_interleaved_short(Hv_alsa_heavy1 *c, short *inputBuffers, short *outputBuffers, int n4) {
  hv_assert(!(n4 & HV_N_SIMD_MASK)); // ensure that n4 is a multiple of HV_N_SIMD
  int numChannels = ctx_getNumInputChannels(Base(c));
  float *bIn = (float *) hv_alloca(numChannels*n4*sizeof(float));
  for (int i = 0; i < numChannels; ++i) {
    for (int j = 0; j < n4; ++j) {
      bIn[i*n4+j] = ((float) inputBuffers[i+numChannels*j]) * 0.00003051757813f;
    }
  }

  numChannels = ctx_getNumOutputChannels(Base(c));
  float *bOut = (float *) hv_alloca(numChannels*n4*sizeof(float));

  int n = hv_alsa_heavy1_process_inline(c, bIn, bOut, n4);

  for (int i = 0; i < numChannels; ++i) {
    for (int j = 0; j < n4; ++j) {
      outputBuffers[i+numChannels*j] = (short) (bOut[i*n4+j] * 32767.0f);
    }
  }

  return n;
}
#ifdef __cplusplus
}
#endif
