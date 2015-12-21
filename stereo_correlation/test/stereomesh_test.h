#ifndef STEREOMESH_TEST
#define STEREOMESH_TEST

#ifndef QSTEREOMESHWIDGET_TEST
#define QSTEREOMESHWIDGET_TEST

#include "model/stereomesh.h"
#include "strcon.h"
#include <iostream>
#include <QCoreApplication>

using namespace Stereo;
using namespace std;


class FriendlyStereoMesh : public StereoMesh {
public:

    FriendlyStereoMesh(){}

};

class StereoMeshTest : public ::testing::Test {
 protected:
    FriendlyStereoMesh m_mesh;

    StereoMeshTest() :
        m_widget()
    {
    }

};

TEST_F(StereoMeshTest, LoadsPointCloud) {

    string testfile = StrCon(QCoreApplication::applicationDirPath() + "/../Resources/stereomesh_test/bunny.vdb");
    std::cout << testfile;
}

#endif // QSTEREOMESHWIDGET_TEST



#endif // STEREOMESH_TEST

