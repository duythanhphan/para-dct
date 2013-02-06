#include "dct.hh"

Dct::Dct (Data* data)
  : data_ (data)
{
}

void Dct::operator () (const tbb::blocked_range<unsigned>& r) const
{
  data_->ranged_dct (r.begin (), r.end ());
};
