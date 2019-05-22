/************************************************************************\
*                                                                        *
* This file is part of Avango.                                           *
*                                                                        *
* Copyright 1997 - 2010 Fraunhofer-Gesellschaft zur Foerderung der       *
* angewandten Forschung (FhG), Munich, Germany.                          *
*                                                                        *
* Avango is free software: you can redistribute it and/or modify         *
* it under the terms of the GNU Lesser General Public License as         *
* published by the Free Software Foundation, version 3.                  *
*                                                                        *
* Avango is distributed in the hope that it will be useful,              *
* but WITHOUT ANY WARRANTY; without even the implied warranty of         *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the           *
* GNU General Public License for more details.                           *
*                                                                        *
* You should have received a copy of the GNU Lesser General Public       *
* License along with Avango. If not, see <http://www.gnu.org/licenses/>. *
*                                                                        *
* Avango is a trademark owned by FhG.                                    *
*                                                                        *
\************************************************************************/

#include <avango/utils/PCLPointCloud.h>
#include <avango/Logger.h>
#include <cmath>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

namespace
{
av::Logger& logger(av::getLogger("av::utils::PCLPointCloud"));
}

AV_FC_DEFINE(av::utils::PCLPointCloud);

AV_FIELD_DEFINE(av::utils::SFPCLPointCloud);
AV_FIELD_DEFINE(av::utils::MFPCLPointCloud);

av::utils::PCLPointCloud::PCLPointCloud()
{
    AV_FC_ADD_FIELD(Filename, "");
    AV_FC_ADD_FIELD(Points, av::osg::MFVec3::ContainerType());
    AV_FC_ADD_FIELD(Colors, av::osg::MFVec4::ContainerType());

    Name.setValue("PCLPointCloud");
}

av::utils::PCLPointCloud::~PCLPointCloud() {}

void av::utils::PCLPointCloud::initClass()
{
    if(!isTypeInitialized())
    {
        av::FieldContainer::initClass();

        AV_FC_INIT(av::FieldContainer, av::utils::PCLPointCloud, true);

        SFPCLPointCloud::initClass("av::utils::SFPCLPointCloud", "av::Field");
        MFPCLPointCloud::initClass("av::utils::MFPCLPointCloud", "av::Field");
    }
}

/* virtual */ void av::utils::PCLPointCloud::fieldHasChanged(const av::Field& field)
{
    av::FieldContainer::fieldHasChanged(field);

    if(&field == &Filename)
    {
        loadPCDFile(Filename.getValue());
    }
}

/* virtual */ void av::utils::PCLPointCloud::evaluate() { av::FieldContainer::evaluate(); }

void av::utils::PCLPointCloud::loadPCDFile(std::string filename)
{
    pcl::PointCloud<pcl::PointXYZRGB> cloud;

    if(pcl::io::loadPCDFile<pcl::PointXYZRGB>(filename, cloud) == -1) // load the file
    {
        std::cerr << "Couldn't read file " << filename << std::endl;
    }
    else
    {
        std::cout << "Loaded " << cloud.width * cloud.height << " data points from " << filename << std::endl;

        std::vector<::osg::Vec3f> points;
        std::vector<::osg::Vec4f> colors;

        for(int i = 0; i < cloud.points.size(); i++)
        {
            points.push_back(::osg::Vec3f(cloud.points[i].x, cloud.points[i].y, cloud.points[i].z));

            uint32_t rgb_val_;
            memcpy(&rgb_val_, &(cloud.points[i].rgb), sizeof(uint32_t));

            uint32_t red, green, blue;
            blue = rgb_val_ & 0x000000ff;
            rgb_val_ = rgb_val_ >> 8;
            green = rgb_val_ & 0x000000ff;
            rgb_val_ = rgb_val_ >> 8;
            red = rgb_val_ & 0x000000ff;

            colors.push_back(::osg::Vec4f((float)red / 255.0f, (float)green / 255.0f, (float)blue / 255.0f, 1.0f));
        }
        Points.setValue(points);
        Colors.setValue(colors);
    }
}
