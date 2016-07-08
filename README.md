# alsa-integration-example
An integration example of Heavy from EnzienAudio using Alsa.

GCC 5 is required because of `__has_include()` being used. (heavy/HvUtils.h line 264)

``` bash
$ make
$ ./alsa_heavy 44100 2 10
```

Generates a tone from the [osc~] object, receives a message to the `"freq"` identifier to change tone frequency.
