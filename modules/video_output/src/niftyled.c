/* vlc-niftylight - vlc LED output using libniftyled
 *
 * Copyright (C) 2012-2013  Daniel Hiepler <daniel@niftylight.de>
 * 
 * Authors: Daniel Hiepler
 *
 * Based on the vout plugins, examples and documentation from the VideoLAN team
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 */

/*****************************************************************************
 * Preamble
 *****************************************************************************/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_vout_display.h>
#include <vlc_picture_pool.h>

#include <niftyled.h>



/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
static int              Open( vlc_object_t *obj );
static void             Close( vlc_object_t *obj );
static picture_pool_t * Pool(vout_display_t *vd, unsigned count);
static int              Control(vout_display_t *, int, va_list);
static void             PreparePic(vout_display_t *vd, picture_t *picture, subpicture_t *s);
static void             DisplayPic(vout_display_t *vd, picture_t *picture, subpicture_t *s);


/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define PROP_CFGFILE    "configfile"
#define T_CFGFILE       "niftyled XML setup description"
#define TL_CFGFILE      "XML File containing information about the complete LED-Setup. s. http://wiki.niftylight.de/LED-setup_XML"  

#define PROP_WIDTH      "width"
#define T_WIDTH         "fixed width"
#define TL_WIDTH        "use fixed width of LED setup when > 0"

#define PROP_HEIGHT     "height"
#define T_HEIGHT        "fixed height"
#define TL_HEIGHT       "use fixed height of LED setup when > 0"

#define PROP_VERBOSITY  "verbosity"
#define T_VERBOSITY     "verbosity level"
#define TL_VERBOSITY    "amount of logging output"


vlc_module_begin();
    set_shortname( "niftyled LED" );
    set_category( CAT_VIDEO );
    set_subcategory( SUBCAT_VIDEO_VOUT );
    set_description( "niftyled LED output plugin" );
    set_capability( "vout display", 0 );
    add_string(PROP_CFGFILE, "~/.niftyvlc.xml", T_CFGFILE, TL_CFGFILE, false);
    add_integer(PROP_WIDTH, 20, T_WIDTH, TL_WIDTH, false);
    add_integer(PROP_HEIGHT, 20, T_HEIGHT, TL_HEIGHT, false);
    set_callbacks( Open, Close );
vlc_module_end();


               
/*****************************************************************************
 * vout_sys_t: video output descriptor
 *****************************************************************************/
struct vout_display_sys_t
{
    /*** VLC stuff */
    picture_pool_t *    pool;

        
    /*** niftyled stuff */

    /** current settings */
    LedPrefs *prefs;
	/** current setup */
	LedSetup *setup;
    /** first hardware */
    LedHardware *hw;
    /** temporary frame */
    LedFrame *frame;
    /** dimensions of frame */
    LedFrameCord width, height;
};


/** logging function to pipe niftyled log-output through VLC */
static void _log(void *o, NftLoglevel level, const char *file, const char *func, int line, const char *msg)
{
    vlc_object_t *obj = o;
        
    if(!file)
        file = "(extern)";

    switch(level)
    {

        case L_INFO:
        {
            msg_Info(obj, "%s:%d %s(): %s", file, line, func, msg);
            break;
        }

        case L_ERROR:
        {
            msg_Err(obj, "%s:%d %s(): %s", file, line, func, msg);
            break;
        }

        case L_WARNING:
        {
            msg_Warn(obj, "%s:%d %s(): %s", file, line, func, msg);
        }

        case L_VERBOSE:
        case L_DEBUG:
        case L_NOISY:
        case L_VERY_NOISY:
        {
            msg_Dbg(obj, "%s:%d %s(): %s", file, line, func, msg);
            break;
        }
                    
        default:
            break;
    }
}


/**
 * initialize plugin resources
 */
static int Open(vlc_object_t *obj)
{
    msg_Info(obj, "initializing niftyled output...");
    vout_display_t *vd = (vout_display_t *) obj;
            
    
       
    /* Allocate space for vout_display_sys structure */
    vout_display_sys_t *sys;
    vd->sys = sys = calloc(1, sizeof(*sys));
    if (!sys)
        return VLC_ENOMEM;


    /* check libniftyled binary version compatibility */
    if(!NFT_LED_CHECK_VERSION)
		return VLC_EGENERIC;

    /* register logging function to pipe niftyled logging through VLC */
    nft_log_func_register(_log, obj);
        
    /* set default loglevel */
	//nft_log_level_set(var_InheritInteger(vd, PROP_VERBOSITY));
    nft_log_level_set(L_INFO);

	sys->prefs = led_prefs_init();
	sys->frame = NULL;
	sys->setup = NULL;
	
	/* initialize babl */
	led_pixel_format_new();

	/* default returncode of Open() */
    	int result = VLC_EGENERIC;
	
	/* parse config-file */
	LedPrefsNode *pnode;
	char *filename = var_InheritString(vd, PROP_CFGFILE);
	if(!(pnode = led_prefs_node_from_file(filename)))
	{
		msg_Err(obj, "failed to load config from \"%s\"", filename);
		result = VLC_ENOITEM;
		free(filename);
		goto _o_error;
	}

	/* create setup from prefs-node */
	if(!(sys->setup = led_prefs_setup_from_node(sys->prefs, pnode)))
	{
		msg_Err(obj, "No valid setup found in preferences file \"%s\"", filename);
		led_prefs_node_free(pnode);
		result = VLC_ENOITEM;
		free(filename);	
		goto _o_error;
	}

	led_prefs_node_free(pnode);
    free(filename);

    /* get first toplevel hardware */
    if(!(sys->hw = led_setup_get_hardware(sys->setup)))
    {
        msg_Warn(obj, "No <hardware> found in config.");
        result = VLC_ENOITEM;
        goto _o_error;
    }

    /* determine dimensions of LED setup */
    if((sys->width = (LedFrameCord) var_InheritInteger(vd, PROP_WIDTH)) <= 0)
        sys->width = led_setup_get_width(sys->setup);
    if((sys->height = (LedFrameCord) var_InheritInteger(vd, PROP_HEIGHT)) <= 0)
        sys->height = led_setup_get_height(sys->setup);

    msg_Info(obj, "initialized %dx%d pixel LED setup", sys->width, sys->height);


    vout_display_DeleteWindow(vd, NULL);
        
    /* get video format */
    video_format_t fmt = vd->fmt;
        
    fmt.i_chroma = VLC_CODEC_RGB24;
    fmt.i_width  = (unsigned int) sys->width;
    fmt.i_height = (unsigned int) sys->height;
    
    fmt.i_rmask = 0x000000ff;
    fmt.i_gmask = 0x0000ff00;
    fmt.i_bmask = 0x00ff0000;
        
    /* get display info */
    vout_display_info_t info = vd->info;
    //info.has_pictures_invalid = true;
        
    /* copy structures */
    vd->fmt = fmt; 
    vd->info = info;

    /* register functions */
    vd->pool = Pool;
    vd->prepare = PreparePic;
    vd->display = DisplayPic;
    vd->control = Control;
        
        
    /* Inspect initial configuration and send correction events */
    //sys->state = *vd->cfg;
    //sys->state.is_fullscreen = false;
    vout_display_SendEventFullscreen(vd, false);
    vout_display_SendEventDisplaySize(vd, fmt.i_width, fmt.i_height, false);

    msg_Info(obj, "niftyled plugin initialization finished! :)");

    return VLC_SUCCESS;

_o_error:
    free(vd->sys);
    return result;
}


/**
 * free all resources
 */
static void Close( vlc_object_t *obj )
{
    msg_Info(obj, "deinitializing niftyled output...");

    vout_display_t *vd = (vout_display_t *)obj;
    vout_display_sys_t *sys = vd->sys;

    /* delete picture-pool */
    if (sys->pool)
        picture_pool_Delete(sys->pool);

    /* unregister callback */
    nft_log_func_register(NULL, NULL);
        
    /* free niftyled resources */
	led_hardware_list_destroy(sys->hw);
	led_frame_destroy(sys->frame);
    led_prefs_deinit(sys->prefs);
        
    /* free descriptor */    
    free(sys);
}


/**
 * Build a picture-pool for VLC to write to
 */
static picture_pool_t *Pool(vout_display_t *vd, unsigned count)
{
    vout_display_sys_t *sys = vd->sys;
    
    /* pool existing already? */
    if (sys->pool) 
            return sys->pool;
        
            
    /* allocate new frame (8bit R, 8bit G, 8bit B = 24 bpp) */
    LedPixelFormat *format = led_pixel_format_from_string("RGB u8");
    if(!(sys->frame = led_frame_new(sys->width, sys->height, format)))
        return NULL;

    /* respect endianess */
    //led_frame_set_big_endian(frame, capture_is_big_endian());

    /* print some debug-info */
    led_frame_print(sys->frame, L_DEBUG);
    led_hardware_print(sys->hw, L_DEBUG);
    
    /* initialize pixel->led mapping */
    if(!led_hardware_list_refresh_mapping(sys->hw))
            goto _p_error;
		
	/* precalc memory offsets for actual mapping */
    if(!led_chain_map_from_frame(led_hardware_get_chain(sys->hw), sys->frame))
            goto _p_error;
		
    /* set correct gain to hardware */
    if(!led_hardware_list_refresh_gain(sys->hw))
            goto _p_error;

    /* allocate our VLC buffer */
    picture_resource_t rsc;
    memset(&rsc, 0, sizeof(rsc));
    rsc.p->p_pixels = led_frame_get_buffer(sys->frame);
    rsc.p->i_pitch = sys->width*3;
    rsc.p->i_lines = sys->height;
    picture_t *p_pic = picture_NewFromResource(&vd->fmt, &rsc);
    if (!p_pic)
              goto _p_error;

    /* create pool from picture */
    sys->pool = picture_pool_New(1, &p_pic);
        
        
    return sys->pool;

_p_error:
        led_frame_destroy(sys->frame);
        return NULL;
}


/**
 * handle control events
 */
static int Control(vout_display_t *vd, int query, va_list args)
{
    switch (query) 
    {
        default:
            return VLC_EGENERIC;
    }
}


/**
 * prepare display of a picture
 */
static void PreparePic(vout_display_t *vd, picture_t *p, subpicture_t *s)
{
    //msg_Info(vd, "Prepare");
    vout_display_sys_t *sys = vd->sys;

    /*int y;
    unsigned char *buf = p->p->p_pixels;
    for(y=0; y < p->format.i_height; y++)
    {
        int x;
        for(x=0; x < p->format.i_width*3; x++)
        {
            fprintf(stdout, "0x%x ", *buf++);
        }
        fprintf(stdout, "\n");
    }
    fflush(stdout);*/


    /* fill chain of every hardware from frame-buffer */
    LedHardware *h;
    for(h = sys->hw; h; h = led_hardware_list_get_next(h))
    {
        if(!led_chain_fill_from_frame(led_hardware_get_chain(h), sys->frame))
        {
            msg_Info(vd, "Error while mapping frame");
            break;
        }
    }
        
    /* send frame to hardware(s) */
    led_hardware_list_send(sys->hw);
}


/**
 * display a picture
 */
static void DisplayPic(vout_display_t *vd, picture_t *p, subpicture_t *s)
{

    /* set VLC buffer as buffer in LedFrame */
    /*led_frame_set_buffer(sys->frame, p->p->p_pixels, 
                         p->p->i_pitch * 
                         p->p->i_lines * 3, NULL);*/
        
    vout_display_sys_t *sys = vd->sys;

    /* latch hardware */
    led_hardware_list_show(sys->hw);
        
    picture_Release(p);
}


