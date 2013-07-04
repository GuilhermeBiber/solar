/*
 * Text.h
 *
 *  Created on: 15/06/2013
 *      Author: Guilherme Sampaio
 *            : Frederico Sampaio
 */

#ifndef TEXT_H_
#define TEXT_H_

#include <string>

class Texto {
private:
	char message[256];
    double x, y, z;
    double cor[3];
public:
    void setColor(int rgb[3]);
    void setPosition(double _x, double _y, double _z);
	void setText(char *);
	void setText(const char* format, ...);
    void draw();
    void draw2D();
};

#endif /* TEXT_H_ */
