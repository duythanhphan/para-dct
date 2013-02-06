#include "tools.hh"

t_arr4d allocate_arr4d (size_t d1, size_t d2, size_t d3, size_t d4)
{
  t_arr4d res = (t_arr4d) malloc (sizeof (t_arr3d) * d1);
  for (size_t i1 = 0; i1 < d1; ++i1)
  {
    res[i1] = (t_arr3d) malloc (sizeof (t_arr2d) * d2);
    for (size_t i2 = 0; i2 < d2; ++i2)
    {
      res[i1][i2] = (t_arr2d) malloc (sizeof (t_arr1d) * d3);
      for (size_t i3 = 0; i3 < d3; ++i3)
        res[i1][i2][i3] = (t_arr1d) malloc (sizeof (t_real) * d4);
    }
  }
  return res;
}

t_arr2d allocate_arr2d (size_t d1, size_t d2)
{
  t_arr2d res = (t_arr2d) malloc (sizeof (t_arr1d) * d1);
  for (size_t i = 0; i < d1; ++i)
  {
    res[i] = (t_arr1d) malloc (sizeof (t_real) * d2);
  }
  return res;
}
