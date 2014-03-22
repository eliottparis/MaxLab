/**
 * Copyright (c) 2014 Eliott Paris
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE" in this distribution.
 *
 * Some of the following code comes from the HoaLibrary : https://github.com/CICM/HoaLibrary/
 * Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
 */

#ifndef __DEF_EP_TOOLS__
#define __DEF_EP_TOOLS__

#include "ep.defs.h"

//! The minimum function
/** This function returns the minimum value between two values
 
 @param     v1   The first value.
 @param     v2   The low boundary.
 @return    The maximum value.
 
 @see    min
 */
inline double min(const double v1, const double v2)
{
	if (v1 <= v2)
		return v1;
	else
		return v2;
}

//! The maximum function
/** This function returns the maximum value between two values
 
 @param     v1   The first value.
 @param     v2   The low boundary.
 @return    The maximum value.
 
 @see    min
 */
inline double max(const double v1, const double v2)
{
	if (v1 >= v2)
		return v1;
	else
		return v2;
}

//! The clipping function
/** The function clips a number between boundaries. \n
 If \f$x < min\f$, \f$f(x) = min\f$ else if \f$x > max\f$, \f$f(x) = max\f$ else \f$f(x) = x\f$.
 
 @param     value   The value to clip.
 @param     min     The low boundary.
 @param     max     The high boundary.
 @return    The function return the clipped value.
 
 @see    clip_min
 @see    clip_max
 */
inline double clip_minmax(const double value, const double min, const double max)
{
	if(value < min)
		return min;
	else if(value > max)
		return max;
	else
		return value;
}

//! The minimum clipping function
/** The function clips a number at a minimum value. \n
 If \f$x < min\f$, \f$f(x) = min\f$ else \f$f(x) = x\f$.
 
 @param     value   The value to clip.
 @param     low     The low boundary.
 @return    The function return the clipped value.
 
 @see    clip_minmax
 @see    clip_max
 */
inline double clip_min(const double value, const double low)
{
	if(value < low)
		return low;
	else
		return value;
}

//! The maximum clipping function
/** The function clips a number at a maximum value. \n
 If \f$x > max\f$, \f$f(x) = max\f$ else \f$f(x) = x\f$.
 
 @param     value   The value to clip.
 @param     high    The high boundary.
 @return    The function return the clipped value.
 
 @see    clip_minmax
 @see    clip_min
 */
inline double clip_max(const double value, const double high)
{
	if(value > high)
		return high;
	else
		return value;
}

//! The wrapping function
/** The function wraps a number between boundarys.
 
 @param     value   The value to wrap.
 @param     low     The low boundary.
 @param     high    The high boundary.
 @return    The function return the wrapped value.
 
 @see    wrap_twopi
 */
inline double wrap(double value, const double low, const double high)
{
	double increment = high - low;
	while(value < low)
	{
		value += increment;
	}
	
	while(value > high)
	{
		value -= increment;
	}
	
	return value;
}

//! The wrapping function over \f$2\pi\f$
/** The function wraps a number between \f$0\f$ and \f$2\pi\f$.
 
 @param     value   The value to wrap.
 @return    The function return the wrapped value.
 
 @see    wrap, wrap_360
 */
inline double wrap_twopi(const double value)
{
	double new_value = value;
	while(new_value < 0.)
	{
		new_value += EP_2PI;
	}
	
	while(new_value >= EP_2PI)
	{
		new_value -= EP_2PI;
	}
	return new_value;
}

//! The wrapping function in degrees.
/** The function wraps a number between \f$0\f$ and \f$360°\f$.
 
 @param     value   The value to wrap.
 @return    The wrapped value.
 
 @see    wrap, wrap_twopi
 */
inline double wrap_360(const double value)
{
	double new_value = value;
	while(new_value < 0.)
		new_value += 360.;
	while(new_value >= 360.)
		new_value -= 360.;
	
	return new_value;
}

//! The ordinate converter function.
/** This function takes a radius and an azimuth value and convert them to an ordinate value.
 
 @param     radius		The radius value (greather than 0).
 @param     azimuth		The azimuth value (between \f$0\f$ and \f$2\pi\f$).
 @return    The ordinate value.
 
 @see    abscissa
 */
inline double ordinate(const double radius, const double azimuth)
{
	return radius * sin(azimuth + EP_PI2);
}

//! The abscissa converter function.
/** This function takes a radius and an azimuth value and convert them to an abscissa value.
 
 @param     radius		The radius value (greather than 0).
 @param     azimuth		The azimuth value (between \f$0\f$ and \f$2\pi\f$).
 @return    The abscissa value.
 
 @see    ordinate
 */
inline double abscissa(const double radius, const double azimuth)
{
	return radius * cos(azimuth + EP_PI2);
}

//! The radius converter function.
/** This function takes an abscissa and an ordinate value and convert them to a radius value.
 
 @param     x		The abscissa value.
 @param     y		The ordinate value.
 @return    The radius value.
 
 @see    azimuth
 */
inline double radius(const double x, const double y)
{
	return sqrt(x*x + y*y);
}

//! The azimuth converter function.
/** This function takes an abscissa and an ordinate value and convert them to an azimuth value.
 
 @param     x		The abscissa value.
 @param     y		The ordinate value.
 @return    The azimuth value.
 
 @see    radius
 */
inline double azimuth(const double x, const double y)
{
	return atan2(y, x) - EP_PI2;
}

inline double distance(const double x1, const double y1, const double x2, const double y2)
{
	return sqrt((x1-x2) * (x1-x2) + (y1-y2) * (y1-y2));
}

inline double distance_radian(double angle1, double angle2)
{
	angle1 = wrap_twopi(angle1);
	angle2 = wrap_twopi(angle2);
	if(angle1 > angle2)
		return (angle1 - angle2);
	else
		return (angle2 - angle1);
}

inline double radianClosestDistance(double angle1, double angle2)
{
	double minRad, maxRad;
	angle1 = wrap_twopi(angle1);
	angle2 = wrap_twopi(angle2);
	minRad = min(angle1, angle2);
	maxRad = max(angle1, angle2);
	
	if (maxRad - minRad <= EP_PI)
		return maxRad - minRad;
	else
		return EP_2PI - (maxRad - minRad);
}

inline double radToDeg(const double radian)
{
	return radian * 360. / EP_2PI;
}

inline double degToRad(const double degree)
{
	return degree / 360. * EP_2PI;
}

inline long mstosamps(const double ms, const double samplerate = 44100.)
{
	return (long)(samplerate * ms * 0.001);
}

inline double sampstoms(const double s, const double samplerate=44100.)
{
	return 1000. * s / samplerate;
}

inline double atodb(const double amp)
{
	return (amp <= 0.) ? -999.f : (20. * log10(amp));
}

inline double dbtoa(const double dB)
{
	return pow(10., dB * 0.05);
}

inline double safediv(const double num, const double denom)
{
	return denom == 0. ? 0. : num/denom;
}

inline double scale(const double in, const double inlow, const double inhigh, const double outlow, const double outhigh, const double power)
{
	double value;
	double inscale = safediv(1., inhigh - inlow);
	double outdiff = outhigh - outlow;
	
	value = (in - inlow) * inscale;
	if (value > 0.0)
		value = pow(value, power);
	else if (value < 0.0)
		value = -pow(-value, power);
	value = (value * outdiff) + outlow;
	
	return value;
}

inline double scale(const double in, const double inlow, const double inhigh, const double outlow, const double outhigh)
{
	return ( (in - inlow) * safediv(1., inhigh - inlow) * (outhigh - outlow) ) + outlow;
}

inline bool isInside(const double val, const double v1, const double v2)
{
	return (v1 <= v2) ? (val >= v1 && val <= v2) : (val >= v2 && val <= v1);
}

inline bool isInsideRad(const double radian, const double loRad, const double hiRad)
{
	return isInside(wrap_twopi(radian-loRad), 0, wrap_twopi(hiRad-loRad));
}

inline bool isInsideDeg(const double degree, const double loDeg, const double hiDeg)
{
	return isInside(wrap_360(degree-loDeg), 0, wrap_360(hiDeg-loDeg));
}

inline void vector_add(unsigned int size, double *vec, double inc)
{
	for (unsigned int i=0; i < size; i++)
		vec[i] += inc;
}

inline void vector_clip_minmax(unsigned int size, double *vec, double min, double max)
{
	for (unsigned i=0; i < size; i++)
		vec[i] = clip_minmax(vec[i], min, max);
}

inline void vector_sort(unsigned int size, double* vector)
{
	int index;
	double* temp = new double[size];
	if(temp && size)
	{
		index  = 0;
		temp[0] = vector[0];
		temp[size - 1] = vector[0];
		
		for(unsigned int i = 1; i < size; i++)
		{
			if(vector[i] < temp[0]) // Get the minimum
			{
				temp[0] = vector[i];
				index = i;
			}
			if(vector[i] > temp[size - 1]) // Get the maximum
				temp[size - 1] = vector[i];
		}
		vector[index] -= 1;
		
		for(unsigned int i = 1; i < size - 1; i++)
		{
			temp[i] = temp[size - 1];
			index   = -1;
			for(unsigned int j = 0; j < size; j++)
			{
				if(vector[j] >= temp[i-1] && vector[j] <= temp[i])
				{
					temp[i] = vector[j];
					index = j;
				}
			}
			if(index > -1)
			{
				vector[index] -= 1;
			}
		}
		memcpy(vector, temp, size * sizeof(double));
		delete [] temp;
	}
}

#endif