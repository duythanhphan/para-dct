#include "idct.hh"

Idct::Idct (Data* data)
  : data_ (data)
{
}

void Idct::operator () (const tbb::blocked_range<unsigned>& r) const
{
  data_->ranged_idct (r.begin (), r.end ());
};
