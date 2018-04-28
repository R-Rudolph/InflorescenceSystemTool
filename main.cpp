#include "inflorescencesystemwindow.h"
#include <QApplication>
#include <caffe/caffe.hpp>

int main(int argc, char *argv[])
{
  caffe::Caffe::set_mode(caffe::Caffe::CPU);
  QApplication a(argc, argv);
  InflorescenceSystemWindow w;
  w.show();

  return a.exec();
}
