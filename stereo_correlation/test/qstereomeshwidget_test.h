#ifndef QSTEREOMESHWIDGET_TEST
#define QSTEREOMESHWIDGET_TEST

#include "widgets/qstereomeshwidget.h"

using namespace Stereo;
using namespace std;


class FriendlyQStereoMeshWidget : public QStereoMeshWidget {
public:

    FriendlyQStereoMeshWidget(){}

};

class QStereoMeshWidgetTest : public ::testing::Test {
 protected:
    FriendlyQStereoMeshWidget m_widget;

    QStereoMeshWidgetTest() :
        m_widget()
    {
    }

};

TEST_F(QStereoMeshWidgetTest, LoadsPoints) {


}

#endif // QSTEREOMESHWIDGET_TEST

