/**
 * Copyright (c) 2014 Eliott Paris
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE" in this distribution.
 */

#ifndef __DEF_EP_MAX__
#define __DEF_EP_MAX__

#include "ep.defs.h"
#include "ep.tools.h"
#include "ep.commonsyms.h"

extern "C"
{
#include <ext.h>
#include <ext_obex.h>
#include <z_dsp.h>
#include <ext_common.h>
#include <jpatcher_api.h>
#include <jgraphics.h>
#include <jpatcher_syms.h>
#include <ext_dictionary.h>
#include <buffer.h>
#include <ext_globalsymbol.h>
#include <commonsyms.h>
#include <ext_parameter.h>
#include <jpatcher_syms.h>
}

/*
static t_symbol* ep_sym_version			= gensym("(v2.0)");
static t_symbol* ep_sym_credit_line1	= gensym("by Julien Colafrancesco, Pierre Guillot & Eliott Paris");
static t_symbol* ep_sym_credit_line2	= gensym("Copyright \u00a9 2012 - 2014, CICM | Universite Paris 8");
*/

/**
 Print to the Max console some credits for the HoaLibrary
 @ingroup hoa_max
 */
void ep_print_credit();

#endif


