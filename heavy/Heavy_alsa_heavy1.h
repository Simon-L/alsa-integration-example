/**
 * Copyright (c) 2014,2015,2016 Enzien Audio, Ltd.
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

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * For more information find the C API documentation at:
 * https://enzienaudio.com/docs/c.html
 */

#if !_MSC_VER
#pragma mark - Heavy Context
#endif

#ifndef _HEAVY_ALSA_HEAVY1_H_
#define _HEAVY_ALSA_HEAVY1_H_


enum {
  HV_ALSA_HEAVY1_PARAM_FREQ = 0x345FC008, // freq
};

typedef struct Hv_alsa_heavy1 Hv_alsa_heavy1;

/**
 * Creates a new patch instance.
 * Sample rate should be positive and in Hertz, e.g. 44100.0.
 */
Hv_alsa_heavy1 *hv_alsa_heavy1_new(double sampleRate);

/**
 * Creates a new patch instance.
 * @param sampleRate  Sample rate should be positive (> 0) and in Hertz, e.g. 48000.0.
 * @param poolKb  Pool size is in kilobytes, and determines the maximum amount of memory
 *   allocated to messages at any time. By default this is 10 KB.
 * @param queueKb  The size of the message pool in kilobytes. It determined the
 *   amount of memory dedicated to holding scheduled messages between calls to
 *   process(). Default is 2 KB.
 */
Hv_alsa_heavy1 *hv_alsa_heavy1_new_with_options(double sampleRate, int poolKb, int queueKb);

/** Frees a patch instance. */
void hv_alsa_heavy1_free(Hv_alsa_heavy1 *c);

/**
 * Processes one block of samples for a patch instance. The buffer format is an array of float channel arrays.
 * If the context has not input or output channels, the respective argument may be NULL.
 * The number of samples to to tbe processed should be a multiple of 1, 4, or 8, depending on if
 * no, SSE or NEON, or AVX optimisation is being used, respectively.
 * e.g. [[LLLL][RRRR]]
 *
 * @return  The number of samples processed.
 *
 * This function is NOT thread-safe. It is assumed that only the audio thread will execute this function.
 */
int hv_alsa_heavy1_process(Hv_alsa_heavy1 *const c, float **const inputBuffers, float **const outputBuffers, int n4);

/**
 * Processes one block of samples for a patch instance. The buffer format is an uninterleaved float array of channels.
 * If the context has not input or output channels, the respective argument may be NULL.
 * The number of samples to to tbe processed should be a multiple of 1, 4, or 8, depending on if
 * no, SSE or NEON, or AVX optimisation is being used, respectively.
 * e.g. [LLLLRRRR]
 *
 * @return  The number of samples processed.
 *
 * This function is NOT thread-safe. It is assumed that only the audio thread will execute this function.
 */
int hv_alsa_heavy1_process_inline(Hv_alsa_heavy1 *c, float *inputBuffers, float *outputBuffers, int n4);

/**
 * Processes one block of samples for a patch instance. The buffer format is an interleaved float array of channels.
 * If the context has not input or output channels, the respective argument may be NULL.
 * The number of samples to to tbe processed should be a multiple of 1, 4, or 8, depending on if
 * no, SSE or NEON, or AVX optimisation is being used, respectively.
 * e.g. [LRLRLRLR]
 *
 * @return  The number of samples processed.
 *
 * This function is NOT thread-safe. It is assumed that only the audio thread will execute this function.
 */
int hv_alsa_heavy1_process_interleaved(Hv_alsa_heavy1 *c, float *inputBuffers, float *outputBuffers, int n4);

/**
 * Processes one block of samples for a patch instance. The buffer format is an interleaved short array of channels.
 * If the context has not input or output channels, the respective argument may be NULL.
 * The number of samples to to tbe processed should be a multiple of 1, 4, or 8, depending on if
 * no, SSE or NEON, or AVX optimisation is being used, respectively.
 * e.g. [LRLRLRLR]
 *
 * @return  The number of samples processed.
 *
 * This function is NOT thread-safe. It is assumed that only the audio thread will execute this function.
 */
int hv_alsa_heavy1_process_interleaved_short(Hv_alsa_heavy1 *c, short *inputBuffers, short *outputBuffers, int n4);

#endif // _HEAVY_ALSA_HEAVY1_H_



#if !_MSC_VER
#pragma mark - Heavy Common
#endif

#ifndef _HEAVY_COMMON_H_
#define _HEAVY_COMMON_H_

typedef void Heavy;
typedef struct HvMessage HvMessage;

/**
 * Returns the total size in bytes of the context.
 * This value may change if tables are resized.
 */
int hv_getSize(Heavy *c);

/** Returns the sample rate with which this patch has been configured. */
double hv_getSampleRate(Heavy *c);

/** Returns the number of input channels with which this patch has been configured. */
int hv_getNumInputChannels(Heavy *c);

/** Returns the number of output channels with which this patch has been configured. */
int hv_getNumOutputChannels(Heavy *c);

/** Set the print hook. The function is called whenever a message is sent to a print object. */
void hv_setPrintHook(Heavy *c,
   void (*f)(double timestampMs, const char *printName, const char *message, void *userData));

/**
 * Set the send hook. The function is called whenever a message is sent to any send object.
 * Messages returned by this function should NEVER be freed. If the message must persist, call
 * hv_msg_copy() first.
 */
void hv_setSendHook(Heavy *c, void (*f)(double timestampMs, const char *receiverName, const HvMessage *const m, void *userData));

/** Returns a 32-bit hash of any string. Returns 0 if string is NULL. */
unsigned int hv_stringToHash(const char *s);

/**
 * A convenience function to send a bang to a receiver to be processed immediately.
 * The receiver is addressed with its hash, which can also be determined using hv_stringToHash().
 * This function is thread-safe.
 *
 * @return  True if the message was accepted. False if the message could not fit onto
 *           the message queue to be processed this block.
 */
bool hv_sendBangToReceiver(Heavy *c, unsigned int receiverHash);

/**
 * A convenience function to send a float to a receiver to be processed immediately.
 * The receiver is addressed with its hash, which can also be determined using hv_stringToHash().
 * This function is thread-safe.
 *
 * @return  True if the message was accepted. False if the message could not fit onto
 *           the message queue to be processed this block.
 */
bool hv_sendFloatToReceiver(Heavy *c, unsigned int receiverHash, const float x);

/**
 * A convenience function to send a symbol to a receiver to be processed immediately.
 * The receiver is addressed with its hash, which can also be determined using hv_stringToHash().
 * This function is thread-safe.
 *
 * @return  True if the message was accepted. False if the message could not fit onto
 *           the message queue to be processed this block.
 */
bool hv_sendSymbolToReceiver(Heavy *c, unsigned int receiverHash, char *s);

/**
 * Sends a formatted message to a receiver that can be scheduled for the future.
 * The receiver is addressed with its hash, which can also be determined using hv_stringToHash().
 * This function is thread-safe.
 *
 * @return  True if the message was accepted. False if the message could not fit onto
 *           the message queue to be processed this block.
 */
bool hv_vscheduleMessageForReceiver(Heavy *c, unsigned int receiverHash, double delayMs, const char *format, ...);

/**
 * Sends a message to a receiver that can be scheduled for the future.
 * The receiver is addressed with its hash, which can also be determined using hv_stringToHash().
 * This function is thread-safe.
 *
 * @return  True if the message was accepted. False if the message could not fit onto
 *           the message queue to be processed this block.
 */
bool hv_scheduleMessageForReceiver(Heavy *c, unsigned int receiverHash, double delayMs, HvMessage *m);

/** Cancels a previously scheduled message. */
void hv_cancelMessage(Heavy *c, HvMessage *m);

/** Returns a table object given its name. NULL if no table with that name exists. */
struct HvTable *hv_getTableForName(Heavy *c, const char *tableName);

/** Returns the current patch time in milliseconds. This value may have rounding errors. */
double hv_getCurrentTime(Heavy *c);

/** Returns the current patch time in samples. This value is always exact. */
unsigned int hv_getCurrentSample(Heavy *c);

/** Sets a user-definable value. This value is never manipulated by Heavy. */
void hv_setUserData(Heavy *c, void *userData);

/** Returns the user-defined data. */
void *hv_getUserData(Heavy *c);

/** Define the base path of the patch. Used as the root path to locate assets. */
void hv_setBasePath(Heavy *c, const char *basePath);

/** Returns the read-only user-assigned name of this patch. */
const char *hv_getName(Heavy *c);

#endif // _HEAVY_COMMON_H_



#if !_MSC_VER
#pragma mark - Heavy Message
#endif

#ifndef _HEAVY_MESSAGE_H_
#define _HEAVY_MESSAGE_H_

typedef struct HvMessage HvMessage;

/** Returns the total size in bytes of a HvMessage with a number of elements on the heap. */
unsigned int hv_msg_getByteSize(int numElements);

/** Initialise a HvMessage structure with the number of elements and a timestamp (in samples). */
void hv_msg_init(HvMessage *m, int numElements, unsigned int timestamp);

/** Returns the number of elements in this message. */
int hv_msg_getNumElements(const HvMessage *const m);

/** Returns the time at which this message exists (in samples). */
unsigned int hv_msg_getTimestamp(const HvMessage *const m);

/** Set the time at which this message should be executed (in samples). */
void hv_msg_setTimestamp(HvMessage *m, unsigned int timestamp);

/** Returns true of the indexed element is a bang. False otherwise. Index is not bounds checked. */
bool hv_msg_isBang(const HvMessage *const m, int i);

/** Sets the indexed element to a bang. Index is not bounds checked. */
void hv_msg_setBang(HvMessage *m, int i);

/** Returns true of the indexed element is a float. False otherwise. Index is not bounds checked. */
bool hv_msg_isFloat(const HvMessage *const m, int i);

/** Returns the indexed element as a float value. Index is not bounds checked. */
float hv_msg_getFloat(const HvMessage *const m, int i);

/** Sets the indexed element to float value. Index is not bounds checked. */
void hv_msg_setFloat(HvMessage *m, int i, float f);

/** Returns true of the indexed element is a symbol. False otherwise. Index is not bounds checked. */
bool hv_msg_isSymbol(const HvMessage *const m, int i);

/** Returns the indexed element as a symbol value. Index is not bounds checked. */
char *hv_msg_getSymbol(const HvMessage *const m, int i);

/** Returns true of the indexed element is a hash. False otherwise. Index is not bounds checked. */
bool hv_msg_isHash(const HvMessage *const m, int i);

/** Returns the indexed element as a hash value. Index is not bounds checked. */
unsigned int hv_msg_getHash(const HvMessage *const m, int i);

/** Sets the indexed element to symbol value. Index is not bounds checked. */
void hv_msg_setSymbol(HvMessage *m, int i, const char *s);

/**
 * Returns true if the message has the given format, in number of elements and type. False otherwise.
 * Valid element types are:
 * 'b': bang
 * 'f': float
 * 's': symbol
 *
 * For example, a message with three floats would have a format of "fff". A single bang is "b".
 * A message with two symbols is "ss". These types can be mixed and matched in any way.
 */
bool hv_msg_hasFormat(const HvMessage *const m, const char *fmt);

/**
 * Returns a basic string representation of the message.
 * The character array MUST be deallocated by the caller.
 */
char *hv_msg_toString(const HvMessage *const m);

/** Copy a message onto the stack. The message persists. */
HvMessage *hv_msg_copy(const HvMessage *const m);

/** Free a copied message. */
void hv_msg_free(HvMessage *m);

#endif // _HEAVY_MESSAGE_H_



#if !_MSC_VER
#pragma mark - Heavy Table
#endif

#ifndef _HEAVY_TABLE_H_
#define _HEAVY_TABLE_H_

typedef struct HvTable HvTable;

/**
 * Resizes the table to the given length. Length must be positive.
 * Existing contents are copied to the new table. Remaining space is cleared.
 * The change in byte-size of the table is returned. A value of zero indicates error.
 */
int hv_table_resize(HvTable *o, unsigned int newLength);

/** Returns a pointer to the raw buffer backing this table. DO NOT free it. */
float *hv_table_getBuffer(HvTable *o);

/** Returns the length of this table in samples. */
unsigned int hv_table_getLength(HvTable *o);

#endif // _HEAVY_TABLE_H_

#ifdef __cplusplus
} // extern "C"
#endif
