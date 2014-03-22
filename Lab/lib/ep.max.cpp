/*
// Copyright (c) 2012-2014 Eliott Paris, Julien Colafrancesco & Pierre Guillot, CICM, Universite Paris 8.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "ep.max.h"

void ep_print_credit()
{
    if(gensym("ep_print_credits")->s_thing == NULL)
    {
		t_object* print;
		t_atom* sym = (t_atom*) malloc( sizeof(t_atom));
		atom_setsym(sym, gensym("EpLib Beta 0.1"));
		print = (t_object*)object_new_typed(CLASS_BOX, gensym("print"), 1, sym);
		atom_setsym(sym, gensym("Copyright \u00a9 2014, Eliott Paris"));
		object_method_typed(print, gensym("list"), 1, sym, NULL);
		gensym("hoa_print_credits")->s_thing = print;
		freeobject(print);
		free(sym);
    }
}