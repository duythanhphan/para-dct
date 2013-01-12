#ifndef IDCT_HH_
# define IDCT_HH_

# include "includes.hh"
# include "constants.hh"

class Idct
{
  protected:
    float *pixels_;
    float *freqs_;
    t_float4d coefs_;
    unsigned width_;
    unsigned height_;
    unsigned fwidth_;
    unsigned fheight_;
    float aw1_, aw2_;
    float ah1_, ah2_;

  public:
    Idct (float *pix,
          float *freqs,
          t_float4d cfs,
          unsigned w = WIDTH,
          unsigned h = HEIGHT,
          unsigned fw = FREQ_W,
          unsigned fh = FREQ_H);
    void operator () (const tbb::blocked_range<unsigned>& r) const;
};


#endif /* !IDCT_HH_ */
