#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/time.h>

#include "VG/openvg.h"
#include "VG/vgu.h"
#include "VG/vgext.h"
#include "EGL/egl.h"
#include "GLES/gl.h"
#include "bcm_host.h"

typedef struct {
	uint32_t screen_width;
	uint32_t screen_height;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	VGfloat normalized_screen[9];
} state_t;

static void init_egl(state_t *s)
{
	static EGL_DISPMANX_WINDOW_T nativewindow;
	static const EGLint attribute_list[] = {
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};

	int32_t success = 0;
	EGLBoolean result;
	EGLint num_config;
	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T dispman_update;
	VC_RECT_T dst_rect;
	VC_RECT_T src_rect;
	EGLConfig config;

	// get an EGL display connection
	s->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	assert(s->display != EGL_NO_DISPLAY);

	// initialize the EGL display connection
	result = eglInitialize(s->display, NULL, NULL);
	assert(EGL_FALSE != result);

	// bind OpenVG API
	eglBindAPI(EGL_OPENVG_API);

	// get an appropriate EGL frame buffer configuration
	result = eglChooseConfig(s->display, attribute_list, &config, 1, &num_config);
	assert(EGL_FALSE != result);

	// create an EGL rendering context
	s->context = eglCreateContext(s->display, config, EGL_NO_CONTEXT, NULL);
	assert(s->context != EGL_NO_CONTEXT);

	// create an EGL window surface
	success = graphics_get_display_size(0 /* LCD */ , &s->screen_width,
					    &s->screen_height);
	assert(success >= 0);

	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width = s->screen_width;
	dst_rect.height = s->screen_height;

	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width = s->screen_width << 16;
	src_rect.height = s->screen_height << 16;

	dispman_display = vc_dispmanx_display_open(0 /* LCD */ );
	dispman_update = vc_dispmanx_update_start(0);

	dispman_element = vc_dispmanx_element_add(dispman_update, dispman_display, 0 /*layer */ , &dst_rect, 0 /*src */ ,
						  &src_rect, DISPMANX_PROTECTION_NONE, 0 /*alpha */ , 0 /*clamp */ ,
						  0 /*transform */ );

	nativewindow.element = dispman_element;
	nativewindow.width = s->screen_width;
	nativewindow.height = s->screen_height;
	vc_dispmanx_update_submit_sync(dispman_update);

	s->surface = eglCreateWindowSurface(s->display, config, &nativewindow, NULL);
	assert(s->surface != EGL_NO_SURFACE);

	// preserve the buffers on swap
	result = eglSurfaceAttrib(s->display, s->surface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);
	assert(EGL_FALSE != result);

	// connect the context to the surface
	result = eglMakeCurrent(s->display, s->surface, s->surface, s->context);
	assert(EGL_FALSE != result);

	// set up screen ratio
	glViewport(0, 0, (GLsizei) s->screen_width, (GLsizei) s->screen_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float ratio = (float)s->screen_width / (float)s->screen_height;
	glFrustumf(-ratio, ratio, -1.0f, 1.0f, 1.0f, 10.0f);
}

void fini_egl(state_t *s)
{
	glClear(GL_COLOR_BUFFER_BIT);
	eglSwapBuffers(s->display, s->surface);
	eglMakeCurrent(s->display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroySurface(s->display, s->surface);
	eglDestroyContext(s->display, s->context);
	eglTerminate(s->display);
}

static VGPaint create_paint(const VGfloat rgba[4])
{
	VGPaint paint = vgCreatePaint();
	vgSetParameteri(paint, VG_PAINT_TYPE, VG_PAINT_TYPE_COLOR);
	vgSetParameterfv(paint, VG_PAINT_COLOR, 4, rgba);
	return paint;
}

static VGPaint create_gradient_paint(state_t *s)
{
	VGfloat lgcoord[4] = { 0, 0, s->screen_width, s->screen_height };
	VGfloat stops[] = {
		/* s   R    G    B    A  */
		0.0,  .20, .20, .20, 1.0,
		0.4,  .65, .65, .65, 1.0,
		0.45, .75, .75, .75, 1.0,
		0.6,  .65, .65, .65, 1.0,
		1.0,  .20, .20, .20, 1.0,
	};

	VGPaint paint = vgCreatePaint();
	vgSetParameteri(paint, VG_PAINT_TYPE, VG_PAINT_TYPE_LINEAR_GRADIENT);
	vgSetParameterfv(paint, VG_PAINT_LINEAR_GRADIENT, 4, lgcoord);

	//vgSetParameteri(paint, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_REPEAT);
	vgSetParameteri(paint, VG_PAINT_COLOR_RAMP_SPREAD_MODE, VG_COLOR_RAMP_SPREAD_REFLECT);
	vgSetParameteri(paint, VG_PAINT_COLOR_RAMP_PREMULTIPLIED, VG_FALSE);
	vgSetParameterfv(paint, VG_PAINT_COLOR_RAMP_STOPS, sizeof(stops)/sizeof(stops[0]), stops);

	return paint;
}

static VGPath create_big_hand(VGfloat size)
{
	VGfloat pts[] = {
#if 0
		0,		0,
		-0.035,		0.10,
		0,		0.85*size,
		+0.035,		0.10,
		0,		0
#else
		-0.01,		-0.1,
		-0.035,		0.85*size-0.1,
		0,		0.85*size,
		+0.035,		0.85*size-0.1,
		+0.01,		-0.1,
		-0.01,		-0.1,
#endif
	};
	VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
	vguPolygon(path, pts, sizeof(pts)/sizeof(pts[0])/2, VG_FALSE);
	return path;
}

static VGPath create_small_hand(VGfloat size)
{
	VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);

	VGfloat end = -0.18, r = 0.03;
	vguLine(path, .0, .85 * size, 0., end);
	vguRect(path, -0.01, end, 2*0.01, 2*r);
	vguEllipse(path, .0, .0, 2*r, 2*r);

	return path;
}

static VGPath create_rect(VGfloat x, VGfloat y, VGfloat w, VGfloat h)
{
	VGPath path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
	vguRect(path, x, y, w, h);
	return path;
}

static VGPath create_tick(VGfloat a, VGfloat b, VGfloat w)
{
	return create_rect(-w/2, a, w, b-a);
}

static const VGfloat rgba_white[4]  = { 1.0, 1.0, 1.0, 1.0 };
static const VGfloat rgba_red[4]    = { 1.0,   0,   0, 1.0 };
static const VGfloat rgba_black[4]  = {   0,   0,   0, 1.0 };
static const VGfloat rgba_silver[4] = { 0.3, 0.3, 0.3, 1.0 };

static VGImage load_ppm(const char *filename, int *logo_w, int *logo_h)
{
	FILE *fp = NULL;
	char *buf = NULL;
	VGImage image = VG_INVALID_HANDLE;
	unsigned int w, h, d;
	int r;

	fp = fopen(filename, "rb");
	if (!fp) goto err;

	if (fscanf(fp, "P6\n%u %u\n%u\n", &w, &h, &d) != 3) goto err;
	if (d != 255) goto err;

	buf = malloc(w*h*4);
	if (!buf) goto err;

	for (r = 0; r < w*h*4; r += 4) {
		buf[r+0] = fgetc(fp);
		buf[r+1] = fgetc(fp);
		buf[r+2] = fgetc(fp);
		buf[r+3] = 0xff;
	}

	image = vgCreateImage(VG_sABGR_8888, w, h, VG_IMAGE_QUALITY_BETTER);
	vgImageSubData(image, buf+w*4*(h-1), -w*4, VG_sABGR_8888, 0, 0, w, h);
	*logo_w = w;
	*logo_h = h;

err:
	if (fp) fclose(fp);
	free(buf);
	return image;
}

int main(int argc, char **argv)
{
	float fps = 5.0;
	state_t state = {0}, *s = &state;
	struct timeval tv;
	struct tm tm;
	int i, size, logo_w, logo_h;

	VGImage logo = VG_INVALID_HANDLE;
	VGPath mega_tick, big_tick, small_tick, hour_hand, minute_hand, second_hand, background_rect;
	VGPaint black_paint, red_paint, silver_paint, gradient_paint;

	bcm_host_init();
	init_egl(s);
	size = s->screen_height < s->screen_width ? s->screen_height : s->screen_width;

	/* create standard translation (center screen) */
	vgTranslate(s->screen_width / 2.0, s->screen_height / 2.0);
	vgScale(size / 2.0, size / 2.0);
	vgGetMatrix(s->normalized_screen);

	vgSeti(VG_STROKE_CAP_STYLE, VG_CAP_BUTT);
	vgSeti(VG_STROKE_JOIN_STYLE, VG_JOIN_MITER);

	if (argc > 1)
		logo = load_ppm(argv[1], &logo_w, &logo_h);

	gradient_paint = create_gradient_paint(s);
	black_paint = create_paint(rgba_black);
	silver_paint = create_paint(rgba_silver);
	red_paint = create_paint(rgba_red);

	mega_tick = create_tick(.71, .91, 0.04);
	big_tick = create_tick(.8, .9, 0.04);
	small_tick = create_tick(.825, .875, 0.01);

	hour_hand = create_big_hand(0.5);
	minute_hand = create_big_hand(0.9);
	second_hand = create_small_hand(0.95);
	background_rect = create_rect(0, 0, s->screen_width, s->screen_height);

	assert(vgGetError() == VG_NO_ERROR);

	while (1) {
		/* clear */
		vgLoadIdentity();
		vgSetPaint(gradient_paint, VG_FILL_PATH);
		vgDrawPath(background_rect, VG_STROKE_PATH | VG_FILL_PATH);

		/* clock face */
		vgLoadMatrix(s->normalized_screen);
		vgSetPaint(black_paint, VG_FILL_PATH);
		//vgSetPaint(black_paint, VG_STROKE_PATH);
		vgSetPaint(silver_paint, VG_STROKE_PATH);
		vgSetf(VG_STROKE_LINE_WIDTH, 0.008);
		for (i = 0; i < 60; i++) {
			if (i % 15 == 0) {
				vgDrawPath(mega_tick, VG_FILL_PATH | VG_STROKE_PATH);
			} else if (i % 5 == 0) {
				vgDrawPath(big_tick, VG_FILL_PATH);
			} else {
				vgDrawPath(small_tick, VG_FILL_PATH);
			}
			vgRotate(360/60.0);
		}

		/* logo */
		if (logo != VG_INVALID_HANDLE) {
			vgSeti(VG_MATRIX_MODE, VG_MATRIX_IMAGE_USER_TO_SURFACE);
			vgLoadIdentity();
			//vgTranslate(s->screen_width/2, 24*s->screen_height/32);
			vgTranslate(s->screen_width/2, 1*s->screen_height/4);
			vgScale(0.3, 0.3);
			vgTranslate(-logo_w/2, -logo_h/2);
			vgDrawImage(logo);
			vgSeti(VG_MATRIX_MODE, VG_MATRIX_PATH_USER_TO_SURFACE);
		}

		/* time */
		gettimeofday(&tv, 0);
		localtime_r(&tv.tv_sec, &tm);

		//tm.tm_hour = 7;
		//tm.tm_min = 20;

		vgSetf(VG_STROKE_LINE_WIDTH, 0.008);
		vgSetPaint(silver_paint, VG_STROKE_PATH);
		vgSetPaint(black_paint, VG_FILL_PATH);

		vgLoadMatrix(s->normalized_screen);
		vgRotate(-360.0/12.0 * (tm.tm_min/60.0 + tm.tm_hour));
		vgDrawPath(hour_hand, VG_STROKE_PATH | VG_FILL_PATH);

		vgLoadMatrix(s->normalized_screen);
		vgRotate(-360.0/60.0 * (tm.tm_sec/60.0 + tm.tm_min));
		vgDrawPath(minute_hand, VG_STROKE_PATH | VG_FILL_PATH);

		vgSetPaint(red_paint, VG_STROKE_PATH);
		vgSetPaint(red_paint, VG_FILL_PATH);
		vgSetf(VG_STROKE_LINE_WIDTH, 0.01);
		vgLoadMatrix(s->normalized_screen);
		vgRotate(-360.0/60.0 * tm.tm_sec);
		vgDrawPath(second_hand, VG_STROKE_PATH | VG_FILL_PATH);

		assert(vgGetError() == VG_NO_ERROR);
		eglSwapBuffers(s->display, s->surface);
		assert(eglGetError() == EGL_SUCCESS);

		usleep(1000000/fps);
		//usleep(50 * 1000);
	}

	fini_egl(s);
	return 0;
}
