#ifndef ICON_DATA_H_INCLUDED
#define ICON_DATA_H_INCLUDED

#define ICON_WIDTH  13
#define ICON_HEIGHT 13

static unsigned char icon_playing[ICON_WIDTH * ICON_HEIGHT] =
{
      0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,
      0,   0,   0, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,
      0,   0,   0, 255, 255, 255, 255, 255, 255, 127,   0,   0,   0,
      0,   0,   0, 255, 255, 255, 255, 255, 127,   0,   0,   0,   0,
      0,   0,   0, 255, 255, 255, 255, 127,   0,   0,   0,   0,   0,
      0,   0,   0, 255, 255, 255, 127,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 255, 255, 127,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 255, 127,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 127,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static unsigned char icon_paused[ICON_WIDTH * ICON_HEIGHT] =
{
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0, 255, 255,   0,   0, 255, 255,   0,   0,   0,   0,
      0,   0,   0, 255, 255,   0,   0, 255, 255,   0,   0,   0,   0,
      0,   0,   0, 255, 255,   0,   0, 255, 255,   0,   0,   0,   0,
      0,   0,   0, 255, 255,   0,   0, 255, 255,   0,   0,   0,   0,
      0,   0,   0, 255, 255,   0,   0, 255, 255,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static unsigned char icon_stopped[ICON_WIDTH * ICON_HEIGHT] =
{
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0, 255, 255, 255, 255, 255,   0,   0,   0,   0,
      0,   0,   0,   0, 255, 255, 255, 255, 255,   0,   0,   0,   0,
      0,   0,   0,   0, 255, 255, 255, 255, 255,   0,   0,   0,   0,
      0,   0,   0,   0, 255, 255, 255, 255, 255,   0,   0,   0,   0,
      0,   0,   0,   0, 255, 255, 255, 255, 255,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static unsigned char icon_buffering[ICON_WIDTH * ICON_HEIGHT] =
{
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0, 255,   0,   0,   0,   0,   0,   0,
      0,   0, 139,  86,   0,   0, 255,   0,   0,  86, 139,   0,   0,
      0,   0,  86, 233,  64,   0, 255,   0,  64, 233,  86,   0,   0,
      0,   0,   0,  64, 196,   0,   0,   0, 196,  64,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0, 255, 255, 255,   0,   0,   0,   0,   0, 255, 255, 255,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,  64, 196,   0,   0,   0, 196,  64,   0,   0,   0,
      0,   0,  86, 233,  64,   0, 255,   0,  64, 233,  86,   0,   0,
      0,   0, 139,  86,   0,   0, 255,   0,   0,  86, 139,   0,   0,
      0,   0,   0,   0,   0,   0, 255,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

static unsigned char icon_waiting[ICON_WIDTH * ICON_HEIGHT] =
{
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0, 128, 128,   0,   0, 128, 128,   0,   0, 128, 128,   0,   0,
      0, 128, 128,   0,   0, 128, 128,   0,   0, 128, 128,   0,   0,
      0, 128, 128,   0,   0, 128, 128,   0,   0, 128, 128,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
};

#endif // !ICON_DATA_H_INCLUDED
