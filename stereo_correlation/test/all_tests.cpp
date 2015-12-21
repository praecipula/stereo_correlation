#include "gtest/gtest.h"
#include <QApplication>
#include "strcon_test.h"
#include "qtdistancefieldfonttexturegenerator_test.h"
#include "qstereomeshwidget_test.h"
#include "stereomesh_test.h"


int main(int argc, char **argv) {
    // Some tests need a running QApplication
    QApplication app(argc, argv);
    QApplication::processEvents(); // this should allow `QApplication` to complete its initialization
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
