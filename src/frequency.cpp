#include "includes.hh"
#include "constants.hh"
#include "idct.hh"

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
  PixelToaster::Display display ("Frequency",
                                 WIDTH,
                                 HEIGHT);

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
