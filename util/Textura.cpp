/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*
 * ---------------- www.spacesimulator.net --------------
 *   ---- Space simulators and 3d engine tutorials ----
 *
 * (c) Copyright 2002 Damiano Vitulli <info@spacesimulator.net>
 *
 * ALL RIGHTS RESERVED
 *
 *   Modificado por Guilherme Sampaio e Frederico Sampaio
 *
 * Tutorial 3: 3d engine - Texture mapping with OpenGL!
 *
 * Include File: texture.cpp
 *
 */

/*
Linux port by Panteleakis Ioannis
mail: pioann@csd.auth.gr

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
//#include <GL/glu.h>
#include "Textura.h"


/**********************************************************
 *
 * FUNCTION LoadBitmap(char *)
 *
 * This function loads a bitmap file and return the OpenGL reference ID to use that texture
 *
 *********************************************************/
int LoadBitmapTexture(char *filename)
{
    /*** Counter to keep track of the last loaded texture ***/
    static int num_texture=-1;
    num_texture++; // The counter of the current texture is increased
    return LoadBitmapTexture(filename, num_texture);
}

int LoadBitmapTexture(char *filename, int glId)
{
    FILE * file;
    char temp;
    long i;

    BITMAPINFOHEADER infoheader;


    if( (file = fopen(filename, "rb"))==NULL) return (-1); // Open the file for reading

    fseek(file, 18, SEEK_CUR);  /* start reading width & height */
    fread(&infoheader.biWidth, sizeof(int), 1, file);

    fread(&infoheader.biHeight, sizeof(int), 1, file);

    fread(&infoheader.biPlanes, sizeof(short int), 1, file);
    if (infoheader.biPlanes != 1) {
        printf("Planes from %s is not 1: %u\n", filename, infoheader.biPlanes);
        return -1;
    }

    // read the bpp
    fread(&infoheader.biBitCount, sizeof(unsigned short int), 1, file);
    if (infoheader.biBitCount != 24) {
      printf("Bpp from %s is not 24: %d\n", filename, infoheader.biBitCount);
      return -1;
    }

    fseek(file, 24, SEEK_CUR);

    // read the data.
    char *data = (char *) malloc(infoheader.biWidth * infoheader.biHeight * 3);
    if (data == NULL) {
        printf("Error allocating memory for color-corrected image data\n");
        return -1;
    }

    if ((i = fread(data, infoheader.biWidth * infoheader.biHeight * 3, 1, file)) != 1) {
        printf("Error reading image data from %s.\n", filename);
        return -1;
    }

    fclose(file); // Closes the file stream

    for (i=0; i<(infoheader.biWidth * infoheader.biHeight * 3); i+=3) { // reverse all of the colors. (bgr -> rgb)
        temp = data[i];
        data[i] = data[i+2];
        data[i+2] = temp;
    }

    glBindTexture(GL_TEXTURE_2D, glId); // Bind the ID texture specified by the 2nd parameter

    // The next commands sets the texture parameters
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // If the u,v coordinates overflow the range 0,1 the image is repeated
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // The magnification function ("linear" produces better results)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); //The minifying function

    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); // We don't combine the color with the original surface color, use only the texture map.

    // Finally we define the 2d texture
    glTexImage2D(GL_TEXTURE_2D, 0, 3, infoheader.biWidth, infoheader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // And create 2d mipmaps for the minifying function
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, infoheader.biWidth, infoheader.biHeight, GL_RGB, GL_UNSIGNED_BYTE, data);

    free(data); // Free the memory we used to load the texture

    return (glId); // Returns the current texture OpenGL ID
}
