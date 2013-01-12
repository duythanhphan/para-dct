#include "coef_computer.hh"

CoefComputer::CoefComputer (t_float4d coefs,
                            unsigned w,
                            unsigned h,
                            unsigned fw,
                            unsigned fh)
: coefs_ (coefs),
  width_ (w),
  height_ (h),
  fwidth_ (fw),
  fheight_ (fh)

{
  cst_w = M_PI / (2.f * w);
  cst_h = M_PI / (2.f * h);
}

void CoefComputer::operator () (const tbb::blocked_range<unsigned>& r) const
{
  for (unsigned y = r.begin(); y != r.end (); ++y)
  {
    for (unsigned x = 0; x < width_; ++x)
    {
      for (unsigned u = 0; u < fwidth_; ++u)
      {
        float cst = u * cst_w;
        for (unsigned v = 0; v < fheight_; ++v)
          coefs_[x][y][u][v] = cos (cst * (2.f * x + 1.f)) *
                               cos (cst_h * v * (2.f * y + 1));
      }
    }
  }
}
