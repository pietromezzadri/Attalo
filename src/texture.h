#pragma once

class Texture
{
public:
    unsigned int texture, ID;
    int width, height, nr_channels;
    const char* filename;
    unsigned char* data;
    int glTexture;
    bool alpha = false;
};