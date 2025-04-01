#ifndef _ROLORAN_AUDIO
#define _ROLORAN_AUDIO

#include <Audio.h>
#include <driver/i2s.h>
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorRTTTL.h"
#include "AudioOutputI2S.h"

#define HAS_I2S
#define DAC_I2S_BCK 7
#define DAC_I2S_WS 5
#define DAC_I2S_DOUT 6
#define DAC_I2S_MCLK 21
#define SPK_I2S_PORT I2S_NUM_0

/**
 * Toggles whether sound is played (or disabled). 
 * @param v true if sound is enabled, false to disable sound
 */
void set_audio_play(bool v);

/**
 * Play the ringtone once.
 */
void audio_play_melody(void);

#endif 
/* EOF */