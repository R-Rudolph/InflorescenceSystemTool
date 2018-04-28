#include "segmentationruntimetester.h"

#include <QFormLayout>
#include "convolutionalsegmenter/convolutionalsegmenterparameterdialog.h"
#include "convolutionalsegmenter/convolutionalsegmenter.h"
#include "imagelib/imagelibrary.h"
#include <QPushButton>
#include <QMessageBox>
#include <chrono>

SegmentationRuntimeTester::SegmentationRuntimeTester(ConvolutionalSegmenterParameterDialog* convSegDialog, QString* dataFile, QWidget *parent) : QWidget(parent)
{
  repeats = 10;
  this->dataFile = dataFile;
  layout = new QFormLayout(this);
  this->convSegDialog = convSegDialog;
  startButton = new QPushButton("Start",this);
  layout->addRow(startButton);
  connect(startButton,&QPushButton::clicked,this,&SegmentationRuntimeTester::start);
}

void SegmentationRuntimeTester::start()
{
  ImageLibrary lib;
  if(lib.load(*dataFile))
  {
    ConvolutionalSegmenter segmenter(convSegDialog->getParameters());
    size_t ctr = 0;
    std::chrono::high_resolution_clock::duration timeTotal = std::chrono::high_resolution_clock::duration(0);
    QList<ImageInfo> infos = lib.getInfos();
    foreach(const ImageInfo& info, infos)
    {
      if(info.isInTestSet())
      {
        cv::Mat image = cv::imread(info.getFilepath().toStdString());
        for(int i=0;i<repeats;i++)
        {
          std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
          segmenter.segment(image,1);
          std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
          timeTotal += end - start;
          ctr++;
        }
      }
    }
    long int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeTotal).count();
    QMessageBox::information(this,"Result","s/image : "+QString::number(milliseconds/(1000.0 * ctr))+"s\n"
                                           "images/s: "+QString::number((1000.0 * ctr)/milliseconds)+"s");
  }
  else
  {
    QMessageBox::warning(this,"Error","Could not load data file.");
  }
}
