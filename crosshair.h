#ifndef crosshair_include_file
#define crosshair_include_file

#ifdef __cplusplus
extern "C" {
#endif

#define crosshair_width 16
#define crosshair_height 16
#define crosshair_size 258
#define crosshair ((gfx_sprite_t*)crosshair_data)
extern unsigned char crosshair_data[258];

#ifdef __cplusplus
}
#endif

#endif
