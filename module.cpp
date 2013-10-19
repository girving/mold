// Smooth mold renderer

#include <other/core/array/Array3d.h>
#include <other/core/array/convert.h>
#include <other/core/random/Sobol.h>
#include <other/core/python/module.h>
#include <other/core/python/wrap.h>
#include <other/core/utility/interrupts.h>
#include <other/core/utility/ProgressIndicator.h>
#include <other/core/vector/Rotation.h>
#include <other/core/vector/Vector.h>
namespace other {
ARRAY_CONVERSIONS(3,Vector<double,2>)
}
namespace mold {

using namespace other;
typedef double T;
typedef Vector<T,2> TV;
typedef Vector<int,2> IV;
using std::cout;
using std::endl;
using std::asin;

static inline IV floor_div(const IV a, const IV b) {
  const IV q = a/b;
  const IV r = a-q*b;
  return q+(r+b)/b-1;
}

static inline IV ceil_div(const IV a, const IV b) {
  return floor_div(a+b-1,b)-1; 
}

static Array<Vector<T,2>,3> sample_mold(RawArray<const TV> path, const T radius, const T dx, const IV sizes, const int samples) {
  // Prepare sample locations
  Array<TV> sample_offsets(samples,false);
  if (samples==1)
    sample_offsets[0] = dx*TV(.5,.5);
  else {
    const auto sobol = new_<Sobol<TV>>(Box<TV>(TV(),TV(1,1)));
    for (auto& x : sample_offsets)
      x = dx*sobol->vector();
  }

  // Render
  Array<Vector<T,2>,3> film(Vector<int,3>(sizes,samples)); // angle,time for each pixel sample
  const Box<IV> ibox(IV(),sizes);
  if (path.size()) {
    ProgressIndicator progress(path.size()-1);
    for (const int n : range(path.size()-1)) {
      progress.progress();
      check_interrupts();
      const TV x0 = path[n],
               x1 = path[n+1],
               x01 = x1-x0;
      const T len = magnitude(x01);
      OTHER_ASSERT(len<100);
      const T angle = other::angle(x01);
      const auto R = Rotation<TV>::from_rotated_vector(TV(1,0),x01);
      const auto box = bounding_box(x0,x1).thickened(radius);
      const Box<IV> ib(IV(floor(box.min)),IV(ceil(box.max)));
      const Box<IV> sbox( ceil_div(ibox.min-ib.max,sizes),
                         floor_div(ibox.max-ib.min,sizes)+1);
      for (const int si : range(sbox.min.x,sbox.max.x))
        for (const int sj : range(sbox.min.y,sbox.max.y)) {
          const IV s = sizes*vec(si,sj);
          const TV sx0 = x0+dx*TV(s);
          const IV slo = ibox.clamp(ib.min+s),
                   shi = ibox.clamp(ib.max+s);
          for (const int i : range(slo.x,shi.x))
            for (const int j : range(slo.y,shi.y)) {
              const TV base = dx*TV(i,j)-sx0;
              for (const int s : range(samples)) {
                const TV X = R.inverse_times(base+sample_offsets[s]);
                if (abs(X.y)<radius) {
                  const T t = X.x+sqrt(sqr(radius)-sqr(X.y));
                  if (0<t && t<len) {
                    const T st = n+t/len;
                    if (film(i,j,s).y < st)
                      film(i,j,s) = vec(angle+T(pi)-asin(X.y/radius),st);
                  }
                }
              }
            }
        }
    }
  }
  return film;
}

}
using namespace mold;

OTHER_PYTHON_MODULE(mold_core) {
  OTHER_FUNCTION(sample_mold)
}
