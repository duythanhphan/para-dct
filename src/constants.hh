#ifndef CONSTANTS_HH_
# define CONSTANTS_HH_

# define WIDTH 100
# define HEIGHT 100
# define FREQ_W 64
# define FREQ_H 64
# define HB(v) (v < 255 ? v : 255)
# define LB(v) (v < 0 ? 0 : v)
# define NB_WORKERS 64
# define INV_255 0.00392157
# define PI2 6.28318530
# define COEF 0.024639942


typedef float t_real;
typedef t_real**** t_arr4d;
typedef t_real*** t_arr3d;
typedef t_real** t_arr2d;
typedef t_real* t_arr1d;


#endif /* CONSTANTS_HH_ */
