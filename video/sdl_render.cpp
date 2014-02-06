/*
 * Copyright (C) 2012 microcai <microcai@fedoraproject.org>
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA. 
 */
#define UINT64_C(c)     c ## ULL

extern          "C" {
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}
#include <boost/scope_exit.hpp>
#include <avplay.h>
#include <SDL.h>
#include "sdl_render.h"
#ifdef  __cplusplus
extern          "C" {
#endif

EXPORT_API int  sdl_init_video(void *ctx, int w, int h, int pix_fmt)
{
	vo_context     *vo = (vo_context *) ctx;
	sdl_render     *sdl = NULL;

	vo->priv = (void *) (sdl = new sdl_render);
	return sdl->init_render(vo->user_data, w, h, pix_fmt) ? 0 : -1;
}

EXPORT_API int sdl_render_one_frame(void *ctx, AVFrame * data,
					int pix_fmt, double pts)
{
	vo_context     *vo = (vo_context *) ctx;
	sdl_render     *sdl = (sdl_render *) vo->priv;
	return sdl->render_one_frame(data, pix_fmt) ? 0 : -1;
}

EXPORT_API void sdl_re_size(void *ctx, int width, int height)
{
	vo_context     *vo = (vo_context *) ctx;
	sdl_render     *sdl = (sdl_render *) vo->priv;
	sdl->re_size(width, height);
}

EXPORT_API void sdl_aspect_ratio(void *ctx, int srcw, int srch,
					int enable_aspect)
{
	vo_context     *vo = (vo_context *) ctx;
	sdl_render     *sdl = (sdl_render *) vo->priv;
	sdl->aspect_ratio(srcw, srch, enable_aspect);
}

EXPORT_API int  sdl_use_overlay(void *ctx)
{
	vo_context     *vo = (vo_context *) ctx;
	sdl_render     *sdl = (sdl_render *) vo->priv;
	return sdl->use_overlay()? 0 : -1;
}

EXPORT_API void sdl_destory_render(void *ctx)
{
	vo_context     *vo = (vo_context *) ctx;
	sdl_render     *sdl = (sdl_render *) vo->priv;
	if (sdl) {
		sdl->destory_render();
		delete          sdl;
		vo->priv = NULL;
	}
}

#ifdef  __cplusplus
}
#endif

bool
sdl_render::render_one_frame(AVFrame * frame, int pix_fmt)
{

    SDL_Texture * m_yuv;
    SDL_Rect        rect = {0};

    rect.w = m_image_width;
    rect.h = m_image_height;

	boost::mutex::scoped_lock l(renderlock);

	m_yuv = SDL_CreateTexture(m_render, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STATIC,  m_image_width , m_image_height);

	BOOST_SCOPE_EXIT(m_yuv)
	{
	    SDL_DestroyTexture(m_yuv);
	}BOOST_SCOPE_EXIT_END

    SDL_UpdateYUVTexture(m_yuv, NULL, frame->data[0], frame->linesize[0],  frame->data[1], frame->linesize[1],  frame->data[2], frame->linesize[2]);
	SDL_RenderCopy(m_render, m_yuv, NULL, &rect);
	SDL_RenderPresent(m_render);
/*
    AVPicture       pict = { {0} };

    uint8_t       **px = m_yuv->pixels;

    uint8_t        *pixels[3] = { data->data[0],
	data->data[1],
	data->data[2]
    };

    int             linesize[3] = { data->linesize[0],
	data->linesize[1],
	data->linesize[2]
    };

	m_swsctx = sws_getCachedContext(m_swsctx, m_image_width, m_image_height,
				(PixelFormat)m_pix_fmt, sfc->w, sfc->h, PIX_FMT_YUV420P,
				SWS_BICUBIC, NULL, NULL, NULL);
			     
    SDL_LockYUVOverlay(m_yuv);

    pict.data[0] = m_yuv->pixels[0];
    pict.data[1] = m_yuv->pixels[2];
    pict.data[2] = m_yuv->pixels[1];

    pict.linesize[0] = m_yuv->pitches[0];
    pict.linesize[1] = m_yuv->pitches[2];
    pict.linesize[2] = m_yuv->pitches[1];

    sws_scale(m_swsctx, pixels, linesize, 0, m_image_height, pict.data,
	      pict.linesize);

    SDL_UnlockYUVOverlay(m_yuv);
    rect.x = 0;
    rect.y = 0;
    rect.w = sfc->w;
    rect.h = sfc->h;

    SDL_DisplayYUVOverlay(m_yuv, &rect);

    m_yuv->pixels = px; */


    return true;
}

void
sdl_render::re_size(int w, int h)
{
	boost::mutex::scoped_lock l(renderlock);

	SDL_SetWindowSize(m_sdlwindow, w, h);
}

bool
sdl_render::init_render(void *ctx, int w, int h, int pix_fmt)
{
	m_sdlwindow = (SDL_Window*)ctx;
    m_swsctx = NULL;
    m_pix_fmt = pix_fmt;

    if (!SDL_WasInit(SDL_INIT_VIDEO))
	SDL_InitSubSystem(SDL_INIT_VIDEO);

	m_render = SDL_CreateRenderer(m_sdlwindow, -1, SDL_RENDERER_ACCELERATED);

    m_image_height = h;
    m_image_width = w;
    re_size(w, h);
    return 1;
}

void sdl_render::aspect_ratio(int srcw, int srch, bool enable_aspect)
{

}

void sdl_render::destory_render()
{
	SDL_DestroyRenderer(m_render);
	SDL_DestroyWindow(m_sdlwindow);
}

bool sdl_render::use_overlay()
{
    return true;
}
