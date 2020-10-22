#ifndef AUDIO_H
#define AUDIO_H
#include "rdll.h"

class daudio {
public:
    void playsnd(char* snz, float vol, int pitch);
    void playsnd3d(char* snz, float vol, int pitch, vector3d pos);
    void playsnd2d(char* snz, float vol, int pitch, vector2d pos);
};

#endif // AUDIO_H
