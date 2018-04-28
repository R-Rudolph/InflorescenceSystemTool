#include "segmentationtesterwidget.h"

#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include "convolutionalsegmenter/convolutionalsegmenterparameterdialog.h"
#include "convolutionalsegmenter/convolutionalsegmenter.h"
#include "imagelib/imagelibrary.h"
#include "confusionmatrix.h"
#include <opencv2/imgproc.hpp>

SegmentationTesterWidget::SegmentationTesterWidget(ConvolutionalSegmenterParameterDialog* convSegDialog, QString* dataFile, QWidget *parent) : QWidget(parent)
{
  this->dataFile = dataFile;
  layout = new QFormLayout(this);
  startButton = new QPushButton("Start",this);
  convDialog = convSegDialog;
  layout->addRow(startButton);

  connect(startButton,&QPushButton::clicked,this,&SegmentationTesterWidget::start);
}

void SegmentationTesterWidget::start()
{
  ImageLibrary lib;
  if(lib.load(*dataFile))
  {
    QList<ImageInfo> list = lib.getInfos();
    ConvolutionalSegmenter convSeg(convDialog->getParameters());
    ConfusionMatrix cm(2);
    QString error;
    foreach(const ImageInfo& info, list)
    {
      if(info.isInTestSet())
      {
        cv::Mat image       = info.getImage();
        if(image.data==nullptr)
        {
          error = info.getFilepath();
          break;
        }
        cv::Mat groundTruth = info.getMap() / 255;
        cv::Mat segmentation = convSeg.segment(image,1);
        segmentation.convertTo(segmentation,CV_8U,255);
        cv::threshold(segmentation,segmentation,128,1,cv::THRESH_BINARY);
        cm.addImage(groundTruth,segmentation);
      }
    }
    if(error.isEmpty())
    {
      QMessageBox::information(this,"Results","mean IOU: "+QString::number(cm.meanJaccardIndex())+"\n"
                                              "IOU(inflorescence): "+QString::number(cm.jaccardIndex(1))+"\n"
                                              "IOU(non-inflorescence): "+QString::number(cm.jaccardIndex(0)));
    }
    else
    {
      QMessageBox::warning(this,"Error","Could not load image file: "+error+".");
    }
  }
  else
  {
    QMessageBox::warning(this,"Error","Could not load data file: "+*dataFile+".");
  }
}
