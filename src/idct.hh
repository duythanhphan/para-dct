#ifndef IDCT_HH_
# define IDCT_HH_

# include "includes.hh"
# include "constants.hh"
# include "data.hh"

class Idct
{
  protected:
    Data *data_;

  public:
    Idct (Data *data_);
    void operator () (const tbb::blocked_range<unsigned>& r) const;
};


#endif /* !IDCT_HH_ */
