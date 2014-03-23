/**
 * Copyright (c) 2011-2014 Eliott Paris.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE" in this distribution.
 *
 * timeStretcher/pitchShifter Object adapted from "sugarSynth" MaxMSP Patch.
 */

#include "../lib/ep.max.h"

#define COS_TABLE_LEN 512
#define MIN_VOICES 1
#define DEFAULT_VOICES 12
#define MAX_VOICES 64
#define TRIG .001
#define MAX_LIST_SCALE 20

#define COMPTEUR register unsigned int
#define INTERP_QUAD (be + frac * (cminusb - 0.1666667f * (1.-frac) * ((d - a - 3.0f * cminusb) * frac + (d + 2.0f*a - 3.0f*be))))
#define INTERP_LINE (yDeb+((phase-debut)*((fazCosWT[debut+1])-yDeb)))

typedef struct _timestretch
{
    t_pxobject l_obj;
	
	// ------ Obj Var ------ //
	t_double x_speed; // -> vitesse de lecture (1 = normal | 2 = 2x + vite | 0.5 = 2x - vite).
	t_double x_pitch; // -> hauteur independante de la vitesse (1 = normal | 2 = 2x + haut | 0.5 = 2x + bas)
	t_double x_randPitchRate;
	t_double x_grainSize;
	t_double x_stereoSpread; // entre 0. et 1.
	t_double x_gain;
	long    x_nbVoice; // nombre de d'instance du poly~
	long    x_freeze; // 1 = lecture à 0 / 0 reprend la vitesse d'avant le freeze.
	long    x_stretch;
	long    x_stretchInterp;
	t_float x_beforeFreeze; // stocke la vitesse courante quand freeze = 1 pour la reprendre si besoin.
	t_float x_randPosVal; // quantité de décalage en ms par rapport à la position actuelle.
	t_float x_currentPos; // position actuelle de la tête de lecture sur le sample.
	t_float x_start; // debut de la lecture (ms).
	t_float x_start_phase;
	t_float x_end; // fin de la lecture (ms).
	t_float x_end_phase;
	t_float x_bufLen2Read; // partie à lire du buffer (end-start)
	t_float x_powGain;
	t_float x_activStretchInterpolTime;
	t_float *x_pitchScale;
	long    x_pitchScaleSize;
	//t_float x_nbOut; -> pour la spatialisation ambisonique.
	
	// ------ Dsp Var ------ //
	t_float x_sr;
	t_float x_msr;
	short *x_connected; // signal branché ou non à chaque inlet (0/1).
	
	// ------ buffer ------ //
    t_symbol *l_sym;
    t_buffer *l_buf;
    long l_chan;
	t_float x_buf_sr;
	t_float x_buf_msr;
	t_float x_buf_sizeInMs;
	
	// ------ Cos WaveTable ------ //
	t_float *x_cosWaveTable;
	t_float *x_fazCosWT;
	
	// ------ Phasors ------ //
	t_float x_grainPhasor_increment;
	//t_float x_grainPhasor_freq;
	//t_float x_grainPhasor_phase;
	t_float x_grainPhasor_glob_tempFreq;
	t_float x_grainPhasor_glob_freq;
	t_float x_grainPhasor_glob_phase;
	t_int   x_grainPhasor_sens;
	//--
	t_float x_playerPhasor_increment;
	t_float x_playerPhasor_freq;
	t_float x_playerPhasor_phase;
	t_int   x_playerPhasor_sens;
	
	// ------ Sah~ ------ //
	t_float x_trigger_threshold;
	
	// ------ instance Specific variables ------ //
	// ------ Sah~
	t_float *x_hold_grPlusRdPitch;
	t_float *x_hold_currentPos;
	t_float *x_hold_stereoSpread;
	t_float *x_hold_grainSize; // à implémenter => ne marche pas sur le patch !
	t_float *x_hold_gain;
	t_float *x_hold_gainL;
	t_float *x_hold_gainR;
	t_float *x_previous;
	// ------ grainPhasor
	t_float *x_hold_grainPhasor_freq;
	t_float *x_grainPhasor_initPhase;
	t_float *x_grainPhasor_freq;
	t_float *x_grainPhasor_phase;
	t_float x_grainPhasor_dephaseVal;
	
	
} t_timestretch;

t_class *timestretch_class;

//---------------------------------------------------------------------------------------------------------

#pragma mark -
#pragma mark Prototypes :

void timestretch_perform64(t_timestretch *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam);
void timestretch_dsp(t_timestretch *x, t_signal **sp, short *count);
void timestretch_dsp64(t_timestretch *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags);
void timestretch_free(t_timestretch *x);
void *timestretch_new(t_symbol *s, long chan);
void timestretch_assist(t_timestretch *x, void *b, long m, long a, char *s);
void timestretch_dblclick(t_timestretch *x);

void timestretch_set(t_timestretch *x, t_symbol *s);
void timestretch_int(t_timestretch *x, long n);
void timestretch_float(t_timestretch *x, double f);
void timestretch_info_msg(t_timestretch *x);
void timestretch_setChan(t_timestretch *x, long n);
void timestretch_setVoices(t_timestretch *x, long n);
void timestretch_jumpto(t_timestretch *x, double f);
void timestretch_freeze(t_timestretch *x, long n);
void timestretch_setStretch(t_timestretch *x, long n);
void timestretch_setRandPos(t_timestretch *x, double n);
void timestretch_setMin(t_timestretch *x, double d);
void timestretch_setMax(t_timestretch *x, double d);
void timestretch_setMinMax(t_timestretch *x, double min, double max);
void timestretch_setScale(t_timestretch *x, t_symbol *s, short ac, t_atom *av);

double timestretch_interp_quad(double val, t_float *tab, long nc, long chan);
void fill_wavetable(float wavetable[], int tableLength);
void fill_fazCosWT(float wavetable[], int tableLength);
double read_table(double phase, float wavetable[], int tableLength);
double noise(double min, double max);
double ep_dscale(double value, double inlow, double inhigh, double outlow, double outhigh);

t_symbol *ps_nothing, *ps_buffer;

//---------------------------------------------------------------------------------------------------------

#pragma mark -
#pragma mark Main :

int C74_EXPORT main(void)
{
	t_class *c;

	c = class_new("ep.timestretch~", (method)timestretch_new, (method)timestretch_free,
				  (short)sizeof(t_timestretch), 0L, A_SYM, A_DEFLONG, 0);
	
	class_addmethod(c, (method)timestretch_dsp64,		"dsp64",	A_CANT, 0);
	class_addmethod(c, (method)timestretch_set,			"set",		A_SYM, 0);
	class_addmethod(c, (method)timestretch_int,			"int",		A_LONG, 0);
	class_addmethod(c, (method)timestretch_float,		"float",	A_FLOAT, 0);
	class_addmethod(c, (method)timestretch_setChan,		"channel",	A_LONG, 0);
	class_addmethod(c, (method)timestretch_setVoices,	"voices",	A_LONG, 0);
	class_addmethod(c, (method)timestretch_setStretch,	"stretch",	A_LONG, 0);
	class_addmethod(c, (method)timestretch_setRandPos,	"randpos",	A_FLOAT, 0);
	class_addmethod(c, (method)timestretch_setMin,		"min",		A_FLOAT, 0);
	class_addmethod(c, (method)timestretch_setMax,		"max",		A_FLOAT, 0);
	class_addmethod(c, (method)timestretch_setMinMax,	"minmax",	A_FLOAT, A_FLOAT, 0);
	class_addmethod(c, (method)timestretch_jumpto,		"jumpto",	A_FLOAT, 0);
	class_addmethod(c, (method)timestretch_float,		"freeze",	A_LONG, 0);
	class_addmethod(c, (method)timestretch_setScale,	"scale",	A_GIMME, 0);
	class_addmethod(c, (method)timestretch_info_msg,	"info",		0, 0);
	class_addmethod(c, (method)timestretch_assist,		"assist",	A_CANT, 0);
	class_addmethod(c, (method)timestretch_dblclick,	"dblclick", A_CANT, 0);
	
	class_dspinit(c);
	class_register(CLASS_BOX, c);
	timestretch_class = c;
	
	ps_buffer = gensym("buffer~");
	ps_nothing = gensym("");
	return 0;
}

#pragma mark -
#pragma mark Dsp :

void timestretch_dsp64(t_timestretch *x, t_object *dsp64, short *count, double samplerate, long maxvectorsize, long flags)
{
	timestretch_set(x,x->l_sym);
	x->x_sr = samplerate;
	int i;
	for (i=0; i<6; i++) x->x_connected[i] = count[i];
	object_method(dsp64, gensym("dsp_add64"), x, timestretch_perform64, 0, NULL);
}


//---------------------------------------------------------------------------------------------------------

#pragma mark -
#pragma mark Perform Routine :

void timestretch_perform64(t_timestretch *x, t_object *dsp64, double **ins, long numins, double **outs, long numouts, long sampleframes, long flags, void *userparam)
{	
	// ------- inlets ------- //
	t_double *speed = x->x_connected[0]? ins[0] : &x->x_speed;
	t_double *pitch = x->x_connected[1]? ins[1] : &x->x_pitch;
	t_double *randPitchRate = x->x_connected[2]? ins[2] : &x->x_randPitchRate;
	t_double *grainSize = x->x_connected[3]? ins[3] : &x->x_grainSize;
	t_double *gain = x->x_connected[4]? ins[4] : &x->x_gain;
	t_double *stereoSpread = x->x_connected[5]? ins[5] : &x->x_stereoSpread;
	
	// ------- outlets ------- //
    t_double *outL = outs[0];
	t_double *outR = outs[1];
	t_double *ramp = outs[2];
	
	// ------- vecSize ------- //
    //int n = sampleframes;

	
	t_buffer *b = x->l_buf;
	t_float *tab;
	
	t_double monoOut, temp, polyL, polyR, tempWindowPhasorOut, windowCosOut;
	t_float sr = x->x_sr; // augmente cpu
	t_float *cosWaveTable = x->x_cosWaveTable;
	t_float *fazCosWT = x->x_fazCosWT;
	long chan,frames,nc;
	long grainActif;
	COMPTEUR i;
	
	long nbVoice = x->x_nbVoice;
	t_float buf_msr = x->x_buf_msr;
	t_double bufSize = x->x_buf_sizeInMs;
	t_float rdPosVal = x->x_randPosVal;
	t_double playerPhasor_increment = ((1000. / bufSize) * *speed / x->x_sr);
	t_double tempRamp = x->x_playerPhasor_phase;
	t_double start_phase = x->x_start_phase;
	t_double end_phase = x->x_end_phase;
	t_float  powGain = x->x_powGain;
	t_double grainPhasor_glob_tempFreq;
	t_double grainPhasor_glob_inc;
	t_double curPos = x->x_currentPos;
	t_double grainPhasor_glob_phase = x->x_grainPhasor_glob_phase;
	t_double glob_stereoSpread;
	
	
	t_double ptch, rdPtchRte, grSize, gn, stSprd;
	
	long debut;
	t_double yDeb, phase;
	int tableLen = COS_TABLE_LEN;
	
	long index;
	t_double frac, a, be, c, d, cminusb;
	
	//////////////
	
	ptch = *pitch;
	if (ptch < 0.01) ptch = 0.01;
	else if (ptch > 100) ptch = 100;
	
	rdPtchRte = *randPitchRate;
	if (rdPtchRte < 0.) rdPtchRte = 0.;
	else if (rdPtchRte > 10) rdPtchRte = 10;
	
	grSize = *grainSize;
	if (grSize < 15.) grSize = 15;
	else if (grSize > x->x_bufLen2Read) grSize = x->x_bufLen2Read;
	
	gn = *gain * powGain;
	if (gn < 0.) gn = 0.;
	else if (gn > 10) gn = 10;
	
	stSprd = *stereoSpread;
	if (stSprd < 0.) stSprd = 0.;
	else if (stSprd > 0.9999) stSprd = 0.9999;
	
	grainPhasor_glob_tempFreq = 1000. / grSize;
	grainPhasor_glob_inc = (grainPhasor_glob_tempFreq * ptch) / sr;
	
	
	// ------- si l'objet est mute ou si pas de buffer : nothing ------- //
	if (x->l_obj.z_disabled) goto out;	
	if (!b) goto zero;
	
	ATOMIC_INCREMENT(&b->b_inuse);
	
	// ------- si buffer indispo : nothing ------- //
	if (!b->b_valid) {
		ATOMIC_DECREMENT(&b->b_inuse);
		goto zero;
	}
	
	//if (buffer_perform_begin(x->l_buf) != MAX_ERR_NONE)
	//	goto zero;
	
	// ------- tout est bon : on calcule ------- //
	timestretch_set(x,x->l_sym); // on verifie que le contenu du buffer n'a pas changé (actualisation des valeurs).	
	
	tab = b->b_samples;
	chan = x->l_chan;
	frames = b->b_frames;
	nc = b->b_nchans;
	
	while (sampleframes--) {
		
		polyL = polyR = 0.; // on remet les valeurs de sortie à 0;
		
		// ------- Phasor Player :
		
		tempRamp += playerPhasor_increment; // increment
		if (tempRamp > end_phase){ // si depasse max
			tempRamp = start_phase; // reviens à min
		}
		else if (tempRamp < start_phase){ // si < min
			tempRamp = end_phase; // va à max
		}		
		
		// ------------------- -------------------------- ------------------//
		// ------------------- si stretch = 1, on calcule ------------------//
		// ------------------- -------------------------- ------------------//
		
		if (x->x_stretch) {
			
			// ------- Position courante d'où debute la lecture des grains.
			//curPos = ((tempRamp * bufSize) + (noise(-1., 1.) * rdPosVal));
			curPos = ((tempRamp * bufSize) + ((double)(rand()/(float)RAND_MAX * 2 - 1)) * rdPosVal);
			
			
			
			// ------- Phasor Grain :	
			grainPhasor_glob_phase += grainPhasor_glob_inc;
			if (grainPhasor_glob_phase > 1.) grainPhasor_glob_phase -= 1.0f; // reviens à min
			
			// ------- Boucle Poly~ ------- //
			for (i = nbVoice; i--;) {				
				// ------- Phasor Grain :
				if (x->x_hold_grainPhasor_freq[i] != grainPhasor_glob_tempFreq) {
					x->x_grainPhasor_phase[i] += (x->x_hold_grainPhasor_freq[i] * ptch / sr);
					if (x->x_grainPhasor_phase[i] > 1.) x->x_grainPhasor_phase[i] -= 1.0f;
				} else x->x_grainPhasor_phase[i] = grainPhasor_glob_phase;
				
				tempWindowPhasorOut = x->x_grainPhasor_phase[i] + x->x_grainPhasor_initPhase[i];
				if (tempWindowPhasorOut >= 1) tempWindowPhasorOut -= 1.0f;
				
				phase = tempWindowPhasorOut * tableLen;
				//if (phase >= tableLen) phase -= tableLen;
				debut = phase;
				yDeb = fazCosWT[debut];
				windowCosOut = INTERP_LINE;
				
				//windowCosOut = read_table((tempWindowPhasorOut * 0.5 + 0.75), x->x_cosWaveTable, COS_TABLE_LEN);
				
				// conditions pour les sah~:
				if ((x->x_previous[i] <= TRIG) && (windowCosOut >= TRIG)) {
					
					x->x_hold_grainPhasor_freq[i] = grainPhasor_glob_tempFreq;
					x->x_grainPhasor_phase[i] = grainPhasor_glob_phase;
					
					x->x_hold_currentPos[i] = curPos;
					
					if (rdPtchRte > 0.0f) {
						//x->x_hold_grPlusRdPitch[i] = grSize * (rdPtchRte * noise(-1., 1.) + 1.);
						// choisi pitch en fonction des valeurs de x->x_pitchScale[?];
						x->x_hold_grPlusRdPitch[i] = grSize * x->x_pitchScale[(long)(rand()/(float)RAND_MAX * (long)(rdPtchRte * x->x_pitchScaleSize))];
						//x->x_hold_grPlusRdPitch[i] = grSize * (rdPtchRte * ((double)(rand()/(float)RAND_MAX * 2 - 1)) + 1.);
					} else x->x_hold_grPlusRdPitch[i] = grSize;
					
					if (stSprd > 0.0f) {
						
						//glob_stereoSpread = (stSprd * 0.5 * noise(-1., 1.) + 0.5) * 0.25;
						//glob_stereoSpread = (stSprd * noise(-0.5, 0.5) + 0.5) * 0.25;
						glob_stereoSpread = (stSprd * ((double)(rand()/(float)RAND_MAX * 1.5 - 0.5)) + 0.5) * 0.25;
						
						phase = glob_stereoSpread * tableLen;
						if (phase >= tableLen) phase -= tableLen;
						debut = (long)phase; // arrondi à l'entier inferieur
						yDeb = cosWaveTable[debut];
						x->x_hold_gainL[i] = (yDeb+((phase-debut)*((cosWaveTable[debut+1])-yDeb))) * gn;
						
						phase = (glob_stereoSpread + 0.75) * tableLen;
						if (phase >= tableLen) phase -= tableLen;
						debut = (long)phase;
						yDeb = cosWaveTable[debut];
						x->x_hold_gainR[i] = (yDeb+((phase-debut)*((cosWaveTable[debut+1])-yDeb))) * gn;
						
					} else x->x_hold_gainL[i] = x->x_hold_gainR[i] = gn;
					
					grainActif = 0;
					
				} else grainActif = 1;
				
				x->x_previous[i] = windowCosOut;
				
				//if (grainActif) {
				
				temp = (x->x_hold_currentPos[i] + (x->x_hold_grPlusRdPitch[i] * tempWindowPhasorOut)) * buf_msr; // conversion msToSample
				if (temp <= 1) temp = 1;
				else if (temp > frames-1) temp = frames-1;
				
				index = temp;
				frac = temp - index;
				
				if (nc > 1) {
					a = tab[((index-1) * nc + chan)];
					be = tab[(index * nc + chan)];
					c = tab[((index+1) * nc + chan)];
					d = tab[((index+2) * nc + chan)];
				} else {
					a = tab[index-1];
					be = tab[index];
					c = tab[index+1];
					d = tab[index+2];
				}
				cminusb = c-be;
				monoOut = INTERP_QUAD * windowCosOut;
				
				polyL += monoOut * x->x_hold_gainL[i];
				polyR += monoOut * x->x_hold_gainR[i];
			}
		}
		
		else
		{
			// ------- si stretch = 0 on lit le buffer
			curPos = tempRamp * bufSize;
			if (curPos <= 1) curPos = 1;
			else if (curPos > frames-1) curPos = frames-1;
			polyR = polyL = timestretch_interp_quad(curPos * buf_msr, tab, nc, chan);
		}
		
		if (x->x_connected[0]) x->x_speed = *speed++;
		if (x->x_connected[1]) x->x_pitch = *pitch++; 
		if (x->x_connected[2]) x->x_randPitchRate = *randPitchRate++;
		if (x->x_connected[3]) x->x_grainSize = *grainSize++;
		if (x->x_connected[4]) x->x_gain = *gain++;
		if (x->x_connected[5]) x->x_stereoSpread = *stereoSpread++;
		
		// ------- Outputs
		*ramp++ = curPos/bufSize;
		*outR++ = polyR;
		*outL++ = polyL;
		
		x->x_currentPos = curPos;
	}
	
	x->x_playerPhasor_phase = tempRamp;
	x->x_grainPhasor_glob_phase = grainPhasor_glob_phase;
	
	//buffer_perform_end(x->l_buf);
	ATOMIC_DECREMENT(&b->b_inuse);
	return;
zero:
	while (sampleframes--){
		*ramp++ = 0.;
		*outR++ = 0.;
		*outL++ = 0.;
	}
out:
	return;
}

//---------------------------------------------------------------------------------------------------------

double timestretch_interp_quad(double val, t_float *tab, long nc, long chan)
{
	// fonction d'interpolation prise dans tabread4 => PD array.h
	
	long index;
	double frac;
	double a,  b,  c,  d, cminusb;
	index = val;
	frac = val - index;
	
	//double a0,a1,a2,mu2;
	
	if (nc > 1) {
		a = tab[((index-1) * nc + chan)];
		b = tab[(index * nc + chan)];
		c = tab[((index+1) * nc + chan)];
		d = tab[((index+2) * nc + chan)];
	} else {
		a = tab[index-1];
		b = tab[index];
		c = tab[index+1];
		d = tab[index+2];
	}
	cminusb = c-b;
	return (b + frac * (cminusb - 0.1666667f * (1.-frac) * ((d - a - 3.0f * cminusb) * frac + (d + 2.0f*a - 3.0f*b))));
}

//---------------------------------

double read_table(double phase, float wavetable[], int tableLength)
{
	long debut;
	double yDeb;
	phase *= tableLength;
	if (phase >= tableLength) phase -= tableLength;
	debut = phase;
	yDeb = wavetable[debut];
	return (yDeb + ( (phase-debut) * ( (wavetable[debut+1])-yDeb ) ) );
}

//---------------------------------

void fill_wavetable(float wavetable[], int tableLength)
{
	int i;
	for (i = 0; i < tableLength; i++) {
		wavetable[i] = cos(TWOPI*i/tableLength);
	}
	wavetable[tableLength] = wavetable[0];	
}

void fill_fazCosWT(float wavetable[], int tableLength)
{
	for(int i = 0; i < tableLength; i++)
		wavetable[i] = (t_float)(((sin(((double)i/(double)tableLength*TWOPI)+3.f * PIOVERTWO))/2.f)+0.5f);
	
	wavetable[tableLength] = wavetable[0];
}

//---------------------------------

double noise(double min, double max)
{
	return (double)(rand()/(double)RAND_MAX * (max - min) + min);
}

//---------------------------------------------------------------------------------------------------------

#pragma mark -
#pragma mark Msg In Methods :

//---------------------------------------------------------------------------------------------------------

// here's where we set the buffer~ we're going to access
void timestretch_set(t_timestretch *x, t_symbol *s)
{
	t_buffer *b = NULL;
	
	if (s)
	{
		x->l_sym = s;
		if ((b = (t_buffer *)(s->s_thing)) && ob_sym(b) == ps_buffer)
		{
			x->l_buf = b;
		}
		else
		{
			object_error((t_object *)x, "no buffer~ %s", s->s_name);
			x->l_buf = 0;
		}
	}
	x->x_buf_sr = x->l_buf->b_sr;
	x->x_buf_msr = x->x_buf_sr * 0.001;
	
	// si la taille du buffer change on li le buffer en entier (edit : pourquoi ?)
	if (x->x_buf_sizeInMs != (b->b_frames / b->b_msr))
	{
		x->x_buf_sizeInMs = (b->b_frames / b->b_msr);
		timestretch_setMinMax(x, 0, x->x_buf_sizeInMs);
	}
}

//------------------------------------------------------------------

void timestretch_setMin(t_timestretch *x, double d)
{
	timestretch_setMinMax(x, d, x->x_end);
}

//---------------------------------

void timestretch_setMax(t_timestretch *x, double d)
{
	timestretch_setMinMax(x, x->x_start, d);
}

//---------------------------------

void timestretch_setMinMax(t_timestretch *x, double min, double max)
{
	t_float bufSize = x->x_buf_sizeInMs;
	if (min < 0.) min = 0.;
	else if (min > bufSize) min = bufSize;
	
	if (max < 0.) max = 0.;
	else if (max > x->x_buf_sizeInMs) max = bufSize;
	
	if (min < max) {
		x->x_start = min;
		x->x_end = max;
	} else if (min > max){
		x->x_start = max;
		x->x_end = min;
	} else if ((max - min) < 100.){
		x->x_start = min;
		x->x_end = min+100;
	}
	x->x_bufLen2Read = x->x_end - x->x_start;
	x->x_start_phase = x->x_start / bufSize;
	x->x_end_phase = x->x_end / bufSize;
}

//---------------------------------------------------------------------------------------------------

void timestretch_setChan(t_timestretch *x, long n)
{
	if (n) x->l_chan = CLAMP(n,1,4) - 1;
	else x->l_chan = 0;
}

//---------------------------------

void timestretch_setRandPos(t_timestretch *x, double f)
{
	if (f < 0.0f) x->x_randPosVal = 0.0f;
	else if (f >= x->x_buf_sizeInMs) x->x_randPosVal = x->x_buf_sizeInMs;
	else x->x_randPosVal = f;
}

//---------------------------------

void timestretch_setVoices(t_timestretch *x, long n)
{
	long voices = CLAMP(n, MIN_VOICES, MAX_VOICES);
	x->x_grainPhasor_dephaseVal = 1./(float)voices;
	int i;
	for (i = 1; i <= voices; i++) {
		x->x_grainPhasor_initPhase[i-1] = i * (1./(float)voices);
		x->x_previous[i-1] = 0.;
		//post("x->x_grainPhasor_initPhase[%ld] = %f", i-1, i * (1./voices));
	}
	x->x_nbVoice = voices;
	x->x_powGain = pow((double)voices, -0.5) * 1.414;
	
	for (i = 0; i < voices; i++) {
		x->x_grainPhasor_freq[i] = x->x_hold_grainPhasor_freq[i] = 10;
	}
}

// Scale function for doubles
double ep_dscale(double value, double inlow, double inhigh, double outlow, double outhigh)
{
	double inscale, outdiff;
	
 	inscale = 1 / (inhigh - inlow);
 	outdiff = outhigh - outlow;
 	
	value = (value - inlow) * inscale;
	value = (value * outdiff) + outlow;
	return(value);
}

void timestretch_jumpto(t_timestretch *x, double f)
{
	x->x_playerPhasor_phase = CLAMP(ep_dscale(f, 0, x->x_buf_sizeInMs, 0., 1.), 0., 1.);
}

void timestretch_freeze(t_timestretch *x, long n)
{
	if (n) {
		x->x_freeze = 1;
	} else x->x_freeze = 0;
}

void timestretch_setStretch(t_timestretch *x, long n)
{
	if (n) x->x_stretch = 1;
	else x->x_stretch = 0;
}

void timestretch_setScale(t_timestretch *x, t_symbol *s, short ac, t_atom *av)
{
	int i, nbGoodArg = 0;
	for (i = 0; i < ac; i++, *av++) {
		if (nbGoodArg >= MAX_LIST_SCALE) break; // on ignore le reste de la liste;
		switch (av->a_type) {
			case A_LONG:
				x->x_pitchScale[nbGoodArg] = (float)pow(2., (float)atom_getlong(av)/12.0f);
				nbGoodArg++;
				break;
			case A_FLOAT:
				x->x_pitchScale[nbGoodArg] = (float)pow(2., (float)atom_getfloat(av)/12.0f);
				nbGoodArg++;
				break;
			default:
				break;
		}
	}
	x->x_pitchScaleSize = nbGoodArg;	
}

//---------------------------------------------------------------------------------------------------------

void timestretch_int(t_timestretch *x, long n)
{
	timestretch_float(x, (double)n);
}

/////////////////////////////////

void timestretch_float(t_timestretch *x, double f)
{
	long inlet = proxy_getinlet((t_object *)x);
	switch (inlet) {
		case 0: // speed
			if (!x->x_connected[0]) x->x_speed = f; break;
		case 1: // pitch
			if (!x->x_connected[1]) x->x_pitch = f; break;
		case 2: // pitchRateVar
			if (!x->x_connected[2]) x->x_randPosVal = f; break;
		case 3: // grainSize
			if (!x->x_connected[3]) x->x_grainSize = f; break;
		case 4: // gain
			if (!x->x_connected[4]) x->x_gain = f; break;
		case 5: // stereoSpread
			if (!x->x_connected[5]) x->x_stereoSpread = f; break;
		default:
			break;
	}
}

//---------------------------------------------------------------------------------------------------------

#pragma mark Info :

void timestretch_info_msg(t_timestretch *x)
{
	t_buffer *b;
	post("start = %f, end = %f", x->x_start, x->x_end);
	
	object_post((t_object *)x, "********** infos : **********");
	
	if ((b = (t_buffer *)(x->l_sym->s_thing)) && ob_sym(b) == ps_buffer)
	{
		post("nom du buffer : %s", (t_buffer *)(x->l_sym->s_name));
		//post("frames = %f", b->b_frames); // probleme !!!
		post("sampling-rate du buffer : %ld Hz.", (long)b->b_sr);
		//post("sampling-rate du buffer (b->msr) : %f Hz.", b->b_msr);
		post("taille du buffer = %f ms", x->x_buf_sizeInMs);
		post("nombre de canaux du buffer : %ld", b->b_nchans);
	}
	else
	{
		post("no valid buffer assignement");
	}
	
	post(" *------------*");
	post("buffer's channel used : %ld", x->l_chan);
	post("current speed = %f", x->x_speed);
	post("current pitch = %f", x->x_pitch);
	post("current grainSize = %f", x->x_grainSize);
	post("current randPitchRate = %f", x->x_randPitchRate);
	post("current stereoSpread = %f", x->x_stereoSpread);
	post("current gain = %f", x->x_gain);
	post("current Nb Voices = %ld", x->x_nbVoice);
	post("current Position in ms = %f", x->x_currentPos);
	post("current Random Position Deviation = %f", x->x_randPosVal);
	post("x_playerPhasor_freq = %f", x->x_playerPhasor_freq);
	post("********************");
}

//---------------------------------------------------------------------------------------------------------

// this lets us double-click on timestretch~ to open up the buffer~ it references
void timestretch_dblclick(t_timestretch *x)
{
	t_buffer *b;
	
	if ((b = (t_buffer *)(x->l_sym->s_thing)) && ob_sym(b) == ps_buffer)
		mess0((t_object *)b,gensym("dblclick"));
}

//---------------------------------------------------------------------------------------------------------

void timestretch_assist(t_timestretch *x, void *b, long m, long a, char *s)
{
	if (m == ASSIST_OUTLET)
		switch (a) {
			case 0:	sprintf(s,"(signal) Out Left");	break;
			case 1:	sprintf(s,"(signal) Out Right");	break;
			case 2:	sprintf(s,"(signal) Ramp Outlet (0 to 1)");	break;
		}
	else {
		switch (a) {
			case 0:	sprintf(s,"(signal/float) Speed");	break;
			case 1:	sprintf(s,"(signal/float) Pitch");	break;
			case 2:	sprintf(s,"(signal/float) Random Pitch Rate");	break;
			case 3:	sprintf(s,"(signal/float) Grain Size");	break;
			case 4:	sprintf(s,"(signal/float) Gain");	break;
			case 5:	sprintf(s,"(signal/float) Stereo Spread");	break;
		}
	}
}

//---------------------------------------------------------------------------------------------------------

#pragma mark -
#pragma mark New Method :

void *timestretch_new(t_symbol *s, long chan)
{
	t_timestretch *x = (t_timestretch *)object_alloc(timestretch_class);
	
	dsp_setup((t_pxobject *)x, 6); // => speed, pitch, randPitchRate, grainSize, gain, stereoSpread.
	outlet_new((t_object *)x, "signal"); // => Canal gauche traité.
	outlet_new((t_object *)x, "signal"); // => Canal droit traité.
	outlet_new((t_object *)x, "signal"); // => Ramp Outlet.
	
	srand(time(NULL)); // initialise fonction aleatoire
	
	x->l_sym = s;

	x->x_speed = 1.;
	x->x_pitch = 1.;
	x->x_grainSize = 100.;
	x->x_randPitchRate = .0;
	x->x_stereoSpread = .0;
	x->x_gain = 1.;
	x->x_nbVoice = DEFAULT_VOICES;
	x->x_randPosVal = 5.;
	x->x_currentPos = .0;
	x->x_powGain = pow(x->x_nbVoice, -0.5) * 1.414;
	x->x_stretch = 1;
	x->x_stretchInterp = 1;
	x->x_activStretchInterpolTime = 50;
	
	x->x_trigger_threshold = .001;
	
	x->x_connected = (short *)getbytes(6 * sizeof(short));
	
	//allocation dynamique de memoire pour la table cosinus
	x->x_cosWaveTable = (float *)getbytes((COS_TABLE_LEN+1)*sizeof(float));
	x->x_fazCosWT = (float *)getbytes((COS_TABLE_LEN+1)*sizeof(float));
	fill_wavetable(x->x_cosWaveTable, COS_TABLE_LEN);
	fill_fazCosWT(x->x_fazCosWT, COS_TABLE_LEN);
	
	//allocation dynamique de memoire pour les besoins de chaque instances :
	x->x_hold_grPlusRdPitch = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_hold_currentPos = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_hold_stereoSpread = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_hold_grainSize = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_previous = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_grainPhasor_initPhase = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_grainPhasor_phase = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_grainPhasor_freq = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_hold_grainPhasor_freq = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_hold_gain = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_hold_gainL = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_hold_gainR = (float *)getbytes((MAX_VOICES)*sizeof(float));
	x->x_pitchScale = (float *)getbytes((MAX_LIST_SCALE)*sizeof(float));
	
	timestretch_setChan(x,chan); // l'argument chan determine le canal utilisé
	timestretch_setVoices(x, x->x_nbVoice);
	defer_low(x, (method)timestretch_set, s, 0, NULL);
	
	return (x);
}

//-------------------------------------------------------------------------------------------------------------------

void timestretch_free(t_timestretch *x)
{
	freebytes( (void *)(x->x_connected), (6 * sizeof(short)));
	
	freebytes( (void *)(x->x_cosWaveTable), ((COS_TABLE_LEN+1) * sizeof(float)));
	freebytes( (void *)(x->x_fazCosWT), ((COS_TABLE_LEN+1) * sizeof(float)));
	
	freebytes( (void *)(x->x_hold_grPlusRdPitch), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_hold_currentPos), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_hold_stereoSpread), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_hold_grainSize), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_previous), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_grainPhasor_initPhase), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_grainPhasor_phase), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_grainPhasor_freq), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_hold_grainPhasor_freq), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_hold_gain), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_hold_gainL), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_hold_gainR), (MAX_VOICES * sizeof(float)));
	freebytes( (void *)(x->x_pitchScale), (MAX_LIST_SCALE * sizeof(float)));
	
	dsp_free((t_pxobject *)x);
}

//---------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------
