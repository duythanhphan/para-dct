#include "includes.hh"
#include "constants.hh"
#include "idct.hh"

void random_freqs (float* freqs)
{
  float* ptr = freqs;

  float frand = rand();
  *ptr = 50.f * (frand / RAND_MAX) + 10.f;

  for (unsigned v = 0; v < MAX_FH; ++v)
  {
    for (unsigned u = 0; u < MAX_FW; ++u)
    {
      if (u || v)
      {
        float frand = rand();
        float dist = 10.f / (u * u + v * v);
        *ptr = dist * (1.f - 2.f * frand / RAND_MAX);
      }
      ++ptr;
    }
  }
}

void random_freqs_v (float* freqs)
{
  float* ptr = freqs;

  float frand = rand();
  *ptr += 0.1f * (1.f - 2.f * frand / RAND_MAX);

  if (*ptr < 10)
    *ptr = 10.f;
  if (*ptr > 200)
    *ptr = 200;

  for (unsigned v = 0; v < MAX_FH; ++v)
  {
    for (unsigned u = 0; u < MAX_FW; ++u)
    {
      if (u || v)
      {
        float frand = rand();
        float dist = 0.8f / sqrt (u * u + v * v);
        *ptr += dist * (1.f - 2.f * frand / RAND_MAX);
      }
      ++ptr;
    }
  }
}


t_float4d allocate_float4d (unsigned d1, unsigned d2, unsigned d3, unsigned d4)
{
  t_float4d res = (t_float4d) malloc (sizeof (t_float3d) * d1);
  for (unsigned i1 = 0; i1 < d1; ++i1)
  {
    res[i1] = (t_float3d) malloc (sizeof (t_float2d) * d2);
    for (unsigned i2 = 0; i2 < d2; ++i2)
    {
      res[i1][i2] = (t_float2d) malloc (sizeof (t_float1d) * d3);
      for (unsigned i3 = 0; i3 < d3; ++i3)
        res[i1][i2][i3] = (t_float1d) malloc (sizeof (float) * d4);
    }
  }
  return res;
}

void compute_coef (t_float4d coefs, unsigned W, unsigned H)
{
  float cst_w = M_PI / (2.f * W);
  float cst_h = M_PI / (2.f * H);


  for (unsigned y = 0; y < H; ++y)
  {
    for (unsigned x = 0; x < W; ++x)
    {
      for (unsigned u = 0; u < MAX_FW; ++u)
      {
        float cst = u * cst_w;
        for (unsigned v = 0; v < MAX_FH; ++v)
          coefs[x][y][u][v] = cos (cst * (2.f * x + 1.f)) * cos (cst_h * v * (2.f * y + 1));
      }
    }
  }
}

void dct (t_float4d coefs, const float* pts, float* freqs, unsigned W, unsigned H)
{
  float aw1 = 1.f / sqrt(W);
  float aw2 = sqrt(2.f / W);
  float ah1 = 1.f / sqrt(H);
  float ah2 = sqrt(2.f / H);

  float aw = aw1;
  float ah = ah1;


  float* ptr_f = freqs;

  for (unsigned v = 0; v < MAX_FH; ++v)
  {
    for (unsigned u = 0; u < MAX_FW; ++u)
    {
      const float* ptr_p = pts;

      *ptr_f = 0;
      for (unsigned y = 0; y < H; ++y)
      {
        for (unsigned x = 0; x < W; ++x)
        {
          *ptr_f += coefs[x][y][u][v] *  *(ptr_p);
          ++ptr_p;
        }
      }
      *ptr_f *= aw * ah;
      ++ptr_f;
      aw = aw2;
    }
    ah = ah2;
  }
}



void idct (t_float4d coefs, float* pts, const float* freqs, unsigned W, unsigned H)
{
  float aw1 = 1.f / sqrt(W);
  float aw2 = sqrt(2.f / W);
  float ah1 = 1.f / sqrt(H);
  float ah2 = sqrt(2.f / H);

  float* ptr_p = pts;


  for (unsigned y = 0; y < H; ++y)
  {
    for (unsigned x = 0; x < W; ++x)
    {
      const float* ptr_f = freqs;
      float aw = aw1;
      float ah = ah1;
      *ptr_p = 0;
      for (unsigned v = 0; v < MAX_FH; ++v)
      {
        for (unsigned u = 0; u < MAX_FW; ++u)
        {
          *ptr_p += aw * ah * (*ptr_f) * coefs[x][y][u][v];
          ++ptr_f;
          aw = aw2;
        }
        ah = ah2;
      }
      ++ptr_p;
    }
  }
}

void print_arr (float* arr, unsigned size)
{
  float* ptr = arr;
  for (unsigned i = 0; i < size; ++i, ++ptr)
    printf(" %f |", *ptr);
}

void to_pixels (float* pix, PixelToaster::vector<PixelToaster::Pixel>& res, unsigned w, unsigned h)
{
  unsigned int s = w * h;

  for (unsigned i = 0; i < s; ++i)
  {
    PixelToaster::Pixel & p = res[i];
    p.r = pix[i];
    p.b = pix[i];
    p.g = pix[i];
  }
}

int main ()
{
  int WIDTH = 0;
  int HEIGHT = 0;


  float* pixels = NULL;

 // if ( !load("ExampleImage.tga", WIDTH, HEIGHT, &pixels ) &&
 //     !load( "../ExampleImage.tga", WIDTH, HEIGHT, &pixels ) )
 // {
 //   fprintf(stderr, "failed to load image\n" );
 //   return 1;
 // }

  WIDTH = 128;
  HEIGHT = 128;

  pixels = (float*)malloc(sizeof (float) * WIDTH * HEIGHT);
  PixelToaster::Display display ("Frequency",
                                 WIDTH,
                                 HEIGHT);

  float* freqs = (float*)malloc(sizeof (float) * MAX_FW * MAX_FH);
  t_float4d coefs = allocate_float4d (WIDTH, HEIGHT, MAX_FW, MAX_FH);

  tbb::parallel_for(tbb::blocked_range<unsigned>(0, HEIGHT, HEIGHT / 8),
                      CoefComputer (coefs, WIDTH, HEIGHT, MAX_FW, MAX_FH));

  random_freqs (freqs);

  srand (time(NULL));
  PixelToaster::vector<PixelToaster::Pixel> pix_val;
  unsigned s = WIDTH * HEIGHT;
  pix_val.resize(s);


  while (display.open())
  {
    random_freqs_v (freqs);
//


 //   struct timeval begin, end;
 //   gettimeofday(&begin, NULL);

    tbb::parallel_for(tbb::blocked_range<unsigned>(0, HEIGHT, HEIGHT / 8),
                      IDCT (pixels, freqs, coefs, WIDTH, HEIGHT, MAX_FW, MAX_FH));

 //   gettimeofday(&end, NULL);
 //   double diff_sec = difftime(end.tv_sec, begin.tv_sec) * 1000;
 //   double diff_micro = difftime(end.tv_usec, begin.tv_usec) / 1000;

 //   std::cout<<"diff (in milliseconds): "<< (diff_sec + diff_micro)  <<std::endl;

    to_pixels (pixels, pix_val, WIDTH, HEIGHT);
    display.update(pix_val);


 //   gettimeofday(&begin, NULL);
 //   idct (coefs, pixels, freqs, WIDTH, HEIGHT);

 //   gettimeofday(&end, NULL);
 //   diff_sec = difftime(end.tv_sec, begin.tv_sec) * 1000;
 //   diff_micro = difftime(end.tv_usec, begin.tv_usec) / 1000;

 //   std::cout<<"diff (in milliseconds): "<< (diff_sec + diff_micro)<<std::endl;


  //  tm.tv_sec = 0;
  //  tm.tv_nsec = 40000000;
//    nanosleep(&tm, NULL);

// display.close ();
  }

  free(freqs);
  free(pixels);
  free(coefs);
  return 0;
}
