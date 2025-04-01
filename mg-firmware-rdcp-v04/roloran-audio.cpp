#include "roloran-audio.h"

const char melody[] PROGMEM = "Oxygen:d=8,o=5,b=240:c6,p,g,d#,g.,p,2c,4p,4c6,p,g,d#,g.,p,2c,4p";
// from http://mines.lumpylumpy.com/Electronics/Computers/Software/Cpp/MFC/RingTones.RTTTL

AudioGeneratorRTTTL *rtttl;
AudioFileSourcePROGMEM *file;
AudioOutputI2S *out;

bool audio_play = true;

void set_audio_play(bool v)
{
    audio_play = v;
    return;
}

void audio_play_melody(void)
{
  if (!audio_play) return; 

  if (file) delete file;
  if (out) delete out;
  if (rtttl) delete rtttl;

  file = new AudioFileSourcePROGMEM( melody, strlen_P(melody) );
  out = new AudioOutputI2S(1, AudioOutputI2S::EXTERNAL_I2S);

  out->SetPinout(DAC_I2S_BCK, DAC_I2S_WS, DAC_I2S_DOUT, DAC_I2S_MCLK);
  out->SetGain(0.2);

  rtttl = new AudioGeneratorRTTTL();
  rtttl->begin(file, out);
  while (rtttl->loop()) {} 
  rtttl->stop();
}

/* EOF */