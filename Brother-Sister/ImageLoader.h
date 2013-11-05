//
//  ImageLoader.h
//  Brother-Sister
//
//  Created by Jordan Cortes Guzman on 11/4/13.
//  Copyright (c) 2013 Ceci & Jordan. All rights reserved.
//

#ifndef __ImageLoader__
#define __ImageLoader__

class Image {
public:
    Image(char* ps, int w, int h);
    ~Image();
    
    /* An array of the form (R1, G1, B1, R2, G2, B2, ...) indicating the
     * color of each pixel in image.  Color components range from 0 to 255.
     * The array starts the bottom-left pixel, then moves right to the end
     * of the row, then moves up to the next column, and so on.  This is the
     * format in which OpenGL likes images.
     */
    char* pixels;
    int width;
    int height;
};

//Reads a bitmap image from file.
Image* loadBMP(const char* filename);

#endif /* defined(__ImageLoader__) */
