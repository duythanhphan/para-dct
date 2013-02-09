#include "data.hh"

Data::Data (size_t w, size_t h, size_t fw, size_t fh)
  : width_ (w),
    height_ (h),
    fwidth_ (fw),
    fheight_ (fh)
{
  size_ = w * h;
  fsize_ = fw * fh;

  coefsxy_ = allocate_float2d (size_, fsize_);
  coefsuv_ = allocate_float2d (fsize_, size_);

  self->compute_coef ();

  points_ = malloc (sizeof (t_real) * w * h);
  frequencies_ = malloc (sizeof (t_real) * fw * fh);
  aw1_ = 1.f / sqrt(w);
  aw2_ = sqrt(2.f / w);
  ah1_ = 1.f / sqrt(h);
  ah2_ = sqrt(2.f / h);
}

Data::~Data ()
{
  //free up memory !
  if (points_)
    free (points_);
  if (frequencies_)
    free (frequencies_);

  for (size_t i = 0; i < size_; ++i)
    free (coefsxy_[i]);
  free (coefsxy_);

  for (size_t i = 0; i < fsize_; ++i)
    free (coefsuv_[i]);
  free (coefuv_);
}

void Data::compute_coef ()
{
  t_real cst_w = M_PI / (2.f * width_);
  t_real cst_h = M_PI / (2.f * height_);


  for (size_t y = 0; y < height_; ++y)
  {
    for (size_t x = 0; x < width_; ++x)
    {
      for (size_t u = 0; u < fwidth_; ++u)
      {
        t_real cst = u * cst_w;
        for (size_t v = 0; v < fheight_; ++v)
        {
          t_real val =
            cos (cst * (2.f * x + 1.f)) * cos (cst_h * v * (2.f * y + 1));

          size_t xy = x + y * width_;
          size_t uv = u + v * fheight_;

          coefsxy_[xy][uv] = val;
          coefsuv_[uv][xy] = val;
      }
    }
  }
}

void Data::points2pixels (PixelToaster::vector<PixelToaster::Pixel>& res)
{
  t_real *p_ptr = points_;
  for (unsigned i = 0; i < size_; ++i, ++p_ptr)
  {
    PixelToaster::Pixel & p = res[i];
    p.r = *(p_ptr);
    p.b = *(p_ptr);
    p.g = *(p_ptr);
  }
}

void Data::points2pixels (PixelToaster::vector<PixelToaster::Pixel>& res)
{
  t_real *f_ptr = frequencies_;
  for (unsigned i = 0; i < fsize_; ++i, ++f_ptr)
  {
    PixelToaster::Pixel & p = res[i];
    p.r = *(f_ptr);
    p.b = *(f_ptr);
    p.g = *(f_ptr);
  }
}

void Data::random_freq ()
{
  t_real* ptr = freqs;

  t_real frand = (t_real)rand ();

  // Mean value randomisation
  *ptr = 50.f * (frand / RAND_MAX) + 10.f;

  for (size_t v = 0; v < f_height; ++v)
  {
    for (size_t u = 0; u < f_width; ++u)
    {
      if (u || v)
      {
        t_real frand = (t_real)rand ();
        t_real dist = 10.f / (u * u + v * v);
        *ptr = dist * (1.f - 2.f * frand / RAND_MAX);
      }
      ++ptr;
    }
  }
}

const t_real *Data::freq_get ()
{
  return frequencies_;
}

const t_real *Data::points_get ()
{
  return points_;
}

void Data::ranged_idct (size_t begin, size_t end)
{
  t_real* ptr_p = pixels_ + begin;
  t_arr2d ptr_cxy = coefsxy_ + begin;

  t_real awah1 = aw_1 * ah_1;
  t_real awah2 = aw_2 * ah_1;
  t_real awah3 = aw_2 * ah_2;

  for (size_t i = begin; i < end; ++i, ++ptr_p, ++ptr_c)
  {
    const t_real* ptr_f = frequencies_;
    t_arr1d ptr_cuv = *ptr_cxy;

    // Mean

    *ptr_p += awah1 * (*(ptr_f++)) * (*(ptr_cuv++));

    for (size_t u = 1; u < fwidth_; ++u)
      *ptr_p += awah2 * (*(ptr_f++)) * (*(ptr_cuv++));

    for (size_t f = f_width_; f < fsize_; ++f)
      *ptr_p += awah3 * (*(ptr_f++)) * (*(ptr_cuv++));
  }
}

void Data::ranged_dct (size_t begin, size_t end)
{
  t_real* ptr_f = frequencies_ + begin;
  t_arr2d ptr_cuv = coefsuv_ + begin;

  t_real awah1 = aw_1 * ah_1;
  t_real awah2 = aw_2 * ah_1;
  t_real awah3 = aw_2 * ah_2;

  *ptr_f = 0;

  t_arr1d ptr_cxy = *(ptr_cuv++);
  t_real* ptr_p = points_;

  for (size_t p = 0; p < size_; ++p)
    *ptr_f = (*(ptr_cxy++)) * *((ptr_p)++);
  *ptr_f *= awah1;

  for (size_t u = 0; u < fwidth_; ++u)
  {
    ptr_cxy = *(ptr_cuv++);
    ptr_p = points_;
    for (size_t p = 0; p < size_; ++p)
      *ptr_f = (*(ptr_cxy++)) * *((ptr_p)++);
    *ptr_f *= awah2;
  }

  for (size_t f = f_width_; f < end; ++f)
  {
    ptr_cxy = *(ptr_cuv++);
    ptr_p = points_;
    for (size_t p = 0; p < size_; ++p)
      *ptr_f = (*(ptr_cxy++)) * *((ptr_p)++);
    *ptr_f *= awah3;
  }
}
