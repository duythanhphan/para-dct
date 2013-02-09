#include "includes.hh"
#include "constants.hh"
#include "data.hh"
#include "dct.hh"
#include "idct.hh"

int main ()
{
  PixelToaster::Display display ("Frequency",
                                 WIDTH,
                                 HEIGHT);

  srand (time(NULL));
  Data data (WIDTH, HEIGHT, FREQ_W, FREQ_H);
  data.random_freq ();

  PixelToaster::vector<PixelToaster::Pixel> pix_val;
  unsigned long size = FREQ_W * FREQ_H;
  unsigned long step = size / NB_WORKERS;
  pix_val.resize(size);


  while (display.open())
  {
    random_freqs_v (freqs);
    tbb::parallel_for(tbb::blocked_range<unsigned>(0, size, step, Idct (&data)));
    data.points2pixels (pix_val);
    display.update(pix_val);
  }

  display.close ();
  return 0;
}
