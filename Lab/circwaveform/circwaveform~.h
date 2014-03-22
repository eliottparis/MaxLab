/**
 * Copyright (c) 2014 Eliott Paris
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE" in this distribution.
 */

#ifndef __DEF_CIRCWAVEFORM__
#define __DEF_CIRCWAVEFORM__

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

#include "../lib/Tools.h"

#define  contrast_white 0.06
#define  contrast_black 0.14

typedef struct  _circwaveform
{
	t_jbox			j_box;
	t_buffer_ref*	f_buffer_ref;
	t_symbol*		f_buffername;
	long			f_chanoffset;
	
	float*			f_display_buffer;
	float			f_max_peak;
	
	double			f_zoom_factor;
	double			f_waveform_radius;
    
	t_jrgba			f_color_bg;
    t_jrgba			f_color_bd;
	t_jrgba			f_color_waveform;
	t_jrgba			f_color_cursor;
	t_jrgba			f_color_selection;
	
	long			f_cornersize;
	long			f_bordersize;
	
	double			f_center;
	
	long			f_number_of_points;
	
	double			f_selection_start;
	double			f_selection_end;
	double			f_position;
    
} t_circwaveform;

void *circwaveform_new(t_symbol *s, int argc, t_atom *argv);
void circwaveform_free(t_circwaveform *x);
void circwaveform_assist(t_circwaveform *x, void *b, long m, long a, char *s);

t_max_err circwaveform_notify(t_circwaveform *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void circwaveform_getdrawparams(t_circwaveform *x, t_object *patcherview, t_jboxdrawparams *params);

void update_buffer_display(t_circwaveform *x);

// -- playing methods -- //

void circwaveform_position(t_circwaveform *x, double f);

// -- buffer's methods -- //

void circwaveform_normalize(t_circwaveform *x, double f);

// -- mousing methods -- //

void circwaveform_mouse_move(t_circwaveform *x, t_object *patcherview, t_pt pt, long modifiers);
void circwaveform_mouse_down(t_circwaveform *x, t_object *patcherview, t_pt pt, long modifiers);
void circwaveform_mouse_drag(t_circwaveform *x, t_object *patcherview, t_pt pt, long modifiers);

// -- attrs settings methods -- //

t_max_err circwaveform_setattr_buffername(t_circwaveform *x, t_symbol *s, long ac, t_atom* av);
t_max_err circwaveform_setattr_chanoffset(t_circwaveform *x, t_symbol *s, long ac, t_atom* av);
t_max_err circwaveform_setattr_number_of_points(t_circwaveform *x, t_symbol *s, long ac, t_atom* av);
t_max_err circwaveform_setattr_zoom_factor(t_circwaveform *x, t_symbol *s, long ac, t_atom* av);
t_max_err circwaveform_setattr_waveform_radius(t_circwaveform *x, t_symbol *s, long ac, t_atom* av);

// -- drawing methods -- //

void circwaveform_paint(t_circwaveform *x, t_object *view);
void draw_background(t_circwaveform *x, t_object *view, t_rect *rect);
void draw_waveform(t_circwaveform *x,  t_object *view, t_rect *rect);
void draw_cursor(t_circwaveform *x, t_object *view, t_rect *rect);
void draw_selection(t_circwaveform *x, t_object *view, t_rect *rect);

t_class *circwaveform_class;

#endif
