//
//  Sound.h
//  Brother-Sister
//
//  Created by Jordan Cortes Guzman on 11/19/13.
//  Copyright (c) 2013 Ceci & Jordan. All rights reserved.
//

#ifndef __Sound__
#define __Sound__

#include <iostream>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

class Sound {
public:
    void *data;
    unsigned int bufferID;
    unsigned int sourceID;
    int format;
    int sampleRate;
    int size;
    
    Sound(int format, int sampleRate, void* data, int size);
    void play();
    void stop();
    void loop();
    static Sound* loadWave(const char* filename);
};

#endif /* defined(__Sound__) */
