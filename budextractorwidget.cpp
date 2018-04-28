#include "budextractorwidget.h"

#include <opencv2/imgproc.hpp>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QComboBox>
#include <QCheckBox>
#include "util/filedirselectionwidget.h"

#include "extractordialog/chtextractorconfigdialog.h"
#include "convolutionalsegmenter/convolutionalsegmenterparameterdialog.h"
#include "convolutionalsegmenter/convolutionalsegmenter.h"
#include "inflorescencecollection.h"

cv::Scalar BudExtractorWidget::getCircleColor()
{
  return cv::Scalar(0,0,255);
}

bool BudExtractorWidget::processImage(const QString &filepath, const QDir &outputDir, ConvolutionalSegmenter &segmenter, QTextStream& outputFile, QTextStream& outputFileInflorescences)
{
  cv::Mat image = cv::imread(filepath.toStdString());
  QString filename = filepath.split('/').last();
  QStringList splitFilename = filename.split('.');
  splitFilename[0] += "-segmentation";
  QString filenameSegmentation = splitFilename.join('.');
  splitFilename = filename.split('.');
  splitFilename[0] += "-flowers";
  QString filenameBuds = splitFilename.join('.');
  if(image.data==nullptr)
  {
    return false;
  }
  cv::Mat segmentation = segmenter.segment(image,1);
  segmentation.convertTo(segmentation,CV_8U,255);

  cv::Mat thresholdedSegmentation;
  cv::threshold(segmentation,thresholdedSegmentation,128,255,cv::THRESH_BINARY);

  if(saveSegmentationBox->isChecked())
  {
    cv::Mat image2 = image;
    OutputMode currentMode = (OutputMode) outputModeField->currentData().toInt();
    switch(currentMode)
    {
      case OUTPUT_SEGMENTATION:
      {
        cv::imwrite(outputDir.absoluteFilePath(filenameSegmentation).toStdString(),segmentation);
        break;
      }
      case OUTPUT_HEATMAP:
      {
        cv::cvtColor(image2,image2,CV_BGR2GRAY);
        cv::cvtColor(image2,image2,CV_GRAY2BGR);
        cv::applyColorMap(segmentation,segmentation,cv::COLORMAP_JET);
        cv::imwrite(outputDir.absoluteFilePath(filenameSegmentation).toStdString(),segmentation/2 + image2/2);
        break;
      }
      case OUTPUT_BRIGHTEN:
      {
        cv::cvtColor(segmentation,segmentation,CV_GRAY2BGR);
        cv::imwrite(outputDir.absoluteFilePath(filenameSegmentation).toStdString(),segmentation/2 + image2/2);
        break;
      }
    }
  }

  InflorescenceCollection collection(thresholdedSegmentation);

  std::vector<Circle> circles = extractorDialog->getExtractor().extractBuds(image,thresholdedSegmentation);
  collection.addContent(circles);
  for(size_t i=0;i<circles.size();i++)
  {
    cv::circle(image,circles[i].center,circles[i].radius,getCircleColor(),1);
  }
  outputFile << filename << " " << circles.size() << "\n";
  for(int i=0;i<collection.regionsCount();i++)
  {
    if(collection.regionContent(i)>0)
      outputFileInflorescences << filename << " " << i << " " << collection.regionSize(i) << " " << collection.regionContent(i) << "\n";
  }
  if(saveBudsBox->isChecked())
  {
    cv::imwrite(outputDir.absoluteFilePath(filenameBuds).toStdString(),image);
  }

  return true;
}

void BudExtractorWidget::processImages(const QStringList &fileList, const QDir& outputDir)
{
  ConvolutionalSegmenter segmenter(segmenterDialog->getParameters());
  QStringList failedFiles;
  QFile outputFile(outputDir.absoluteFilePath("results"));
  QFile outputFileInflorescences(outputDir.absoluteFilePath("inflorescences"));
  QTextStream ts(&outputFile);
  QTextStream tsInflorescence(&outputFileInflorescences);
  outputFile.open(QIODevice::WriteOnly);
  outputFileInflorescences.open(QIODevice::WriteOnly);
  tsInflorescence << "# filename regionID regionSize regionContent\n";
  foreach(const QString& filepath, fileList)
  {
    if(!processImage(filepath,outputDir,segmenter,ts,tsInflorescence))
    {
      failedFiles.append(filepath);
    }
  }
  outputFile.close();
  outputFileInflorescences.close();
  if(failedFiles.isEmpty())
  {
    QMessageBox::information(this,"Done","Segmentation concluded without errors.");
  }
  else if(failedFiles.size() == fileList.size())
  {
    QMessageBox::warning(this,"Error","None of the selected files could be loaded.");
  }
  else
  {
    QMessageBox::information(this,"Done","Segmentation concluded. The following files could not be loaded:\n" + failedFiles.join('\n'));
  }
}

BudExtractorWidget::BudExtractorWidget(ChtExtractorConfigDialog* extractorDialog, ConvolutionalSegmenterParameterDialog* segmenterDialog, QWidget *parent) : QWidget(parent)
{
  this->extractorDialog = extractorDialog;
  this->segmenterDialog = segmenterDialog;

  layout = new QFormLayout(this);
  outputDirField = new FileDirSelectionWidget(this,QFileDialog::AcceptOpen,FileDirSelectionWidget::Dir);
  outputModeField = new QComboBox(this);
  outputModeField->addItem("Heatmap",OUTPUT_HEATMAP);
  outputModeField->addItem("Segmentation",OUTPUT_SEGMENTATION);
  outputModeField->addItem("Brighten",OUTPUT_BRIGHTEN);
  saveSegmentationBox = new QCheckBox(" ",this);
  saveSegmentationBox->setChecked(true);
  saveBudsBox = new QCheckBox(" ",this);
  saveBudsBox->setChecked(true);
  startButton = new QPushButton("Start",this);

  layout->addRow("Output Directory:",outputDirField);
  layout->addRow("Segmentation Output:",outputModeField);
  layout->addRow("Save Segmentation Image:",saveSegmentationBox);
  layout->addRow("Save Bud Image:",saveBudsBox);
  layout->addRow(startButton);
  connect(startButton,&QPushButton::clicked,this,&BudExtractorWidget::start);
}

void BudExtractorWidget::start()
{
  QDir outputDir(outputDirField->text());
  if(!outputDir.exists())
  {
    QMessageBox::warning(this,"Error","Output directory does not exist.");
    return;
  }
  QStringList list = QFileDialog::getOpenFileNames(nullptr,"Select Image Files");
  if(!list.isEmpty())
    processImages(list,outputDir);
}
