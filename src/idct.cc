#include "idct.hh"


Idct::Idct (float *pix,
            float *freqs,
            t_float4d cfs,
            unsigned w,
            unsigned h,
            unsigned fw,
            unsigned fh)
: pixels_ (pix),
  freqs_ (freqs),
  coefs_ (cfs),
  width_ (w),
  height_ (h),
  fwidth_ (fw),
  fheight_ (fh)
{
  aw1_ = 1.f / sqrt(w);
  aw2_ = sqrt(2.f / w);
  ah1_ = 1.f / sqrt(h);
  ah2_ = sqrt(2.f / h);
}

void Idct::operator () (const tbb::blocked_range<unsigned>& r) const
{
  float* ptr_p = pixels_ + width_ * r.begin();

  //std::cout << "Size : " << r.begin() << " " << r.end() << std::endl;
  for (unsigned y = r.begin(); y != r.end (); ++y)
  {
    for (unsigned x = 0; x < width_; ++x)
    {
      const float* ptr_f = freqs_;
      float aw = aw1_;
      float ah = ah1_;
      *ptr_p = 0;
      for (unsigned v = 0; v < fheight_; ++v)
      {
        for (unsigned u = 0; u < fwidth_; ++u)
        {
          *ptr_p += aw * ah * (*ptr_f) * coefs_[x][y][u][v];
          ++ptr_f;
          aw = aw2_;
        }
        ah = ah2_;
      }
      ++ptr_p;
    }
  }
}
};
