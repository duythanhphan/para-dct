#ifndef COEF_COMPUTER_HH_
# define COEF_COMPUTER_HH_

# include "includes.hh"
# include "constants.hh"

class CoefComputer
{
  protected:
    t_float4d coefs_;
    float width_;
    float height_;
    float fwidth_;
    float fheight_;
    float cst_w;
    float cst_h;

  public:
    CoefComputer (t_float4d coefs,
                  unsigned w,
                  unsigned h,
                  unsigned fw,
                  unsigned fh);

    void operator () (const tbb::blocked_range<unsigned>& r) const;
};
#endif /* !COEF_COMPUTER_HH_ */

