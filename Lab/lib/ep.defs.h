/**
 * Copyright (c) 2014 Eliott Paris
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE" in this distribution.
 *
 * Some of the following code comes from the HoaLibrary : https://github.com/CICM/HoaLibrary/
 * Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
 */

#ifndef __DEF_EP_DEFS__
#define __DEF_EP_DEFS__

#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string>

#ifdef __APPLE__

#include <Accelerate/Accelerate.h>

#endif

#ifdef _WINDOWS

#include <ipps.h>
#include <ippm.h>
#include <mkl.h>
#include <mkl_cblas.h>

#endif

#define EP_PI  (3.141592653589793238462643383279502884)
#define EP_2PI (6.283185307179586476925286766559005)
#define EP_PI2 (1.57079632679489661923132169163975144)
#define EP_PI4 (0.785398163397448309615660845819875721)

namespace Hoa
{
}

#endif


