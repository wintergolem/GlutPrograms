/*  ===============================================
    IO.cpp - read/write from file
    Copyright (c) Jules Bloomenthal, Seattle, 2012.
    All rights reserved.
    ===============================================

    Contents
        RGBfromHSV
        HSVfromRGB
*/

#include <math.h>
#include <float.h>
#include "Color.h"


//==========[ RGB/HSV ]=============

double Max(double a, double b)
{
	return a < b? b : a;
}

double Min(double a, double b)
{
	return a < b? a : b;
}

bool IsZero(double d)
{
    return d < DBL_EPSILON && d > -DBL_EPSILON;
};


void RGBfromHSV(double hsv[3], double rgb[3])
{
  // Foley and van Dam Chapter 13, pp 593
  hsv[0] *= 360;                       // text expects h to be [0..360] but we use [0..1]
  hsv[0] = fmod(hsv[0], 360) ;         // degrees cannot exceed 360
  if (IsZero(hsv[1])) {                // color is on the black-and-white center line
	  rgb[0] = rgb[1] = rgb[2] = hsv[2];
	  return;
  }
  else {                               // chromatic color: there is a hue
    if (IsZero(hsv[0]-360))            // change 360 to 0 degrees
      hsv[0] = 0;
    hsv[0] = hsv[0]/60.f;              // h now in [0,6]
    int i = (int)floor(hsv[0]);        // floor returns the largest integer <= h
    double f = hsv[0]-i;
    double p = hsv[2]*(1-hsv[1]);
    double q = hsv[2]*(1-(hsv[1]*f));
    double t = hsv[2]*(1-(hsv[1]*(1-f)));
    switch(i) {
      case 0:
		  rgb[0] = hsv[2];
		  rgb[1] = t;
		  rgb[2] = p;
          break;
      case 1:
		  rgb[0] = q;
		  rgb[1] = hsv[2];
		  rgb[2] = p;
          break;
      case 2:
		  rgb[0] = p;
		  rgb[1] = hsv[2];
		  rgb[2] = t;
          break;
      case 3:
		  rgb[0] = p;
		  rgb[1] = q;
		  rgb[2] = hsv[2];
          break;;
      case 4:
		  rgb[0] = t;
		  rgb[1] = p;
		  rgb[2] = hsv[2];
          break;
      default:
		  rgb[0] = hsv[2];
		  rgb[1] = p;
		  rgb[2] = q;
          break;
    }
  }
}

void HSVfromRGB(double rgb[3], double hsv[3])
{
  // Foley and van Dam Chapter 13, pp 592
  double max = Max(Max(rgb[0], rgb[1]), rgb[2]);
  double min = Min(Min(rgb[0], rgb[1]), rgb[2]);
  hsv[2] = max;
  // calculate saturation s
  if (!IsZero(max))
    hsv[1] = (max-min)/max;
  else
    hsv[1] = 0;
  if (IsZero(hsv[1])) 
    hsv[0] = -1;                       // achromatic case, no hue
  else {
    double delta = max-min;
    if (IsZero(rgb[0]-max)) 
      hsv[0] = (rgb[1]-rgb[2])/delta;    // resulting color is between yellow and magenta
    else if (IsZero(rgb[1]-max))
      hsv[0] = 2+(rgb[2]-rgb[0])/delta;  // resulting color is between cyan and yellow
    else if (IsZero(rgb[2]-max))
      hsv[0] = 4+(rgb[0]-rgb[1])/delta;  // resulting color is between magenta and cyan

    hsv[0] = hsv[0]*60;                 // convert hue to degrees
    if (hsv[0] < 0.0)
      hsv[0] += 360;                   // ensure hue is nonnegative
  }
  hsv[0] /= 360.;                      // h, s, and v to be in range [0..1]
}
