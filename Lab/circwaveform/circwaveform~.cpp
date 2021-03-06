/**
 * Copyright (c) 2014 Eliott Paris
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE" in this distribution.
 */

#include "../lib/ep.max.h"

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
	
	long			f_number_of_points;
	long			f_wf_draw_mode;
	long			f_cursor_mode;
	
	double			f_selection_start;
	double			f_selection_end;
	double			f_position;
	
	double			f_center;
	double			f_max_radius;
    
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
void draw_waveform_sticks(t_circwaveform *x,  t_object *view, t_rect *rect);
void draw_cursor(t_circwaveform *x, t_object *view, t_rect *rect);
void draw_selection(t_circwaveform *x, t_object *view, t_rect *rect);

t_class *circwaveform_class;

int C74_EXPORT main()
{
	t_class *c;
    
	c = class_new("circwaveform~", (method)circwaveform_new, (method)circwaveform_free, (short)sizeof(t_circwaveform), 0L, A_GIMME, 0);
    
	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
	jbox_initclass(c, JBOX_COLOR | JBOX_FIXWIDTH);

	class_addmethod(c, (method)circwaveform_assist,				"assist",			A_CANT,	0);
	class_addmethod(c, (method)circwaveform_paint,				"paint",			A_CANT,	0);
	class_addmethod(c, (method)circwaveform_notify,				"notify",			A_CANT, 0);
	class_addmethod(c, (method)circwaveform_getdrawparams,		"getdrawparams",	A_CANT, 0);
	class_addmethod(c, (method)circwaveform_mouse_down,			"mousedown",		A_CANT, 0);
    class_addmethod(c, (method)circwaveform_mouse_move,			"mousemove",		A_CANT, 0);
	class_addmethod(c, (method)circwaveform_mouse_drag,			"mousedrag",		A_CANT, 0);
    class_addmethod(c, (method)circwaveform_mouse_down,			"mouseup",			A_CANT, 0);
	class_addmethod(c, (method)circwaveform_setattr_buffername, "set",				A_GIMME, 0);
	class_addmethod(c, (method)circwaveform_normalize,			"normalize",		A_FLOAT, 0);
	class_addmethod(c, (method)circwaveform_position,			"cursor",			A_FLOAT, 0);
    
	CLASS_ATTR_INVISIBLE            (c, "color", 0);
	CLASS_ATTR_INVISIBLE            (c, "textcolor", 0);
	CLASS_ATTR_DEFAULT              (c, "patching_rect", 0, "0 0 225 225");
    
	// -- Color -- //
	
	CLASS_STICKY_CATEGORY           (c, 0, "Color");
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_circwaveform, f_color_bg);
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "rgba");
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.95 0.95 0.95 1.");
    
    CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_circwaveform, f_color_bd);
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "rgba");
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.7 0.7 0.7 1.");
	
	CLASS_ATTR_RGBA                 (c, "cursorcolor", 0, t_circwaveform, f_color_cursor);
	CLASS_ATTR_STYLE                (c, "cursorcolor", 0, "rgba");
	CLASS_ATTR_LABEL                (c, "cursorcolor", 0, "Cursor Color");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "cursorcolor", 0, "0.8 0. 0. 1.");
	
	CLASS_ATTR_RGBA                 (c, "selectioncolor", 0, t_circwaveform, f_color_selection);
	CLASS_ATTR_STYLE                (c, "selectioncolor", 0, "rgba");
	CLASS_ATTR_LABEL                (c, "selectioncolor", 0, "Selection Color");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "selectioncolor", 0, "0.5 0.5 0.5 0.5");
	
	CLASS_ATTR_RGBA                 (c, "waveformcolor", 0, t_circwaveform, f_color_waveform);
	CLASS_ATTR_STYLE                (c, "waveformcolor", 0, "rgba");
	CLASS_ATTR_LABEL                (c, "waveformcolor", 0, "Waveform Color");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "waveformcolor", 0, "0.4 0.4 0.4 1.");
	CLASS_STICKY_CATEGORY_CLEAR (c);
	
	// -- Appearance -- //
	
	CLASS_STICKY_CATEGORY           (c, 0, "Appearance");
	CLASS_ATTR_LONG                 (c, "cornersize", 0, t_circwaveform, f_cornersize);
    CLASS_ATTR_LABEL                (c, "cornersize", 0, "Corner Size");
    CLASS_ATTR_DEFAULT              (c, "cornersize", 0, "0");
	CLASS_ATTR_FILTER_MIN			(c, "cornersize", 0);
    CLASS_ATTR_SAVE                 (c, "cornersize", 1);
	
	CLASS_ATTR_LONG					(c, "border", 0, t_circwaveform, f_bordersize);
    CLASS_ATTR_LABEL                (c, "border", 0, "Border Size");
    CLASS_ATTR_DEFAULT              (c, "border", 0, "1");
	CLASS_ATTR_FILTER_MIN			(c, "border", 0);
    CLASS_ATTR_SAVE                 (c, "border", 1);
	
	CLASS_ATTR_DOUBLE				(c, "zoom", 0, t_circwaveform, f_zoom_factor);
	CLASS_ATTR_ACCESSORS            (c, "zoom", NULL, circwaveform_setattr_zoom_factor);
    CLASS_ATTR_LABEL                (c, "zoom", 0, "Zoom Factor");
    CLASS_ATTR_DEFAULT              (c, "zoom", 0, "1");
    CLASS_ATTR_SAVE                 (c, "zoom", 1);
	
	CLASS_ATTR_DOUBLE				(c, "wfradius", 0, t_circwaveform, f_waveform_radius);
	CLASS_ATTR_ACCESSORS            (c, "wfradius", NULL, circwaveform_setattr_waveform_radius);
    CLASS_ATTR_LABEL                (c, "wfradius", 0, "Waveform Radius");
    CLASS_ATTR_DEFAULT              (c, "wfradius", 0, "0.5");
    CLASS_ATTR_SAVE                 (c, "wfradius", 1);
	
	CLASS_ATTR_LONG                 (c, "wfdrawmode", 0, t_circwaveform, f_wf_draw_mode);
    CLASS_ATTR_LABEL                (c, "wfdrawmode", 0, "Waveform Drawing Mode");
    CLASS_ATTR_ENUMINDEX2           (c, "wfdrawmode", 0, "Full", "Sticks")
	CLASS_ATTR_DEFAULT              (c, "wfdrawmode", 0, "0");
	
	CLASS_ATTR_LONG                 (c, "cursordrawmode", 0, t_circwaveform, f_cursor_mode);
    CLASS_ATTR_LABEL                (c, "cursordrawmode", 0, "Cursor Drawing Mode");
    CLASS_ATTR_ENUMINDEX4           (c, "cursordrawmode", 0, "Thin Line", "Gauge", "Bar", "Gauge Bar")
	CLASS_ATTR_DEFAULT              (c, "cursordrawmode", 0, "0");
	CLASS_STICKY_CATEGORY_CLEAR (c);
	
	
	// -- Behavior -- //
	
	CLASS_STICKY_CATEGORY           (c, 0, "Behavior");
	CLASS_ATTR_SYM                  (c, "buffername", 0, t_circwaveform, f_buffername);
	CLASS_ATTR_CATEGORY             (c, "buffername", 0, "Behavior");
	CLASS_ATTR_ACCESSORS            (c, "buffername", NULL, circwaveform_setattr_buffername);
	CLASS_ATTR_LABEL                (c, "buffername", 0, "Buffer~ Object Name");
	CLASS_ATTR_DEFAULT              (c, "buffername", 0, "");
	CLASS_ATTR_SAVE                 (c, "buffername", 1);
	
    CLASS_ATTR_LONG                 (c, "chanoffset", 0, t_circwaveform, f_chanoffset);
    CLASS_ATTR_LABEL                (c, "chanoffset", 0, "Channel");
	CLASS_ATTR_ACCESSORS            (c, "chanoffset", NULL, circwaveform_setattr_chanoffset);
    CLASS_ATTR_DEFAULT              (c, "chanoffset", 0, "1");
    CLASS_ATTR_SAVE                 (c, "chanoffset", 1);
	
	CLASS_ATTR_LONG                 (c, "nbpoints", 0, t_circwaveform, f_number_of_points);
    CLASS_ATTR_LABEL                (c, "nbpoints", 0, "Number of Points");
	CLASS_ATTR_ACCESSORS            (c, "nbpoints", NULL, circwaveform_setattr_number_of_points);
    CLASS_ATTR_DEFAULT              (c, "nbpoints", 0, "360");
    CLASS_ATTR_SAVE                 (c, "nbpoints", 1);
	CLASS_STICKY_CATEGORY_CLEAR (c);
	
	ep_print_credit();
	
	class_register(CLASS_BOX, c);
	circwaveform_class = c;
    
	return 0;
}

void *circwaveform_new(t_symbol *s, int argc, t_atom *argv)
{
	t_circwaveform *x =  NULL; 
	t_dictionary *d;
	long flags;
	
	if (!(d = object_dictionaryarg(argc,argv)))
		return NULL;

	x = (t_circwaveform *)object_alloc(circwaveform_class);
	flags = 0 
			| JBOX_DRAWFIRSTIN 
			| JBOX_DRAWINLAST
			| JBOX_TRANSPARENT	
			| JBOX_GROWY
			//| JBOX_DRAWBACKGROUND
			;
    
	jbox_new((t_jbox *)x, flags, argc, argv);
	x->j_box.b_firstin = (t_object *)x;
	
	x->f_buffername = gensym("");
	x->f_chanoffset = 1;
	x->f_number_of_points = 360;
	
	x->f_display_buffer = new float[x->f_number_of_points];

	attr_dictionary_process(x, d);
	jbox_ready((t_jbox *)x);

	return (x);
}

t_max_err circwaveform_setattr_buffername(t_circwaveform *x, t_symbol *s, long ac, t_atom* av)
{
	if (ac && av && atom_gettype(av) == A_SYM)
	{
		t_symbol* lastname = x->f_buffername;
		x->f_buffername = atom_getsym(av);
		
		if (lastname != x->f_buffername)
		{
			if (!x->f_buffer_ref)
				x->f_buffer_ref = buffer_ref_new((t_object*)x, x->f_buffername);
			else
				buffer_ref_set(x->f_buffer_ref, x->f_buffername);
			
			update_buffer_display(x);
			jbox_invalidate_layer((t_object *)x, NULL, gensym("waveform_layer"));
			jbox_redraw((t_jbox *)x);
		}
	}
	
	return MAX_ERR_NONE;
}

t_max_err circwaveform_setattr_chanoffset(t_circwaveform *x, t_symbol *s, long ac, t_atom* av)
{
	if (ac && av && atom_gettype(av) == A_LONG)
	{
		x->f_chanoffset = clip_minmax(atom_getlong(av), 1, 4);
		update_buffer_display(x);
		jbox_invalidate_layer((t_object *)x, NULL, gensym("waveform_layer"));
		jbox_redraw((t_jbox *)x);
	}
	
	return MAX_ERR_NONE;
}

t_max_err circwaveform_setattr_number_of_points(t_circwaveform *x, t_symbol *s, long ac, t_atom* av)
{
	if (ac && av && (atom_gettype(av) == A_LONG || atom_gettype(av) == A_FLOAT))
	{
		x->f_number_of_points = clip_min(atom_getlong(av), 10);
		
		delete [] x->f_display_buffer;
		x->f_display_buffer = new float[x->f_number_of_points];
		
		update_buffer_display(x);
		jbox_invalidate_layer((t_object *)x, NULL, gensym("waveform_layer"));
		jbox_redraw((t_jbox *)x);
	}
	
	return MAX_ERR_NONE;
}

t_max_err circwaveform_setattr_zoom_factor(t_circwaveform *x, t_symbol *s, long ac, t_atom* av)
{
	if (ac && av && (atom_gettype(av) == A_LONG || atom_gettype(av) == A_FLOAT))
	{
		x->f_zoom_factor = clip_min(atom_getfloat(av), 0);
		jbox_invalidate_layer((t_object *)x, NULL, gensym("waveform_layer"));
		jbox_redraw((t_jbox *)x);
	}
	
	return MAX_ERR_NONE;
}

t_max_err circwaveform_setattr_waveform_radius(t_circwaveform *x, t_symbol *s, long ac, t_atom* av)
{
	if (ac && av && (atom_gettype(av) == A_LONG || atom_gettype(av) == A_FLOAT))
	{
		x->f_waveform_radius = clip_minmax(atom_getfloat(av), 0, 1);
		update_buffer_display(x);
		jbox_invalidate_layer((t_object *)x, NULL, gensym("waveform_layer"));
		jbox_redraw((t_jbox *)x);
	}
	
	return MAX_ERR_NONE;
}

void circwaveform_position(t_circwaveform *x, double f)
{
	x->f_position = clip_minmax(f, 0., 1.);
	jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
	jbox_redraw((t_jbox *)x);
}

void circwaveform_assist(t_circwaveform *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET)
	{
		sprintf(s,"(messages)");
	}
	else
	{
        sprintf(s,"");
	}
}

void circwaveform_normalize(t_circwaveform *x, double f)
{
	if (f > 0)
	{
		t_atom value;
		atom_setfloat(&value, f);
		t_atom* rv = NULL;
		object_method_typed(buffer_ref_getobject(x->f_buffer_ref), gensym("normalize"), 1, &value, rv);
	}
}

t_max_err circwaveform_notify(t_circwaveform *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
{
	t_max_err err = MAX_ERR_NONE;
	t_symbol *name;
	
	if (msg == gensym("attr_modified"))
	{
		name = (t_symbol *)object_method((t_object *)data, gensym("getname"));
		
		if(name == gensym("bgcolor") || name == gensym("bdcolor") || name == gensym("cornersize"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
		}
		else if (name == gensym("waveformcolor") || name == gensym("wfdrawmode"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("waveform_layer"));
		}
		else if (name == gensym("cursorcolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
		}
		else if(name == gensym("border") || name == gensym("cursordrawmode"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
			jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
			jbox_invalidate_layer((t_object *)x, NULL, gensym("waveform_layer"));
		}
		else if (name == gensym("selectioncolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("selection_layer"));
		}
		jbox_redraw((t_jbox *)x);
	}
	else if (msg == gensym("buffer_modified"))
	{
		update_buffer_display(x);
		jbox_invalidate_layer((t_object *)x, NULL, gensym("waveform_layer"));
		jbox_redraw((t_jbox *)x);
	}
	
	err |= buffer_ref_notify(x->f_buffer_ref, s, msg, sender, data);
	err |= jbox_notify((t_jbox *)x, s, msg, sender, data);
	
	return err;
}

void circwaveform_getdrawparams(t_circwaveform *x, t_object *patcherview, t_jboxdrawparams *params)
{
	params->d_borderthickness = params->d_cornersize = 0;
}

void circwaveform_paint(t_circwaveform *x, t_object *view)
{
	t_rect rect;
	jbox_get_rect_for_view((t_object *)x, view, &rect);
	
	x->f_center = rect.width * 0.5;
	x->f_max_radius = x->f_center - x->f_bordersize;
	
	draw_background(x, view, &rect);
	//draw_selection(x, view, &rect);
	
	if (x->f_cursor_mode != 3)
	{
		if (x->f_wf_draw_mode == 0)
			draw_waveform(x, view, &rect);
		else
			draw_waveform_sticks(x, view, &rect);
		
		draw_cursor(x, view, &rect);
	}
	else
	{
		draw_cursor(x, view, &rect);
		
		if (x->f_wf_draw_mode == 0)
			draw_waveform(x, view, &rect);
		else
			draw_waveform_sticks(x, view, &rect);
	}
}

void draw_background(t_circwaveform *x,  t_object *view, t_rect *rect)
{
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, gensym("background_layer"), rect->width, rect->height);
    
	double bdwidth = (x->f_bordersize*0.5);
	
	if (g)
	{
		jgraphics_rectangle_rounded(g,
									bdwidth+0.5,
									bdwidth+0.5,
									rect->width - (2*bdwidth) - 0.5,
									rect->height - (2*bdwidth) - 0.5,
									x->f_cornersize, x->f_cornersize);
		
		jgraphics_set_source_jrgba(g, &x->f_color_bg);
		jgraphics_fill_preserve(g);
		
		jgraphics_set_source_jrgba(g, &x->f_color_bd);
		jgraphics_set_line_width(g, bdwidth*2);
		jgraphics_stroke(g);
		
		
		jbox_end_layer((t_object*)x, view, gensym("background_layer"));
	}
	jbox_paint_layer((t_object *)x, view, gensym("background_layer"), 0., 0.);
}

void update_buffer_display(t_circwaveform *x)
{
	if (!buffer_ref_exists(x->f_buffer_ref))
	{
		for (int i= 0; i < x->f_number_of_points; i++)
			x->f_display_buffer[i] = 0.0f;
		
		return;
	}
	
	t_float		*tab;
	long		i, j, chan, frames, nc, idx, subIdx;
	float		rmsValue;
	
	t_buffer_obj *buffer = buffer_ref_getobject(x->f_buffer_ref);
	
	tab = buffer_locksamples(buffer);
	if (!tab)
		return;
	
	chan = min(x->f_chanoffset-1, 3);
	frames = buffer_getframecount(buffer);
	nc = buffer_getchannelcount(buffer);
	
	double inc = (double)frames / (double)x->f_number_of_points;
	
	for (i = 0; i < x->f_number_of_points; i++)
	{
		rmsValue = 0.0f;
		idx = (long) (i * inc);
		
		for (j = idx; j < (idx + inc); j++)
		{
			subIdx = wrap(j, 0, frames - 1);
			
			if (nc > 1)
				subIdx = subIdx * nc + chan;
			
			rmsValue = max(rmsValue, fabs(tab[subIdx]));
		}
		
		x->f_display_buffer[i] = rmsValue;
		
		x->f_max_peak = max(x->f_max_peak, rmsValue);
	}
	
	buffer_unlocksamples(buffer);
}

void draw_waveform(t_circwaveform *x,  t_object *view, t_rect *rect)
{
	if (!buffer_ref_exists(x->f_buffer_ref))
		return;
	
	t_pt	pt[3];
	double	v_azimuth[3], v_ordinate[3];
	double	pixelRange, zoom, wfRad;
	
	double	cursorHalfWidth = (4+2)*0.5;
	
	if (x->f_cursor_mode == 2 || x->f_cursor_mode == 3)
		cursorHalfWidth = 0;
	
	t_jrgba innerColor = x->f_color_waveform;
	
    innerColor.red = clip_min(innerColor.red - contrast_black, 0.);
    innerColor.green = clip_min(innerColor.green - contrast_black, 0.);
    innerColor.blue = clip_min(innerColor.blue - contrast_black, 0.);
	
	t_jgraphics	*g = jbox_start_layer((t_object *)x, view, gensym("waveform_layer"), rect->width, rect->height);
	
	if (g)
	{
		t_jmatrix transform;
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
		
		jgraphics_rotate(g, EP_PI2);
		
		wfRad = x->f_max_radius * x->f_waveform_radius + cursorHalfWidth;
		zoom  = (wfRad - 2) * x->f_zoom_factor;
		pixelRange = x->f_max_radius - x->f_waveform_radius - 2;
		
		jgraphics_arc(g, 0, 0, wfRad, 0, EP_2PI);
		
		v_azimuth[0]	= EP_2PI - EP_PI2;
		v_ordinate[0]	= clip_minmax(wfRad + ( x->f_display_buffer[ 0 ] * zoom), 0, pixelRange);
		
		pt[0].x = abscissa(v_ordinate[0], v_azimuth[0]);
		pt[0].y = ordinate(v_ordinate[0], v_azimuth[0]);
		
		jgraphics_move_to(g, pt[0].x, pt[0].y);
		
		for (int i = 3; i < x->f_number_of_points; i += 3)
		{
			v_azimuth[0]	= EP_2PI - wrap_twopi(( (double)(i-2) / (double)x->f_number_of_points) * EP_2PI + EP_PI2);
			v_azimuth[1]	= EP_2PI - wrap_twopi(( (double)(i-1) / (double)x->f_number_of_points) * EP_2PI + EP_PI2);
			v_azimuth[2]	= EP_2PI - wrap_twopi(( (double)( i ) / (double)x->f_number_of_points) * EP_2PI + EP_PI2);
			
			v_ordinate[0]	= clip_minmax(wfRad + ( x->f_display_buffer[i-2] * zoom), 0, pixelRange);
			v_ordinate[1]	= clip_minmax(wfRad + ( x->f_display_buffer[i-1] * zoom), 0, pixelRange);
			v_ordinate[2]	= clip_minmax(wfRad + ( x->f_display_buffer[ i ] * zoom), 0, pixelRange);
			
			pt[0].x = abscissa(v_ordinate[0], v_azimuth[0]);
			pt[0].y = ordinate(v_ordinate[0], v_azimuth[0]);
			
			pt[1].x = abscissa(v_ordinate[1], v_azimuth[1]);
			pt[1].y = ordinate(v_ordinate[1], v_azimuth[1]);
			
			pt[2].x = abscissa(v_ordinate[2], v_azimuth[2]);
			pt[2].y = ordinate(v_ordinate[2], v_azimuth[2]);
			
			jgraphics_curve_to(g, pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y);
		}
		
		jgraphics_close_path(g);
		
		jgraphics_set_source_jrgba(g, &x->f_color_waveform);
		jgraphics_fill_preserve(g);
		jgraphics_set_line_width(g, 1);
		jgraphics_set_source_jrgba(g, &innerColor);
		jgraphics_stroke(g);
		
		// --- Inner WaveForm --- //
		
		wfRad = x->f_max_radius * x->f_waveform_radius - cursorHalfWidth;
		zoom  = (wfRad - 2) * x->f_zoom_factor;
		pixelRange = x->f_max_radius - x->f_waveform_radius - 2;
		
		jgraphics_arc(g, 0, 0, wfRad, 0, EP_2PI);
		
		v_azimuth[0]	= EP_2PI - EP_PI2;
		v_ordinate[0]	= clip_minmax(wfRad - ( x->f_display_buffer[ 0 ] * zoom), 0, pixelRange);
		
		pt[0].x = abscissa(v_ordinate[0], v_azimuth[0]);
		pt[0].y = ordinate(v_ordinate[0], v_azimuth[0]);
		
		jgraphics_move_to(g, pt[0].x, pt[0].y);
		
		for (int i = 3; i < x->f_number_of_points; i += 3)
		{
			v_azimuth[0]	= EP_2PI - wrap_twopi(( (double)(i-2) / (double)x->f_number_of_points) * EP_2PI + EP_PI2);
			v_azimuth[1]	= EP_2PI - wrap_twopi(( (double)(i-1) / (double)x->f_number_of_points) * EP_2PI + EP_PI2);
			v_azimuth[2]	= EP_2PI - wrap_twopi(( (double)( i ) / (double)x->f_number_of_points) * EP_2PI + EP_PI2);
			
			v_ordinate[0]	= clip_minmax(wfRad - ( x->f_display_buffer[i-2] * zoom), 0, pixelRange);
			v_ordinate[1]	= clip_minmax(wfRad - ( x->f_display_buffer[i-1] * zoom), 0, pixelRange);
			v_ordinate[2]	= clip_minmax(wfRad - ( x->f_display_buffer[ i ] * zoom), 0, pixelRange);
			
			pt[0].x = abscissa(v_ordinate[0], v_azimuth[0]);
			pt[0].y = ordinate(v_ordinate[0], v_azimuth[0]);
			
			pt[1].x = abscissa(v_ordinate[1], v_azimuth[1]);
			pt[1].y = ordinate(v_ordinate[1], v_azimuth[1]);
			
			pt[2].x = abscissa(v_ordinate[2], v_azimuth[2]);
			pt[2].y = ordinate(v_ordinate[2], v_azimuth[2]);
			
			jgraphics_curve_to(g, pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x, pt[2].y);
		}
		
		jgraphics_close_path(g);
		
		jgraphics_set_source_jrgba(g, &innerColor);
		jgraphics_fill_preserve(g);
		jgraphics_set_line_width(g, 1);
		jgraphics_set_source_jrgba(g, &x->f_color_waveform);
		jgraphics_stroke(g);
		
		jbox_end_layer((t_object*)x, view, gensym("waveform_layer"));
	}
	jbox_paint_layer((t_object *)x, view, gensym("waveform_layer"), 0., 0.);
}

void draw_waveform_sticks(t_circwaveform *x,  t_object *view, t_rect *rect)
{
	if (!buffer_ref_exists(x->f_buffer_ref))
		return;

	t_pt	pt[3];
	double	v_azimuth[3], v_ordinate[3];
	
	double pixelRange	= rect->height * 0.5 - x->f_bordersize - 2;
	double wfRad		= x->f_max_radius * x->f_waveform_radius;
	double zoom			= (wfRad - 2) * x->f_zoom_factor;
	
	t_jrgba innerColor = x->f_color_waveform;
	
    innerColor.red = clip_min(innerColor.red - contrast_black, 0.);
    innerColor.green = clip_min(innerColor.green - contrast_black, 0.);
    innerColor.blue = clip_min(innerColor.blue - contrast_black, 0.);
	
	t_jgraphics	*g = jbox_start_layer((t_object *)x, view, gensym("waveform_layer"), rect->width, rect->height);
	
	if (g)
	{
		t_jmatrix transform;
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
		
		jgraphics_set_source_jrgba(g, &x->f_color_waveform);
		jgraphics_set_line_cap(g, JGRAPHICS_LINE_CAP_ROUND);
		jgraphics_set_line_width(g, 2);
		
		 for (int i = 0; i < x->f_number_of_points; i++)
		 {
			 v_azimuth[0]	= EP_2PI - wrap_twopi(( (double)i / (double)x->f_number_of_points) * EP_2PI);
			 v_ordinate[0]	= clip_minmax(wfRad + ( x->f_display_buffer[i] * zoom), 0, pixelRange);
			 v_ordinate[1]	= clip_minmax(wfRad - ( x->f_display_buffer[i] * zoom), 0, pixelRange);
			 
			 pt[0].x = abscissa(v_ordinate[0], v_azimuth[0]);
			 pt[0].y = ordinate(v_ordinate[0], v_azimuth[0]);
			 
			 pt[1].x = abscissa(v_ordinate[1], v_azimuth[0]);
			 pt[1].y = ordinate(v_ordinate[1], v_azimuth[0]);
			 
			 
			 jgraphics_set_source_jrgba(g, i%2 == 0 ? &x->f_color_waveform : &innerColor);
			 
			 jgraphics_move_to(g, pt[0].x, pt[0].y);
			 jgraphics_line_to(g, pt[1].x, pt[1].y);
			 jgraphics_stroke(g);
		 }
		
		jbox_end_layer((t_object*)x, view, gensym("waveform_layer"));
	}
	jbox_paint_layer((t_object *)x, view, gensym("waveform_layer"), 0., 0.);
}

void draw_cursor(t_circwaveform *x, t_object *view, t_rect *rect)
{
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, gensym("cursor_layer"), rect->width, rect->height);
	
	double azimuth_position = (1 - x->f_position) * EP_2PI;
	double cRad;
	
	double cursorWidth = 4;
		
	if (g)
	{
		t_jmatrix transform;
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
		
		jgraphics_set_source_jrgba(g, &x->f_color_cursor);
		
		if (x->f_cursor_mode == 0) // thin line
		{
			cRad = x->f_max_radius * x->f_waveform_radius;
			
			jgraphics_rotate(g, azimuth_position);
			jgraphics_translate(g, 0, cRad);
			jgraphics_arc(g, 0, 0, cursorWidth, 0, EP_2PI);
			jgraphics_fill(g);
		}
		else if (x->f_cursor_mode == 1) // gauge
		{
			cRad = x->f_max_radius * x->f_waveform_radius;
			
			jgraphics_rotate(g, EP_PI2);
			
			jgraphics_set_line_width(g, cursorWidth);
			jgraphics_set_line_cap(g, JGRAPHICS_LINE_CAP_ROUND);
			jgraphics_arc_negative(g, 0, 0, cRad, 0, azimuth_position);
			jgraphics_stroke(g);
		}
		else if (x->f_cursor_mode == 2) // Bar
		{
			t_pt pt;
			cRad = x->f_max_radius - 2;
			pt.x = abscissa(cRad, azimuth_position);
			pt.y = ordinate(cRad, azimuth_position);
			
			jgraphics_set_line_width(g, 2);
			jgraphics_set_line_cap(g, JGRAPHICS_LINE_CAP_ROUND);
			jgraphics_move_to(g, 0, 0);
			jgraphics_line_to(g, pt.x, pt.y);
			jgraphics_stroke(g);
		}
		else if (x->f_cursor_mode == 3) // Bar Gauge
		{
			cRad = x->f_max_radius*0.5;
			
			jgraphics_rotate(g, EP_PI2);
			
			jgraphics_set_line_width(g, x->f_max_radius - 4);
			jgraphics_arc_negative(g, 0, 0, cRad, 0, azimuth_position);
			jgraphics_stroke(g);
			jgraphics_arc(g, 0, 0, 4, 0, EP_2PI);
			jgraphics_fill(g);
		}

		jbox_end_layer((t_object*)x, view, gensym("cursor_layer"));
	}
	jbox_paint_layer((t_object *)x, view, gensym("cursor_layer"), 0., 0.);
}

void draw_selection(t_circwaveform *x, t_object *view, t_rect *rect)
{
	//double selection_start_azimuth, selection_end_azimuth;
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, gensym("selection_layer"), rect->width, rect->height);
	
	if (g)
	{
		t_jmatrix transform;
		jgraphics_matrix_init(&transform, 1, 0, 0, -1, x->f_center, x->f_center);
		jgraphics_set_matrix(g, &transform);
		
		jbox_end_layer((t_object*)x, view, gensym("selection_layer"));
	}
	jbox_paint_layer((t_object *)x, view, gensym("selection_layer"), 0., 0.);
}

/**********************************************************/
/*                      Souris                            */
/**********************************************************/

void circwaveform_mouse_move(t_circwaveform *x, t_object *patcherview, t_pt pt, long modifiers)
{
	/*
    if(modifiers == 132 || modifiers == 5) // ctrl : rotation
        jmouse_setcursor(patcherview, (t_object *)x, JMOUSE_CURSOR_DRAGGINGHAND);
    else if(modifiers == 2) // shift : gain
        jmouse_setcursor(patcherview, (t_object *)x, JMOUSE_CURSOR_RESIZE_FOURWAY);
    else
        jmouse_setcursor(patcherview, (t_object *)x, JMOUSE_CURSOR_ARROW);
    */
    jbox_redraw((t_jbox *)x);
}

void circwaveform_mouse_down(t_circwaveform *x, t_object *patcherview, t_pt pt, long modifiers)
{
	t_pt mouse;
    mouse.x = pt.x - x->f_center;
    mouse.y = x->f_center * 2. - pt.y - x->f_center;
	
	x->f_position = 1 - (azimuth(mouse.x, mouse.y) / EP_2PI);
	
	jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
	jbox_redraw((t_jbox *)x);
}

void circwaveform_mouse_drag(t_circwaveform *x, t_object *patcherview, t_pt pt, long modifiers)
{
    t_pt mouse;
    mouse.x = pt.x - x->f_center;
    mouse.y = x->f_center * 2. - pt.y - x->f_center;
	
	x->f_position = 1 - (azimuth(mouse.x, mouse.y) / EP_2PI);
	
	jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
	jbox_redraw((t_jbox *)x);
}

void circwaveform_free(t_circwaveform *x)
{
	jbox_free((t_jbox *)x);
	object_free(x->f_buffer_ref);
	
	delete [] x->f_display_buffer;
}