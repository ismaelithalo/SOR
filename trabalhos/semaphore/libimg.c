#include "libimg.h"

ppm_image alloc_img(unsigned int width, unsigned int height)
{
    ppm_image img;
    img = malloc(sizeof(image_t));
    img->buf = malloc(width * height * sizeof(pixel));
    img->width = width;
    img->height = height;
    img->high = 0;
    img->max_color = 255; //default
    return img;
}
 
void free_img(ppm_image img)
{
    free(img->buf);
    free(img);
}
 
void fill_img(
        ppm_image img,
        color_component r,
        color_component g,
        color_component b )
{
    unsigned int i, n;
    n = img->width * img->height;
    for (i=0; i < n; ++i)
    {
        img->buf[i][0] = r;
        img->buf[i][1] = g;
        img->buf[i][2] = b;
    }
}

void put_pixel_unsafe(
       	ppm_image img,
        unsigned int x,
        unsigned int y,
        color_component r,
        color_component g,
        color_component b )
{
    unsigned int ofs;
    ofs = (y * img->width) + x;
    img->buf[ofs][0] = r;
    img->buf[ofs][1] = g;
    img->buf[ofs][2] = b;
}
 
void put_pixel_clip(
       	ppm_image img,
        unsigned int x,
        unsigned int y,
        color_component r,
        color_component g,
        color_component b )
{
    if (x < img->width && y < img->height)
      put_pixel_unsafe(img, x, y, r, g, b);
}

ppm_image get_ppm(FILE *pf)
{
        char buf[PPMREADBUFLEN], *t;
        ppm_image img;
        unsigned int w, h, d;
        int r;
 
        if (pf == NULL) return NULL;
        t = fgets(buf, PPMREADBUFLEN, pf);
        /* the code fails if the white space following "P6" is not '\n' */
        if ( (t == NULL) || ( strncmp(buf, "P6\n", 3) != 0 ) ) return NULL;
        do
        { /* Px formats can have # comments after first line */
           t = fgets(buf, PPMREADBUFLEN, pf);
           if ( t == NULL ) return NULL;
        } while ( strncmp(buf, "#", 1) == 0 );
        r = sscanf(buf, "%u %u", &w, &h);
        if ( r < 2 ) return NULL;
 
        r = fscanf(pf, "%u", &d);
        if ( (r < 1) || ( d != 255 ) ) return NULL;
        fseek(pf, 1, SEEK_CUR); /* skip one byte, should be whitespace */
 
        img = alloc_img(w, h);
        if ( img != NULL )
        {
            img->high = 0;
            size_t rd = fread(img->buf, sizeof(pixel), w*h, pf);
            if ( rd < w*h )
            {
               free_img(img);
               return NULL;
            }
            return img;
        }
        return NULL;
}

void output_ppm(FILE *fd, ppm_image img)
{
  unsigned int n;
  (void) fprintf(fd, "P6\n%d %d\n%d\n", img->width, img->height, img->max_color);
  n = img->width * img->height;
  (void) fwrite(img->buf, sizeof(pixel), n, fd);
  (void) fflush(fd);
}

void to_greyscale(ppm_image img)
{
    float fr, fg, fb;

    unsigned int i,j;

    for(i = 0 ; i < img->width ; i++)
    {
        for(j = 0; j < img->height ; j++)
        {
            fr = (float) GET_PIXEL(img, i,j)[0]/255.0f;
            fg = (float) GET_PIXEL(img, i,j)[1]/255.0f;
            fb = (float) GET_PIXEL(img, i,j)[2]/255.0f;

            float fwb = sqrt((fr*fr + fg*fg + fb*fb)/3.0f);

            unsigned char c = (unsigned char) (fwb*255);
            if(c > img->high)
                img->high = c;

            put_pixel_unsafe(img, i,j, c, c, c);

        }
    }
}

float packColor(color_component r, color_component g, color_component b)
{
    return (float) (r + g * 255.0f + b * 255.0f * 255.0f);
}

void unpackColor(float f, color_component *r, color_component *g, color_component *b)
{
    *b = floor(f/255.0f/255.0f);
    *g = floor((f - (*b) * 255.0f * 255.0f) / 255.0f);
    *r = floor(f - (*b) * 255.0f * 255.0f - (*g) * 255.0f);
    
    //printf("rgb = %d,%d,%d\n",*r,*g,*b);
    
    //*b = *b / 255.0f;
    //*g = *g / 255.0f;
    //*r = *r / 255.0f;

    
}

double get_time()
{
    struct timespec ts;
    double ret;
    
    clock_gettime(CLOCK_REALTIME, &ts);
    
    ret = (double)((((double)ts.tv_sec) * 1000) + (((double)ts.tv_nsec) / 1000000));
    return ret;
}

