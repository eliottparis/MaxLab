/**
 Send notification (MacOsX >= 10.8 only)
*/

#include "../lib/ep.max.h"

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

using namespace std;

typedef struct _notif
{
	t_object p_ob;
	
	t_atom		f_title[20];
	long		f_title_size;
	
	t_atom		f_subtitle[50];
	long		f_subtitle_size;
	
	t_atom		f_description[120];
	long		f_description_size;
	
	long		f_playsound;
	
	t_symbol*	f_image;
	t_symbol*	f_image_path;
	long		f_image_valid;
	
	void *p_outlet;
} t_notif;

void notif_bang(t_notif *x);
void notif_int(t_notif *x, long n);
void notif_assist(t_notif *x, void *b, long m, long a, char *s);
void *notif_new(t_symbol *s, int ac, t_atom *av);
void notif_free(t_notif *x);

void notif_anything(t_notif *x, t_symbol *s, long ac, t_atom *av);

short doNotification(const string& title, const string& subtitle, const string& description, const string& image, bool playSound);
t_max_err notif_attrset_image(t_notif *x, t_object *attr, long ac, t_atom *av);
t_max_err notif_find_image(t_notif *x, t_symbol* s);

t_class *notif_class;


//--------------------------------------------------------------------------

int C74_EXPORT main(void)
{
	t_class *c;
	
	c = class_new("c.notification", (method)notif_new, (method)notif_free, sizeof(t_notif), 0L, A_GIMME, 0);
	
	//class_addmethod(c, (method)notif_anything,		"anything",		A_GIMME,	0);
    class_addmethod(c, (method)notif_bang,			"bang",						0);
    //class_addmethod(c, (method)notif_int,			"int",			A_LONG,		0);
    class_addmethod(c, (method)notif_assist,		"assist",		A_CANT,		0);
	
	CLASS_ATTR_ATOM_VARSIZE		(c, "title",	0, t_notif, f_title, f_title_size, 20);
	//CLASS_ATTR_SYM				(c, "title",	0, t_notif, f_title);
	CLASS_ATTR_CATEGORY			(c, "title",	0, "Notification");
    CLASS_ATTR_LABEL			(c, "title",	0, "Notification title");
	CLASS_ATTR_ORDER			(c, "title",	0, "1");
	CLASS_ATTR_SAVE				(c, "title",	1);
    // @description The <b>title</b> attribute define the title of the scheduled notifications.
	
	CLASS_ATTR_ATOM_VARSIZE		(c, "subtitle",	0, t_notif, f_subtitle, f_subtitle_size, 50);
	//CLASS_ATTR_SYM				(c, "subtitle",	0, t_notif, f_subtitle);
	CLASS_ATTR_CATEGORY			(c, "subtitle",	0, "Notification");
    CLASS_ATTR_LABEL			(c, "subtitle",	0, "Notification subtitle");
	CLASS_ATTR_ORDER			(c, "subtitle",	0, "2");
	CLASS_ATTR_SAVE				(c, "subtitle",	1);
    // @description The <b>subtitle</b> attribute define the subtitle of the scheduled notifications.
	
	CLASS_ATTR_ATOM_VARSIZE		(c, "description",	0, t_notif, f_description, f_description_size, 120);
	CLASS_ATTR_CATEGORY			(c, "description",	0, "Notification");
    CLASS_ATTR_STYLE_LABEL		(c, "description",	0, "text_large", "Notification description");
	CLASS_ATTR_ORDER			(c, "description",	0, "3");
	CLASS_ATTR_SAVE				(c, "description",	1);
    // @description The <b>description</b> attribute define the description of the scheduled notifications.
	
	CLASS_ATTR_LONG             (c, "playsound",  0, t_notif, f_playsound);
	CLASS_ATTR_CATEGORY			(c, "playsound",  0, "Notification");
    CLASS_ATTR_STYLE_LABEL      (c, "playsound",  0, "onoff", "Notification sound");
	CLASS_ATTR_ORDER			(c, "playsound",  0, "4");
	CLASS_ATTR_SAVE				(c, "playsound",  1);
    // @description If the <b>playsound</b> attribute is checked, <o>c.notification</o> object will play a sound when a notification arrive (only when Max is in background).
	
	CLASS_ATTR_SYM				(c, "image",  0, t_notif, f_image);
	CLASS_ATTR_CATEGORY			(c, "image",  0, "Notification");
    CLASS_ATTR_STYLE_LABEL      (c, "image",  0, "imagefile", "Notification image");
	CLASS_ATTR_ACCESSORS		(c, "image", NULL, notif_attrset_image);
	CLASS_ATTR_ORDER			(c, "image",  0, "5");
	CLASS_ATTR_SAVE				(c, "image",  1);
    // @description The <b>image</b> attribute could be use to add an image the scheduled notifications.
	
	
	ep_print_credit();
	
	class_register(CLASS_BOX, c);
	notif_class = c;
	return 0;
}

//--------------------------------------------------------------------------

void *notif_new(t_symbol *s, int ac, t_atom *av)
{
	t_notif *x = (t_notif *)object_alloc(notif_class);
	
	if (x)
	{
		#if (MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_8)
			object_error((t_object*)x, "Notifications require Mac OSX 10.8 min");
		#endif
		
		//x->f_title = x->f_subtitle = x->f_description = ep_sym_nothing;
		//x->f_title = x->f_subtitle = ep_sym_nothing;
		
		//x->f_description = (t_atom*) getbytes(120 * sizeof(t_atom));
		
		x->f_image_path = ep_sym_nothing;
		x->f_image = ep_sym_nothing;
		x->f_playsound = 1;
		
		x->p_outlet = outlet_new(x, NULL);
		
		attr_args_process(x, ac, av);
	}
	
	return(x);
}

void notif_free(t_notif *x)
{
	;
}

t_max_err notif_attrset_image(t_notif *x, t_object *attr, long ac, t_atom *av)
{
	if (ac && atom_gettype(av) == A_SYM)
	{
		x->f_image = atom_getsym(av);
		defer_low(x, (method)notif_find_image, x->f_image, 0, NULL);
	}
	return MAX_ERR_NONE;
}

t_max_err notif_find_image(t_notif *x, t_symbol* s)
{
	string imagepath = "";
	x->f_image_path = ep_sym_nothing;
	x->f_image_valid = 0;
	
	short path;
	t_fourcc filetype[] = {'PNG ', 'JPEG', 'GIFf', 'PICT', 'PICS'};
	t_fourcc outtype = NULL;
	
	char filename[MAX_FILENAME_CHARS];
	char fullpath[MAX_PATH_CHARS];
	string s_fullpath = "";
	
	if (s != ep_sym_nothing)
	{
		strncpy_zero(filename, s->s_name, MAX_FILENAME_CHARS);
		if (!locatefile_extended(filename, &path, &outtype, filetype, 5))   // non-zero: not found
		{
			if(!path_topathname(path, filename, fullpath))
			{
				imagepath = fullpath;
				unsigned found = imagepath.find_first_of('/');
				imagepath = imagepath.substr(found);
				x->f_image_valid = 1;
				x->f_image_path = gensym(imagepath.c_str());
			}
			else
			{
				object_error((t_object*)x, "can't find %s image file", filename);
			}
		}
		else
		{
			object_error((t_object*)x, "can't find %s image file", filename);
		}
	}
	
	return MAX_ERR_NONE;
}


//--------------------------------------------------------------------------

void notif_assist(t_notif *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET)
	{
		sprintf(s,"Nothing yet !");
	}
	else
	{
		switch (a)
		{
		case 0:
			sprintf(s,"Bang schedules a new notification");
			break;
		default:
			break;
		}
	}
}

void notif_anything(t_notif *x, t_symbol *s, long ac, t_atom *av)
{
	;
}

void notif_bang(t_notif *x)
{
	char *title, *subtitle, *description;
	long title_size, subtitle_size, description_size;
	title = subtitle = description = NULL;
	title_size = subtitle_size = description_size = 0;
	
	atom_gettext(x->f_title_size, x->f_title, &title_size, &title, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
	atom_gettext(x->f_subtitle_size, x->f_subtitle, &subtitle_size, &subtitle, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
	atom_gettext(x->f_description_size, x->f_description, &description_size, &description, OBEX_UTIL_ATOM_GETTEXT_SYM_NO_QUOTE);
	
	doNotification(title, subtitle, description, x->f_image_path->s_name, x->f_playsound);
}


void notif_int(t_notif *x, long n)
{
	;
}

short doNotification(const string& title, const string& subtitle, const string& description, const string& image, bool playSound)
{
	
#if (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_8)
	
    NSUserNotification *notification = [[NSUserNotification alloc] init];
    
    if(!title.empty())
	{
        [notification setTitle:[NSString stringWithCString:title.c_str()
                                                  encoding:[NSString defaultCStringEncoding]]];
    }
    
    if(!subtitle.empty())
	{
        [notification setSubtitle:[NSString stringWithCString:subtitle.c_str()
                                                     encoding:[NSString defaultCStringEncoding]]];
    }
    
    if(!description.empty())
	{
        [notification setInformativeText:[NSString stringWithCString:description.c_str()
                                                            encoding:[NSString defaultCStringEncoding]]];
    }
	
	if(!image.empty())
	{
		/*
		NSImage* img = [[NSImage alloc] URLForImageResource:[NSString stringWithCString:image.c_str()
																				  encoding:[NSString defaultCStringEncoding]]];
		*/
		
		NSImage* img = [[NSImage alloc] initWithContentsOfFile:[NSString stringWithCString:image.c_str()
																				  encoding:[NSString defaultCStringEncoding]]];
		
		[notification setContentImage:img];
    }
    
    [notification setSoundName:playSound ? (NSUserNotificationDefaultSoundName) : nil];
	
    //[notification setDeliveryDate:[NSDate date]]; // now
	
	[notification setDeliveryDate:[NSDate dateWithTimeInterval:0 sinceDate:[NSDate date]]];
    
    NSUserNotificationCenter *center = [NSUserNotificationCenter defaultUserNotificationCenter];
    
    [center scheduleNotification:notification];
	   
    [notification release];
	
	return 1;
#else
	return 0;
#endif
};
