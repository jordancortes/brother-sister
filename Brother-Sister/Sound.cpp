//
//  Sound.cpp
//  Brother-Sister
//
//  Created by Jordan Cortes Guzman on 11/19/13.
//  Copyright (c) 2013 Ceci & Jordan. All rights reserved.
//

#include "Sound.h"

Sound::Sound(int format, int sampleRate, void* data, int size) {
    alGenBuffers(1, &bufferID);
    alBufferData(bufferID, format, data, size, sampleRate);
    alGenSources(1, &sourceID);
    alSourcei(sourceID, AL_BUFFER, bufferID);
    
    this->format = format;
    this->sampleRate = sampleRate;
    this->data = data;
    this->size = size;
}

void Sound::loop()
{
    alSourcei(sourceID, AL_LOOPING, AL_TRUE);
}

void Sound::play() {
    int state;
    alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING) {
        alSourcePlay(sourceID);
    }
}

void Sound::stop() {
    int state;
    alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) {
        alSourceStop(sourceID);
    }
}

Sound* Sound::loadWave(const char* filename) {
    FILE* fp = NULL;
    fp = fopen(filename, "r");
    if (!fp) {
        std::cout << "Could not open: " << filename << std::endl;
        fclose(fp);
        return NULL;
    }
    
    char* ChunkID = new char[4];
    fread(ChunkID, 4, sizeof(char), fp);
    
    if (strcmp(ChunkID, "RIFF")) {
        delete [] ChunkID;
        std::cout << "Not RIFF" << std::endl;
        fclose(fp);
        return NULL;
    }
    
    fseek(fp, 8, SEEK_SET);
    
    char* Format = new char[4];
    fread(Format, 4, sizeof(char), fp);
    
    if (strcmp(Format, "WAVE")) {
        delete [] ChunkID;
        delete [] Format;
        std::cout << "Not WAVE" << std::endl;
        fclose(fp);
        return NULL;
    }
    
    char* SubChunk1ID = new char[4];
    fread(SubChunk1ID, 4, sizeof(char), fp);
    
    if (strcmp(SubChunk1ID, "fmt ")) {
        delete [] ChunkID;
        delete [] Format;
        delete [] SubChunk1ID;
        std::cout << "Corrupt!: " << SubChunk1ID << std::endl;
        fclose(fp);
        return NULL;
    }
    
    unsigned int SubChunk1Size;
    fread(&SubChunk1Size, 1, sizeof(unsigned int), fp);
    
    unsigned int SubChunck2Location = (unsigned int)ftell(fp) + SubChunk1Size;
    
    unsigned short AudioFormat;
    fread(&AudioFormat, 1, sizeof(unsigned short), fp);
    
    if (AudioFormat != 1) {
        delete [] ChunkID;
        delete [] Format;
        delete [] SubChunk1ID;
        std::cout << "Audio is not PCM!" << std::endl;
        fclose(fp);
        return NULL;
    }
    
    unsigned short NumChannels;
    fread(&NumChannels, 1, sizeof(unsigned short), fp);
    unsigned int sampleRate;
    
    fread(&sampleRate, 1, sizeof(unsigned int), fp);
    
    fseek(fp, 34, SEEK_SET);
    
    unsigned short bitsPerSample;
    fread(&bitsPerSample, 1, sizeof(unsigned short), fp);
    
    int ALFormat;
    if (NumChannels == 1 && bitsPerSample == 8) {
        ALFormat = AL_FORMAT_MONO8;
    }
    else if (NumChannels == 1 && bitsPerSample == 16) {
        ALFormat = AL_FORMAT_MONO16;
    }
    else if (NumChannels == 2 && bitsPerSample == 8) {
        ALFormat = AL_FORMAT_STEREO8;
    }
    else if (NumChannels == 2 && bitsPerSample == 16) {
        ALFormat = AL_FORMAT_STEREO16;
    }
    else {
        delete [] ChunkID;
        delete [] Format;
        delete [] SubChunk1ID;
        std::cout << "Audio es not correctly formatted!" << std::endl;
        fclose(fp);
        return NULL;
    }
    
    fseek(fp, SubChunck2Location, SEEK_SET);
    char *subChunk2ID = new char[4];
    fread(subChunk2ID, 4, sizeof(char), fp);
    
    if (strcmp(subChunk2ID, "data")) {
        delete [] ChunkID;
        delete [] Format;
        delete [] SubChunk1ID;
        delete [] subChunk2ID;
        std::cout << "corrupt! " << subChunk2ID << std::endl;
        fclose(fp);
        return NULL;
    }
    
    unsigned int subChunk2Size;
    fread(&subChunk2Size, 1, sizeof(unsigned int), fp);
    
    unsigned char* data = new unsigned char[subChunk2Size];
    fread(data, subChunk2Size, sizeof(unsigned char), fp);
    
    delete [] ChunkID;
    delete [] Format;
    delete [] SubChunk1ID;
    delete [] subChunk2ID;
    fclose(fp);
    
    return new Sound(ALFormat, sampleRate, data, subChunk2Size);
}