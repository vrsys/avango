#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

#include <osg/Geometry>
#include <osg/Geode>

#include <boost/python.hpp>

#include <iostream>

#include <avango/Link.h>
#include <avango/osg/Geode.h>

using namespace boost::python;

namespace boost
 {
  namespace python
   {
    template <class T> struct pointee<av::Link<T> >
     {
      typedef T type;
     };
   }
 }

av::Link<av::osg::Geode> LoadPCDFile(std::string pcd_file)
{
  pcl::PointCloud<pcl::PointXYZRGB> cloud;
  osg::ref_ptr<osg::Geode> geode;

  if (pcl::io::loadPCDFile<pcl::PointXYZRGB>(pcd_file, cloud) == -1) // load the file
  {
    std::cerr << "Couldn't read file " << pcd_file << std::endl;
  } else
  {
    std::cout << "Loaded " << cloud.width * cloud.height
        << " data points from " << pcd_file << std::endl;

    geode = osg::ref_ptr<osg::Geode>(new osg::Geode());
    osg::ref_ptr<osg::Geometry> geometry(new osg::Geometry());

    osg::ref_ptr<osg::Vec3Array> vertices(new osg::Vec3Array());
    osg::ref_ptr<osg::Vec4Array> colors(new osg::Vec4Array());

    colors->push_back(osg::Vec4f(0.8f,0.8f,0.8f,1));

    for (int i = 0; i < cloud.points.size(); i++)
    {
      vertices->push_back(osg::Vec3(cloud.points[i].x, cloud.points[i].y,
          cloud.points[i].z));

      /*
      uint32_t rgb_val_;
      memcpy(&rgb_val_, &(cloud.points[i].rgb), sizeof(uint32_t));

      uint32_t red, green, blue;
      blue = rgb_val_ & 0x000000ff;
      rgb_val_ = rgb_val_ >> 8;
      green = rgb_val_ & 0x000000ff;
      rgb_val_ = rgb_val_ >> 8;
      red = rgb_val_ & 0x000000ff;

      colors->push_back(osg::Vec4f((float) red / 255.0f,
          (float) green / 255.0f, (float) blue / 255.0f, 1.0f));
      */
    }

    geometry->setVertexArray(vertices.get());
    geometry->setColorArray(colors.get());
    //geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    geometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0,
        vertices->size()));

    geode->addDrawable(geometry.get());
    osg::StateSet* state = geometry->getOrCreateStateSet();
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
  }
  return new av::osg::Geode(geode.get());
}


void init_PCD(void)
{
  def("load_pcd_file", LoadPCDFile);
}
