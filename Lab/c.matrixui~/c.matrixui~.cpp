/**
 * Copyright (c) 2014 Eliott Paris
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE" in this distribution.
 */

#include "../lib/ep.max.h"

#define io_padding 3.5
#define	io_width 12
#define contrast_white 0.06
#define contrast_black 0.14

typedef struct _pline {
	long	exist;
	long	mouseover;
	long	selected;
} t_pline;

typedef struct  _matrixui
{
	t_pxjbox		j_box;
	
	t_rect			f_rect;
	
	t_jrgba			f_color_bg;
	t_jrgba			f_color_bd;
	t_jrgba			f_color_inputs;
	t_jrgba			f_color_outputs;
	t_jrgba			f_color_patchlines;
	
	long			f_cornersize;
	long			f_bordersize;
	
	long			f_number_of_inputs;
	long			f_number_of_outputs;
	
	long			f_input_over_index;
	long			f_output_over_index;
	
	t_pt			f_last_mouse_move;
	t_pt			f_last_mouse_drag;
	int				f_mouse_dragging;
	
	t_pline**		f_matrix;
} t_matrixui;

void *matrixui_new(t_symbol *s, int argc, t_atom *argv);
void matrixui_free(t_matrixui *x);
void matrixui_assist(t_matrixui *x, void *b, long m, long a, char *s);
void matrixui_dsp64(t_matrixui *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void matrixui_perform64(t_matrixui *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);

t_max_err matrixui_notify(t_matrixui *x, t_symbol *s, t_symbol *msg, void *sender, void *data);
void matrixui_getdrawparams(t_matrixui *x, t_object *patcherview, t_jboxdrawparams *params);

void matrixui_preset(t_matrixui *x);
t_max_err matrixui_setvalueof(t_matrixui *x, long ac, t_atom *av);
t_max_err matrixui_getvalueof(t_matrixui *x, long *ac, t_atom **av);

// -- mousing methods -- //

void matrixui_mouse_move(t_matrixui *x, t_object *patcherview, t_pt pt, long modifiers);
void matrixui_mouse_leave(t_matrixui *x, t_object *patcherview, t_pt pt, long modifiers);
void matrixui_mouse_down(t_matrixui *x, t_object *patcherview, t_pt pt, long modifiers);
void matrixui_mouse_up(t_matrixui *x, t_object *patcherview, t_pt pt, long modifiers);
void matrixui_mouse_drag(t_matrixui *x, t_object *patcherview, t_pt pt, long modifiers);
long matrixui_key(t_matrixui *x, t_object *patcherview, long keycode, long modifiers, long textcharacter);

// -- attrs settings methods -- //

t_max_err matrixui_setattr_number_of_inputs(t_matrixui *x, t_symbol *s, long ac, t_atom* av);
t_max_err matrixui_setattr_number_of_outputs(t_matrixui *x, t_symbol *s, long ac, t_atom* av);

// -- drawing methods -- //

void matrixui_paint(t_matrixui *x, t_object *view);
void draw_background(t_matrixui *x, t_object *view, t_rect *rect);
void draw_IO(t_matrixui *x, t_object *view, t_rect *rect);
void draw_patchlines(t_matrixui *x, t_object *view, t_rect *rect);

// -- messages methods -- //
void matrixui_clear(t_matrixui *x);
void matrixui_list(t_matrixui *x, t_symbol* s, long ac, t_atom* av);

t_class *matrixui_class;

int C74_EXPORT main()
{
	t_class *c;
    
	c = class_new("c.matrixui~", (method)matrixui_new, (method)matrixui_free, (short)sizeof(t_matrixui), 0L, A_GIMME, 0);
    
	c->c_flags |= CLASS_FLAG_NEWDICTIONARY;
	class_dspinitjbox(c);
	jbox_initclass(c, JBOX_COLOR);

	class_addmethod(c, (method) matrixui_dsp64,				"dsp64",			A_CANT, 0);
	class_addmethod(c, (method) matrixui_assist,			"assist",			A_CANT,	0);
	class_addmethod(c, (method) matrixui_paint,				"paint",			A_CANT,	0);
	class_addmethod(c, (method) matrixui_getdrawparams,		"getdrawparams",	A_CANT, 0);
	class_addmethod(c, (method) matrixui_notify,			"notify",			A_CANT, 0);
	
	class_addmethod(c, (method) matrixui_clear,				"clear",			A_NOTHING, 0);
	class_addmethod(c, (method) matrixui_list,				"list",				A_GIMME, 0);
	
	class_addmethod(c, (method) matrixui_preset,			"preset",			0);
    class_addmethod(c, (method) matrixui_getvalueof,		"getvalueof",		A_CANT, 0);
	class_addmethod(c, (method) matrixui_setvalueof,		"setvalueof",		A_CANT, 0);
	
	class_addmethod(c, (method) matrixui_mouse_down,		"mousedown",		A_CANT, 0);
	class_addmethod(c, (method) matrixui_mouse_move,		"mousemove",		A_CANT, 0);
	class_addmethod(c, (method) matrixui_mouse_leave,		"mouseleave",		A_CANT, 0);
	class_addmethod(c, (method) matrixui_mouse_drag,		"mousedrag",		A_CANT, 0);
	class_addmethod(c, (method) matrixui_mouse_up,			"mouseup",			A_CANT, 0);
	class_addmethod(c, (method) matrixui_key,				"key",				A_CANT, 0);
    
	CLASS_ATTR_INVISIBLE            (c, "color", 0);
	CLASS_ATTR_INVISIBLE            (c, "textcolor", 0);
	CLASS_ATTR_DEFAULT              (c, "patching_rect", 0, "0 0 225 125");
    
	// -- Color -- //
	
	CLASS_STICKY_CATEGORY           (c, 0, "Color");
	CLASS_ATTR_RGBA                 (c, "bgcolor", 0, t_matrixui, f_color_bg);
	CLASS_ATTR_STYLE                (c, "bgcolor", 0, "rgba");
	CLASS_ATTR_LABEL                (c, "bgcolor", 0, "Background Color");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bgcolor", 0, "0.95 0.95 0.95 1.");
    
	CLASS_ATTR_RGBA                 (c, "bdcolor", 0, t_matrixui, f_color_bd);
	CLASS_ATTR_STYLE                (c, "bdcolor", 0, "rgba");
	CLASS_ATTR_LABEL                (c, "bdcolor", 0, "Border Color");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "bdcolor", 0, "0.7 0.7 0.7 1.");
	
	CLASS_ATTR_RGBA                 (c, "incolor", 0, t_matrixui, f_color_inputs);
	CLASS_ATTR_STYLE                (c, "incolor", 0, "rgba");
	CLASS_ATTR_LABEL                (c, "incolor", 0, "Inlets Color");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "incolor", 0, "0.4 0.4 0.4 0.8");
	
	CLASS_ATTR_RGBA                 (c, "outcolor", 0, t_matrixui, f_color_outputs);
	CLASS_ATTR_STYLE                (c, "outcolor", 0, "rgba");
	CLASS_ATTR_LABEL                (c, "outcolor", 0, "Outlets Color");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "outcolor", 0, "0.4 0.4 0.4 0.8");
	
	CLASS_ATTR_RGBA                 (c, "plinecolor", 0, t_matrixui, f_color_patchlines);
	CLASS_ATTR_STYLE                (c, "plinecolor", 0, "rgba");
	CLASS_ATTR_LABEL                (c, "plinecolor", 0, "Patchlines Color");
	CLASS_ATTR_DEFAULT_SAVE_PAINT   (c, "plinecolor", 0, "0.85 1. 0.51 1.");
	
	CLASS_STICKY_CATEGORY_CLEAR (c);
	
	// -- Appearance -- //
	
	CLASS_STICKY_CATEGORY           (c, 0, "Appearance");
	CLASS_ATTR_LONG                 (c, "cornersize", 0, t_matrixui, f_cornersize);
	CLASS_ATTR_LABEL               (c, "cornersize", 0, "Corner Size");
	CLASS_ATTR_DEFAULT              (c, "cornersize", 0, "0");
	CLASS_ATTR_FILTER_MIN			(c, "cornersize", 0);
	CLASS_ATTR_SAVE                 (c, "cornersize", 1);
	
	CLASS_ATTR_LONG					(c, "border", 0, t_matrixui, f_bordersize);
	CLASS_ATTR_LABEL                (c, "border", 0, "Border Size");
	CLASS_ATTR_DEFAULT              (c, "border", 0, "1");
	CLASS_ATTR_FILTER_MIN			(c, "border", 0);
	CLASS_ATTR_SAVE                 (c, "border", 1);
	CLASS_STICKY_CATEGORY_CLEAR (c);
	
	// -- Behavior -- //
	
	CLASS_STICKY_CATEGORY           (c, 0, "Value");
	CLASS_ATTR_LONG                 (c, "inputs", 0, t_matrixui, f_number_of_inputs);
	CLASS_ATTR_LABEL                (c, "inputs", 0, "Number of Inputs");
	CLASS_ATTR_ACCESSORS            (c, "inputs", NULL, matrixui_setattr_number_of_inputs);
	CLASS_ATTR_DEFAULT              (c, "inputs", 0, "1");
	CLASS_ATTR_SAVE                 (c, "inputs", 1);
	
	CLASS_ATTR_LONG                 (c, "outputs", 0, t_matrixui, f_number_of_outputs);
    CLASS_ATTR_LABEL                (c, "outputs", 0, "Number of Outputs");
	CLASS_ATTR_ACCESSORS            (c, "outputs", NULL, matrixui_setattr_number_of_outputs);
    CLASS_ATTR_DEFAULT              (c, "outputs", 0, "2");
    CLASS_ATTR_SAVE                 (c, "outputs", 1);
	CLASS_STICKY_CATEGORY_CLEAR (c);
	
	ep_print_credit();
	class_register(CLASS_BOX, c);
	matrixui_class = c;
    
	return 0;
}

void *matrixui_new(t_symbol *s, int argc, t_atom *argv)
{
	t_matrixui *x =  NULL; 
	t_dictionary *d;
	long flags;
	
	if (!(d = object_dictionaryarg(argc,argv)))
		return NULL;

	x = (t_matrixui *)object_alloc(matrixui_class);
	flags = 0 
			| JBOX_DRAWFIRSTIN 
			| JBOX_DRAWINLAST
			| JBOX_TRANSPARENT
			| JBOX_GROWBOTH
			| JBOX_FOCUS
			| JBOX_HILITE
			//| JBOX_DRAWBACKGROUND
			;
    
	jbox_new((t_jbox *)x, flags, argc, argv);
	x->j_box.z_box.b_firstin = (t_object *)x;
	
	x->f_number_of_inputs = 1;
	x->f_number_of_outputs = 2;
	
	x->f_input_over_index = x->f_output_over_index = -1;
	x->f_mouse_dragging = 0;
	
	x->f_matrix = new t_pline*[x->f_number_of_inputs];
	for (int i = 0; i < x->f_number_of_inputs; i++)
		x->f_matrix[i] = new t_pline[x->f_number_of_outputs];
	
	for (int i = 0; i < x->f_number_of_inputs; i++)
	{
		for (int j = 0; j < x->f_number_of_outputs; j++)
		{
			x->f_matrix[i][j].exist = 0;
			x->f_matrix[i][j].mouseover = 0;
			x->f_matrix[i][j].selected = 0;
		}
	}
	
	dsp_setupjbox((t_pxjbox *)x, x->f_number_of_inputs);
	for(int i = 0; i < x->f_number_of_outputs; i++)
		outlet_new(x, "signal");
	
	x->j_box.z_misc = Z_NO_INPLACE;

	attr_dictionary_process(x, d);
	jbox_ready((t_jbox *)x);

	return (x);
}

void matrixui_clear(t_matrixui *x)
{
	for (int i = 0; i < x->f_number_of_inputs; i++)
	{
		x->f_matrix[i] = new t_pline[x->f_number_of_outputs];
		for (int j = 0; j < x->f_number_of_outputs; j++)
		{
			x->f_matrix[i][j].exist = 0;
			x->f_matrix[i][j].mouseover = 0;
			x->f_matrix[i][j].selected = 0;
		}
	}
	
	jbox_invalidate_layer((t_object *)x, NULL, gensym("patchlines_layer"));
	jbox_redraw((t_jbox *)x);
}

void matrixui_list(t_matrixui *x, t_symbol* s, long ac, t_atom* av)
{
	long inlet, outlet, value;
	if (ac >= 3 && av)
	{
		for (int i=0; i<ac; i+=3)
		{
			if (i+3 <= ac && atom_gettype(av+i) == A_LONG && atom_gettype(av+i+1) == A_LONG && atom_gettype(av+i+2) == A_LONG)
			{
				inlet = atom_getlong(av+i);
				outlet = atom_getlong(av+i+1);
				value = atom_getlong(av+i+2);
				if (isInside(inlet, 0, x->f_number_of_inputs-1) && isInside(outlet, 0, x->f_number_of_outputs-1))
				{
					x->f_matrix[inlet][outlet].exist = (value > 0);
				}
			}
		}
	}
	
	jbox_invalidate_layer((t_object *)x, NULL, gensym("patchlines_layer"));
	jbox_redraw((t_jbox *)x);
}

t_max_err matrixui_setattr_number_of_inputs(t_matrixui *x, t_symbol *s, long ac, t_atom* av)
{
	if (ac && av && (atom_gettype(av) == A_LONG || atom_gettype(av) == A_FLOAT))
	{
		t_object *b = NULL;
		object_obex_lookup(x, gensym("#B"), (t_object **)&b);
		
		if (b)
		{
			long last_number_of_inputs = x->f_number_of_inputs;
			x->f_number_of_inputs = clip_min(atom_getlong(av), 1);
			
			x->f_input_over_index = x->f_output_over_index = -1;
			x->f_mouse_dragging = 0;
			
			// copy matrix in a temp memory
			t_pline** temp_matrix;
			temp_matrix = new t_pline*[last_number_of_inputs];
			for (int i = 0; i < last_number_of_inputs; i++)
			{
				temp_matrix[i] = new t_pline[x->f_number_of_outputs];
				
				for (int j = 0; j < x->f_number_of_outputs; j++)
				{
					temp_matrix[i][j].exist = x->f_matrix[i][j].exist;
					temp_matrix[i][j].mouseover = x->f_matrix[i][j].mouseover;
					temp_matrix[i][j].selected = x->f_matrix[i][j].selected;
				}
			}
			
			// create a new matrix
			for (int i = 0; i < last_number_of_inputs; i++)
				delete [] x->f_matrix[i];
			delete [] x->f_matrix;
			
			x->f_matrix = new t_pline*[x->f_number_of_inputs];
			for (int i = 0; i < x->f_number_of_inputs; i++)
			{
				x->f_matrix[i] = new t_pline[x->f_number_of_outputs];
				for (int j = 0; j < x->f_number_of_outputs; j++)
				{
					if (i < last_number_of_inputs)
					{
						x->f_matrix[i][j].exist = temp_matrix[i][j].exist;
						x->f_matrix[i][j].mouseover = temp_matrix[i][j].mouseover;
						x->f_matrix[i][j].selected = temp_matrix[i][j].selected;
					}
					else
					{
						x->f_matrix[i][j].exist = 0;
						x->f_matrix[i][j].mouseover = 0;
						x->f_matrix[i][j].selected = 0;
					}
				}
			}
			
			for (int i = 0; i < last_number_of_inputs; i++)
				delete [] temp_matrix[i];
			delete [] temp_matrix;
			
			
			int dspState = sys_getdspobjdspstate((t_object*)x);
			if(dspState)
				object_method(gensym("dsp")->s_thing, ep_sym_stop);
			
			
			object_method(b, ep_sym_dynlet_begin);
			dsp_resize((t_pxobject*)x, x->f_number_of_inputs);
			object_method(b, ep_sym_dynlet_end);
		}
		
		jbox_invalidate_layer((t_object *)x, NULL, gensym("patchlines_layer"));
		jbox_invalidate_layer((t_object *)x, NULL, gensym("io_layer"));
		jbox_redraw((t_jbox *)x);
	}
	
	return MAX_ERR_NONE;
}

t_max_err matrixui_setattr_number_of_outputs(t_matrixui *x, t_symbol *s, long ac, t_atom* av)
{
	if (ac && av && (atom_gettype(av) == A_LONG || atom_gettype(av) == A_FLOAT))
	{
		t_object *b = NULL;
		object_obex_lookup(x, gensym("#B"), (t_object **)&b);
		
		if (b)
		{
			long last_number_of_outputs = x->f_number_of_outputs;
			x->f_number_of_outputs = clip_min(atom_getlong(av), 1);
			
			x->f_input_over_index = x->f_output_over_index = -1;
			x->f_mouse_dragging = 0;
			
			// copy matrix in a temp memory
			t_pline** temp_matrix;
			temp_matrix = new t_pline*[x->f_number_of_inputs];
			for (int i = 0; i < x->f_number_of_inputs; i++)
			{
				temp_matrix[i] = new t_pline[last_number_of_outputs];
				
				for (int j = 0; j < last_number_of_outputs; j++)
				{
					temp_matrix[i][j].exist = x->f_matrix[i][j].exist;
					temp_matrix[i][j].mouseover = x->f_matrix[i][j].mouseover;
					temp_matrix[i][j].selected = x->f_matrix[i][j].selected;
				}
			}
			
			// create a new matrix
			for (int i = 0; i < x->f_number_of_inputs; i++)
				delete [] x->f_matrix[i];
			delete [] x->f_matrix;
			
			x->f_matrix = new t_pline*[x->f_number_of_inputs];
			for (int i = 0; i < x->f_number_of_inputs; i++)
			{
				x->f_matrix[i] = new t_pline[x->f_number_of_outputs];
				for (int j = 0; j < x->f_number_of_outputs; j++)
				{
					if (j < last_number_of_outputs)
					{
						x->f_matrix[i][j].exist = temp_matrix[i][j].exist;
						x->f_matrix[i][j].mouseover = temp_matrix[i][j].mouseover;
						x->f_matrix[i][j].selected = temp_matrix[i][j].selected;
					}
					else
					{
						x->f_matrix[i][j].exist = 0;
						x->f_matrix[i][j].mouseover = 0;
						x->f_matrix[i][j].selected = 0;
					}
				}
			}
			
			for (int i = 0; i < x->f_number_of_inputs; i++)
				delete [] temp_matrix[i];
			delete [] temp_matrix;
			
			/*
			int dspState = sys_getdspobjdspstate((t_object*)x);
			if(dspState)
				object_method(gensym("dsp")->s_thing, ep_sym_stop);
			*/
			
			object_method(b, ep_sym_dynlet_begin);
			
			if(outlet_count((t_object *)x) > x->f_number_of_outputs)
			{
				for(int i = outlet_count((t_object *)x); i > x->f_number_of_outputs; i--)
				{
					outlet_delete(outlet_nth((t_object*)x, i-1));
				}
			}
			else if(outlet_count((t_object *)x) < x->f_number_of_outputs)
			{
				for(int i = outlet_count((t_object *)x); i < x->f_number_of_outputs; i++)
				{
					outlet_append((t_object*)x, NULL, gensym("signal"));
				}
			}
			
			object_method(b, ep_sym_dynlet_end);
		}
		
		jbox_invalidate_layer((t_object *)x, NULL, gensym("patchlines_layer"));
		jbox_invalidate_layer((t_object *)x, NULL, gensym("io_layer"));
		jbox_redraw((t_jbox *)x);
	}
	
	return MAX_ERR_NONE;
}

void matrixui_preset(t_matrixui *x)
{
	/*
	void* z;
	long ac;
	t_atom* av;
	t_atom* avptr;
	
    if(!(z = gensym("_preset")->s_thing))
        return;
	
	ac = (255*255);
	av = (t_atom*)getbytes(ac * sizeof(t_atom));
	avptr = av;
	
	atom_setobj(avptr++, x);
    atom_setsym(avptr++, object_classname(x));
	atom_setsym(avptr++, ep_sym_list);
	atom_setsym(avptr++, hoa_sym_source_preset_data);
	
	binbuf_insert(z, NULL, (MAX_NUMBER_OF_SOURCES * 11 + 4), av);
	freebytes(av, ac * sizeof(t_atom));
	*/
}

t_max_err matrixui_setvalueof(t_matrixui *x, long ac, t_atom *av)
{
	return MAX_ERR_NONE;
}

t_max_err matrixui_getvalueof(t_matrixui *x, long *ac, t_atom **av)
{
	return MAX_ERR_NONE;
}

void matrixui_assist(t_matrixui *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_INLET)
	{
		sprintf(s,"(signal) input %ld", a + 1);
	}
	else
	{
        sprintf(s,"(signal) output %ld", a + 1);
	}
}


void matrixui_dsp64(t_matrixui *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	object_method(dsp64, gensym("dsp_add64"), x, matrixui_perform64, 0, NULL);
}

void matrixui_perform64(t_matrixui *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{
	int i, j, k;
	
	for (i = 0; i < numouts; i++)
		for (j = 0; j < sampleframes; j++)
			outs[i][j] = 0.;
	
	for (i = 0; i < x->f_number_of_inputs; i++)
	{
		for (j = 0; j < x->f_number_of_outputs; j++)
		{
			if (x->f_matrix[i][j].exist)
			{
				for (k = 0; k < sampleframes; k++)
				{
					outs[j][k] += ins[i][k];
				}
			}
		}
	}
}

t_max_err matrixui_notify(t_matrixui *x, t_symbol *s, t_symbol *msg, void *sender, void *data)
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
		else if (name == gensym("incolor") || name == gensym("outcolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("io_layer"));
		}
		else if (name == gensym("plinecolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("patchlines_layer"));
		}
		else if (name == gensym("cursorcolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
		}
		else if(name == gensym("border") || name == gensym("cursordrawmode") || name == gensym("wfdrawmode"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("background_layer"));
			jbox_invalidate_layer((t_object *)x, NULL, gensym("cursor_layer"));
			jbox_invalidate_layer((t_object *)x, NULL, gensym("matrixui_layer"));
		}
		else if (name == gensym("selectioncolor"))
		{
			jbox_invalidate_layer((t_object *)x, NULL, gensym("selection_layer"));
		}
		jbox_redraw((t_jbox *)x);
	}

	err |= jbox_notify((t_jbox *)x, s, msg, sender, data);
	
	return err;
}

void matrixui_getdrawparams(t_matrixui *x, t_object *patcherview, t_jboxdrawparams *params)
{
	params->d_borderthickness = params->d_cornersize = 0;
}

void matrixui_paint(t_matrixui *x, t_object *view)
{
	jbox_get_rect_for_view((t_object *)x, view, &x->f_rect);
	
	draw_background(x, view, &x->f_rect);
	draw_patchlines(x, view, &x->f_rect);
	draw_IO(x, view, &x->f_rect);
}

void draw_background(t_matrixui *x,  t_object *view, t_rect *rect)
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

void draw_IO(t_matrixui *x, t_object *view, t_rect *rect)
{
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, gensym("io_layer"), rect->width, rect->height);
	
	double io_range = rect->width - (2*io_padding + io_width);
	
	t_jrgba black = {0.2, 0.2, 0.2, 1.};
	t_jrgba red = {1., 0., 0., 0.4};
	t_jrgba input_sel_color = x->f_color_inputs;
	vector_add(4, (double*) &input_sel_color, 0.2);
	vector_clip_minmax(4, (double*) &input_sel_color, 0, 1);
	
	t_jrgba output_sel_color = x->f_color_outputs;
	vector_add(4, (double*) &output_sel_color, 0.2);
	vector_clip_minmax(4, (double*) &output_sel_color, 0, 1);
	
	if (g)
	{
		// ----
		// draw inlets :
		// ----
		
		jgraphics_set_line_width(g, 2);
		
		// first inlet :
		jgraphics_arc_negative(g, io_padding + (io_width*0.5), 1, io_width*0.5, EP_PI, 0);
		jgraphics_set_source_jrgba(g, x->f_input_over_index != 0 ? &x->f_color_inputs : &input_sel_color);
		jgraphics_fill_preserve(g);
		if (x->f_input_over_index == 0) jgraphics_set_source_jrgba(g, &black);
		jgraphics_stroke(g);
		
		if (x->f_number_of_inputs > 1)
		{
			// last inlet :
			jgraphics_arc_negative(g, rect->width - io_padding - (io_width*0.5), 1, io_width*0.5, EP_PI, 0);
			jgraphics_set_source_jrgba(g, x->f_input_over_index != (x->f_number_of_inputs - 1) ? &x->f_color_inputs : &input_sel_color);
			jgraphics_fill_preserve(g);
			if (x->f_input_over_index == (x->f_number_of_inputs - 1)) jgraphics_set_source_jrgba(g, &black);
			jgraphics_stroke(g);
			
			for (int i = 1; i < (x->f_number_of_inputs - 1); i++)
			{
				jgraphics_arc_negative(g, (io_padding + (io_width*0.5)) + ( i * io_range / ((x->f_number_of_inputs - 1))), 1, io_width*0.5, EP_PI, 0);
				jgraphics_set_source_jrgba(g, x->f_input_over_index != i ? &x->f_color_inputs : &input_sel_color);
				jgraphics_fill_preserve(g);
				if (x->f_input_over_index == i) jgraphics_set_source_jrgba(g, &black);
				jgraphics_stroke(g);
			}
		}
		
		// ----
		// draw outlets :
		// ----
		
		jgraphics_set_line_width(g, 2);
		
		// first outlet :
		jgraphics_arc(g, io_padding + (io_width*0.5), rect->height - 1, io_width*0.5, EP_PI, 0);
		jgraphics_set_source_jrgba(g, x->f_output_over_index != 0 ? &x->f_color_outputs : &output_sel_color);
		jgraphics_fill_preserve(g);
		if (x->f_output_over_index == 0) jgraphics_set_source_jrgba(g, &black);
		jgraphics_stroke(g);
		
		if (x->f_number_of_outputs > 1)
		{
			// last outlet :
			jgraphics_arc(g, rect->width - io_padding - (io_width*0.5), rect->height - 1, io_width*0.5, EP_PI, 0);
			jgraphics_set_source_jrgba(g, x->f_output_over_index != (x->f_number_of_outputs - 1) ? &x->f_color_outputs : &output_sel_color);
			jgraphics_fill_preserve(g);
			if (x->f_output_over_index == (x->f_number_of_outputs - 1)) jgraphics_set_source_jrgba(g, &black);
			jgraphics_stroke(g);
			
			for (int i = 1; i < (x->f_number_of_outputs - 1); i++)
			{
				jgraphics_arc(g, (io_padding + (io_width*0.5)) + ( i * io_range / ((x->f_number_of_outputs - 1))), rect->height - 1, io_width*0.5, EP_PI, 0);
				jgraphics_set_source_jrgba(g, x->f_output_over_index != i ? &x->f_color_outputs : &output_sel_color);
				jgraphics_fill_preserve(g);
				if (x->f_output_over_index == i) jgraphics_set_source_jrgba(g, &black);
				jgraphics_stroke(g);
			}
		}
		
		// ----
		// draw connexion indicators :
		// ----
		
		if (x->f_mouse_dragging && (x->f_input_over_index > -1 || x->f_output_over_index > -1))
		{
			if (x->f_input_over_index > -1 && isInside(x->f_last_mouse_drag.y, x->f_rect.height * 0.5, x->f_rect.height))
			{
				for (int i = 0; i < x->f_number_of_outputs; i++)
				{
					if (isInside(x->f_last_mouse_drag.x, i * (x->f_rect.width / x->f_number_of_outputs), (i+1) * (x->f_rect.width / x->f_number_of_outputs)))
					{
						// first outlet :
						if (i == 0) {
							jgraphics_arc(g, io_padding + (io_width*0.5), rect->height - 1, io_width*0.75, EP_PI, 0);
						}
						else if(i == x->f_number_of_outputs-1)
						{
							jgraphics_arc(g, rect->width - io_padding - (io_width*0.5), rect->height - 1, io_width*0.75, EP_PI, 0);
						}
						else
						{
							jgraphics_arc(g, (io_padding + (io_width*0.5)) + ( i * io_range / ((x->f_number_of_outputs - 1))), rect->height - 1, io_width*0.75, EP_PI, 0);
						}
						jgraphics_set_source_jrgba(g, &red);
						jgraphics_stroke(g);
					}
				}
			}
		}
		
		jbox_end_layer((t_object*)x, view, gensym("io_layer"));
	}
	jbox_paint_layer((t_object *)x, view, gensym("io_layer"), 0., 0.);
}

void draw_patchlines(t_matrixui *x, t_object *view, t_rect *rect)
{
	t_jgraphics *g = jbox_start_layer((t_object *)x, view, gensym("patchlines_layer"), rect->width, rect->height);
	
	t_rect pline_rect;
	double io_range = rect->width - (2*io_padding + io_width);
	
	t_jrgba black = {0.2, 0.2, 0.2, 1.};
	t_jrgba black_shadow = {0.2, 0.2, 0.2, 0.2};
	t_jrgba sel_color = {0., 0., 0.7, 0.2};
	
	double dashes[] = {4, 2};
	
	if (g)
	{
		for (int i = 0; i < x->f_number_of_inputs; i++)
		{
			for (int j = 0; j < x->f_number_of_outputs; j++)
			{
				x->f_matrix[i][j].mouseover = 0;
				
				if (x->f_matrix[i][j].exist == 1)
				{
					//post("x->f_matrix[%i][%i] = %ld", i, j, x->f_matrix[i][j]);
					
					pline_rect.y = 2;
					pline_rect.height = rect->height - 2;
					
					if (i == 0)
						pline_rect.x = io_padding + (io_width*0.5);
					else if (i == (x->f_number_of_inputs - 1))
						pline_rect.x = rect->width - io_padding - (io_width*0.5);
					else
						pline_rect.x = (io_padding + (io_width*0.5)) + ( i * io_range / ((x->f_number_of_inputs - 1)));
					
					// line end
					if (j == 0)
						pline_rect.width = io_padding + (io_width*0.5);
					else if (j == (x->f_number_of_outputs - 1))
						pline_rect.width = rect->width - io_padding - (io_width*0.5);
					else
						pline_rect.width = (io_padding + (io_width*0.5)) + ( j * io_range / ((x->f_number_of_outputs - 1)));
					
					pline_rect.width -= pline_rect.x;
					jgraphics_move_to(g, pline_rect.x, pline_rect.y);
					jgraphics_curve_to(g,
									   pline_rect.x, pline_rect.height*0.4,
									   pline_rect.x + pline_rect.width, pline_rect.height - pline_rect.height*0.4,
									   pline_rect.x + pline_rect.width, pline_rect.height);
					
					x->f_matrix[i][j].mouseover = jgraphics_path_intersects_line(g, x->f_last_mouse_move.x-2, x->f_last_mouse_move.y, x->f_last_mouse_move.x+2, x->f_last_mouse_move.y);
					
					
					jgraphics_set_dash(g, NULL, 0, 0);
					
					if (x->f_matrix[i][j].mouseover)
					{
						jgraphics_set_line_width(g, 5);
						jgraphics_set_source_jrgba(g, &black_shadow);
						jgraphics_stroke_preserve(g);
					}
					
					jgraphics_set_line_width(g, 2.5);
					jgraphics_set_source_jrgba(g, &black);
					jgraphics_stroke_preserve(g);
					
					jgraphics_set_line_width(g, 1.5);
					jgraphics_set_dash(g, dashes, 2, 0);
					jgraphics_set_source_jrgba(g, &x->f_color_patchlines);
					
					if (x->f_matrix[i][j].selected)
					{
						jgraphics_stroke_preserve(g);
						
						jgraphics_set_dash(g, NULL, 0, 0);
						jgraphics_set_line_width(g, 5);
						jgraphics_set_source_jrgba(g, &sel_color);
						jgraphics_stroke(g);
					}
					else
						jgraphics_stroke(g);
				}
			}
		}
		
		
		if (x->f_mouse_dragging && (x->f_input_over_index > -1 || x->f_output_over_index > -1))
		{
			jgraphics_set_line_width(g, 2.5);
			jgraphics_set_source_jrgba(g, &black);
			jgraphics_set_dash(g, NULL, 0, 0);
			
			if (x->f_input_over_index > -1)
			{
				if (x->f_input_over_index == 0)
				{
					pline_rect.x = io_padding + (io_width*0.5);
					pline_rect.y = 2;
					pline_rect.width = x->f_last_mouse_drag.x - pline_rect.x;
					pline_rect.height = x->f_last_mouse_drag.y - pline_rect.y;
				}
				else if (x->f_number_of_inputs > 1)
				{
					if (x->f_input_over_index == (x->f_number_of_inputs - 1))
					{
						pline_rect.x = rect->width - io_padding - (io_width*0.5);
						pline_rect.y = 2;
						pline_rect.width = x->f_last_mouse_drag.x - pline_rect.x;
						pline_rect.height = x->f_last_mouse_drag.y - pline_rect.y;
					}
					else
					{
						for (int i = 1; i < (x->f_number_of_inputs - 1); i++)
						{
							if (i == x->f_input_over_index)
							{
								pline_rect.x = (io_padding + (io_width*0.5)) + ( i * io_range / ((x->f_number_of_inputs - 1)));
								pline_rect.y = 2;
								pline_rect.width = x->f_last_mouse_drag.x - pline_rect.x;
								pline_rect.height = x->f_last_mouse_drag.y - pline_rect.y;
							}
						}
					}
				}
				
				jgraphics_move_to(g, pline_rect.x, pline_rect.y);
				jgraphics_curve_to(g,
								   pline_rect.x, pline_rect.height*0.4,
								   x->f_last_mouse_drag.x, pline_rect.height - pline_rect.height*0.4,
								   x->f_last_mouse_drag.x, x->f_last_mouse_drag.y);
				
				jgraphics_stroke_preserve(g);
				jgraphics_set_line_width(g, 1.5);
				jgraphics_set_dash(g, dashes, 2, 0);
				jgraphics_set_source_jrgba(g, &x->f_color_patchlines);
				jgraphics_stroke(g);
			}
		}
		
		jbox_end_layer((t_object*)x, view, gensym("patchlines_layer"));
	}
	jbox_paint_layer((t_object *)x, view, gensym("patchlines_layer"), 0., 0.);
}

/**********************************************************/
/*                      Souris                            */
/**********************************************************/

void matrixui_mouse_move(t_matrixui *x, t_object *patcherview, t_pt pt, long modifiers)
{
	x->f_mouse_dragging = 0;
	x->f_input_over_index = x->f_output_over_index = -1;
	double io_range = x->f_rect.width - (2*io_padding + io_width);
	
	// test if mouse is over inlets
	if (isInside(pt.y, 0, io_width + 2))
	{
		if (isInside(pt.x, io_padding, io_padding + io_width))
			x->f_input_over_index = 0;
		else if (x->f_number_of_inputs > 1)
		{
			if (isInside(pt.x, x->f_rect.width - io_padding - io_width, x->f_rect.width - io_padding))
				x->f_input_over_index = x->f_number_of_inputs - 1;
			else
			{
				for (int i = 1; i < (x->f_number_of_inputs - 1); i++)
				{
					if(isInside(pt.x, io_padding + ( i * io_range / ((x->f_number_of_inputs - 1))), io_padding + io_width + ( i * io_range / ((x->f_number_of_inputs - 1)))))
					{
						x->f_input_over_index = i;
					}
				}
			}
		}
	}
	else if (isInside(pt.y, x->f_rect.height - io_width + 2, x->f_rect.height))
	{
		if (isInside(pt.x, io_padding, io_padding + io_width))
			x->f_output_over_index = 0;
		else if (x->f_number_of_outputs > 1)
		{
			if (isInside(pt.x, x->f_rect.width - io_padding - io_width, x->f_rect.width - io_padding))
				x->f_output_over_index = x->f_number_of_outputs - 1;
			else
			{
				for (int i = 1; i < (x->f_number_of_outputs - 1); i++)
				{
					if(isInside(pt.x, io_padding + ( i * io_range / ((x->f_number_of_outputs - 1))), io_padding + io_width + ( i * io_range / ((x->f_number_of_outputs - 1)))))
					{
						x->f_output_over_index = i;
					}
				}
			}
		}
	}
	
	x->f_last_mouse_move = pt;
	
	jbox_invalidate_layer((t_object *)x, NULL, gensym("io_layer"));
	jbox_invalidate_layer((t_object *)x, NULL, gensym("patchlines_layer"));
    jbox_redraw((t_jbox *)x);
}

void matrixui_mouse_leave(t_matrixui *x, t_object *patcherview, t_pt pt, long modifiers)
{
	x->f_mouse_dragging = 0;
	x->f_input_over_index = x->f_output_over_index = -1;
	jbox_invalidate_layer((t_object *)x, NULL, gensym("io_layer"));
	jbox_redraw((t_jbox *)x);
}

void matrixui_mouse_down(t_matrixui *x, t_object *patcherview, t_pt pt, long modifiers)
{
	long shift = (modifiers == 18);
	
	for (int i=0; i<x->f_number_of_inputs; i++)
	{
		for (int j=0; j<x->f_number_of_outputs; j++)
		{
			if (!shift)
				x->f_matrix[i][j].selected = 0;
			
			if (x->f_matrix[i][j].mouseover)
			{
				// reverse selection if shift is holded
				if (shift)
					x->f_matrix[i][j].selected = !x->f_matrix[i][j].selected;
				else
					x->f_matrix[i][j].selected = 1;
			}
		}
	}
	jbox_invalidate_layer((t_object *)x, NULL, gensym("patchlines_layer"));
	jbox_redraw((t_jbox *)x);
}

void matrixui_mouse_up(t_matrixui *x, t_object *patcherview, t_pt pt, long modifiers)
{
	if ( x->f_mouse_dragging && (x->f_input_over_index > -1 || x->f_output_over_index > -1))
	{
		if (x->f_input_over_index > -1 && isInside(x->f_last_mouse_drag.y, x->f_rect.height * 0.5, x->f_rect.height))
		{
			for (int i = 0; i < x->f_number_of_outputs; i++)
			{
				if (isInside(x->f_last_mouse_drag.x, i * (x->f_rect.width / x->f_number_of_outputs), (i+1) * (x->f_rect.width / x->f_number_of_outputs)))
				{
					x->f_matrix[x->f_input_over_index][i].exist = 1;
				}
			}
		}
	}
	
	x->f_mouse_dragging = 0;
	
	jbox_invalidate_layer((t_object *)x, NULL, gensym("io_layer"));
	jbox_invalidate_layer((t_object *)x, NULL, gensym("patchlines_layer"));
	jbox_redraw((t_jbox *)x);
}

void matrixui_mouse_drag(t_matrixui *x, t_object *patcherview, t_pt pt, long modifiers)
{
	x->f_mouse_dragging = 1;
	x->f_last_mouse_drag = pt;
	jbox_invalidate_layer((t_object *)x, NULL, gensym("io_layer"));
	jbox_invalidate_layer((t_object *)x, NULL, gensym("patchlines_layer"));
	jbox_redraw((t_jbox *)x);
}

long matrixui_key(t_matrixui *x, t_object *patcherview, long keycode, long modifiers, long textcharacter)
{
	int filter = 0;
	
#ifdef _WINDOWS
	if (keycode == 97 && modifiers == 5 && textcharacter == 1) // Control + a
#else
	if (keycode == 97 && modifiers == 1 && textcharacter == 0) //cmd+a
#endif
	{
		filter = 1;
		
		for (int i=0; i<x->f_number_of_inputs; i++)
			for (int j=0; j<x->f_number_of_outputs; j++)
				x->f_matrix[i][j].selected = 1;
		
		jbox_invalidate_layer((t_object *)x, NULL, gensym("patchlines_layer"));
		jbox_redraw((t_jbox *)x);
	}
	else if (textcharacter == 127) // backspace key
	{
		filter = 1;
		
		for (int i=0; i<x->f_number_of_inputs; i++)
		{
			for (int j=0; j<x->f_number_of_outputs; j++)
			{
				if (x->f_matrix[i][j].exist == 1 && x->f_matrix[i][j].selected)
				{
					x->f_matrix[i][j].exist = 0;
					x->f_matrix[i][j].mouseover = 0;
					x->f_matrix[i][j].selected = 0;
				}
			}
		}
		
		jbox_invalidate_layer((t_object *)x, NULL, gensym("patchlines_layer"));
		jbox_redraw((t_jbox *)x);
	}
	return filter;
}

void matrixui_free(t_matrixui *x)
{
	jbox_free((t_jbox *)x);
	
	for (int i = 0; i < x->f_number_of_inputs; i++)
		delete [] x->f_matrix[i];
	delete [] x->f_matrix;
}