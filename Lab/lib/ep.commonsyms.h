/**
 * Copyright (c) 2014 Eliott Paris
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE" in this distribution.
 */

#ifndef __DEF_EP_MAX_COMMONSYMS__
#define __DEF_EP_MAX_COMMONSYMS__

extern "C"
{
#include <ext.h>
}

// notify
static t_symbol* ep_sym_nothing 					= gensym("");
static t_symbol* ep_sym_notify						= gensym("notify");
static t_symbol* ep_sym_modified 					= gensym("modified");
static t_symbol* ep_sym_attr_modified 				= gensym("attr_modified");
static t_symbol* ep_sym_buffer_modified				= gensym("buffer_modified");
static t_symbol* ep_sym_getname 					= gensym("getname");

// Methods
static t_symbol* ep_sym_set 						= gensym("set");
static t_symbol* ep_sym_bang 						= gensym("bang");
static t_symbol* ep_sym_float 						= gensym("float");
static t_symbol* ep_sym_int 						= gensym("int");
static t_symbol* ep_sym_list 						= gensym("list");
static t_symbol* ep_sym_anything					= gensym("anything");
static t_symbol* ep_sym_dspchain					= gensym("dspchain");
static t_symbol* ep_sym_dynlet_begin 				= gensym("dynlet_begin");
static t_symbol* ep_sym_dynlet_end					= gensym("dynlet_end");
static t_symbol* ep_sym_start						= gensym("start");
static t_symbol* ep_sym_stop 						= gensym("stop");
static t_symbol* ep_sym_front 						= gensym("front");
static t_symbol* ep_sym_iterate 					= gensym("iterate");

// Paint
static t_symbol* ep_sym_rect 						= gensym("rect");
static t_symbol* ep_sym_bgcolor 					= gensym("bgcolor");
static t_symbol* ep_sym_bdcolor 					= gensym("bdcolor");

// Patchers
static t_symbol* ep_sym_patcher 					= gensym("patcher");
static t_symbol* ep_sym_bpatcher 					= gensym("bpatcher");
static t_symbol* ep_sym_jpatcher 					= gensym("jpatcher");
static t_symbol* ep_sym_noedit						= gensym("noedit");
static t_symbol* ep_sym_vis 						= gensym("vis");
static t_symbol* ep_sym_invis 						= gensym("invis");
static t_symbol* ep_sym_P 							= gensym("#P");
static t_symbol* ep_sym_selectbox 					= gensym("selectbox");
static t_symbol* ep_sym_insertboxtext 				= gensym("insertboxtext");
static t_symbol* ep_sym_gettextptr					= gensym("gettextptr");
static t_symbol* ep_sym_firstview 					= gensym("firstview");
static t_symbol* ep_sym_connect 					= gensym("connect");
static t_symbol* ep_sym_selectedboxes				= gensym("selectedboxes");
static t_symbol* ep_sym_startdrag					= gensym("startdrag");

// Layers
static t_symbol* ep_sym_background_layer 			= gensym("background_layer");

#endif
