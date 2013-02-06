#ifndef DATA_HH_
# define DATA_HH_

# include "includes.hh"
# include "constants.hh"

class Data
{
  public:
    Data (size_t w, size_t h, size_t fw, size_t fh);
    ~Data ();
    void random_freq ();

    const t_real *freq_get ();
    const t_real *points_get ();
    void ranged_dct (size_t begin, size_t end);
    void ranged_idct (size_t begin, size_t end)

  private:
    void compute_coef ();
    t_arr2d coefsxy_;
    t_arr2d coefsuv_;
    t_real  *points_;
    t_real  *frequencies_;
    size_t  width_;
    size_t  height_;
    size_t  fwidth_;
    size_t  fheight_;
    size_t  size_;
    size_t  fsize_;
    t_real  aw1_;
    t_real  aw2_;
    t_real  ah1_;
    t_real  ah2_;
};


#endif /* DATA_HH_ */
