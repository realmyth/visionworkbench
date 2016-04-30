// __BEGIN_LICENSE__
//  Copyright (c) 2006-2013, United States Government as represented by the
//  Administrator of the National Aeronautics and Space Administration. All
//  rights reserved.
//
//  The NASA Vision Workbench is licensed under the Apache License,
//  Version 2.0 (the "License"); you may not use this file except in
//  compliance with the License. You may obtain a copy of the License at
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// __END_LICENSE__


#include <vw/Cartography/GeoReferenceUtils.h>
#include <vw/Cartography/GeoTransform.h>

//#include <vw/Math/BresenhamLine.h>
//#include <vw/Cartography/GeoReferenceResourcePDS.h>
//#include <vw/FileIO/DiskImageResourcePDS.h>

// Boost
//#include <boost/algorithm/string.hpp>
//#include <boost/foreach.hpp>

namespace vw {
namespace cartography {


GeoReference crop( GeoReference const& input,
                   double upper_left_x, double upper_left_y,
                   double /*width*/, double /*height*/ ) {
  Vector2 top_left_ll;
  if ( input.pixel_interpretation() == GeoReference::PixelAsArea ) {
    top_left_ll = input.pixel_to_point( Vector2(upper_left_x, upper_left_y ) - Vector2(0.5,0.5) );
  } else {
    top_left_ll = input.pixel_to_point( Vector2(upper_left_x, upper_left_y ) );
  }
  GeoReference output = input;      // Start with copy of current transform
  Matrix3x3 T = output.transform();
  T(0,2) = top_left_ll[0];          // Shift the translation to the crop region
  T(1,2) = top_left_ll[1];          //  (don't need to worry about width/height)
  output.set_transform(T);
  return output;
}

GeoReference crop( GeoReference const& input,
                   BBox2 const& bbox ) {
  // Redirect to the other georeference crop call
  return crop(input, bbox.min().x(), bbox.min().y(),
              bbox.width(), bbox.height());
}

GeoReference resample( GeoReference const& input, double scale_x, double scale_y ) {
  GeoReference output = input;
  Matrix3x3 T = output.transform();
  T(0,0) /= scale_x;
  T(1,1) /= scale_y;
  if ( input.pixel_interpretation() == GeoReference::PixelAsArea ) {
    Vector2 top_left_ll = input.pixel_to_point( -Vector2(0.5 / scale_x, 0.5 / scale_y) );
    T(0,2) = top_left_ll[0];
    T(1,2) = top_left_ll[1];
  }
  output.set_transform(T);
  return output;
}

GeoReference resample( GeoReference const& input, double scale ) {
  return resample(input, scale, scale );
}


}} // vw::cartography

#undef CHECK_PROJ_ERROR
