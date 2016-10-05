#ifndef STEREOMESH_TEST
#define STEREOMESH_TEST


#include "model/pointcloud.h"
#include "strcon.h"
#include <iostream>
#include <QCoreApplication>
#include <QFile>
#include <openvdb/io/File.h>
#include <openvdb/Grid.h>
#include <openvdb/tools/VolumeToMesh.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace Stereo;
using namespace std;


class FriendlyPointCloud : public PointCloud {
public:

    FriendlyPointCloud(){}

};

class StereoMeshTest : public ::testing::Test {
 protected:
    FriendlyPointCloud m_cloud;

    StereoMeshTest() :
        m_cloud()
    {
    }

};

TEST_F(StereoMeshTest, LoadsPointCloud) {

    string testfile = StrCon(QCoreApplication::applicationDirPath() + "/../Resources/stereomesh_test/bunny.vdb").to_s();
    QFile f(StrCon(testfile).to_qs());
    ASSERT_TRUE(f.exists());
    openvdb::io::File ovdbFile(testfile);
    ovdbFile.open();
    openvdb::GridPtrVecPtr gridVec = ovdbFile.getGrids();
    openvdb::FloatGrid::Ptr firstGrid(openvdb::gridPtrCast<openvdb::FloatGrid>(gridVec->at(0)));
    firstGrid->print(std::cerr);
    openvdb::tools::VolumeToMesh converter;
    converter(*firstGrid);
    std::cerr << "Converted";
    cv::Mat imageMe = cv::imread("frog.jpg", 0);
}


#endif // STEREOMESH_TEST

