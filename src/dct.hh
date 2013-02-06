#ifndef DCT_HH_
# define DCT_HH_

# include "includes.hh"
# include "constants.hh"
# include "data.hh"

class Dct
{
  protected:
    Data *data_;

  public:
    Dct (Data *data_);
    void operator () (const tbb::blocked_range<unsigned>& r) const;
};


#endif /* IDCT_HH_ */
