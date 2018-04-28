#include "budextractorruntimetestwidget.h"

#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QFormLayout>
#include <QMessageBox>
#include <QLineEdit>
#include <QSettings>
#include <QDoubleSpinBox>
#include <QComboBox>

#include "util/filedirselectionwidget.h"
#include "extractordialog/extractorconfigdialog.h"
#include "extractordialog/chtextractorconfigdialog.h"
#include "convolutionalsegmenter/convolutionalsegmenterparameterdialog.h"

void BudExtractorRuntimeTestWidget::saveSettings()
{
  QSettings settings;
  //settings.setValue("budextractortest/outputDir",outputDirField->text());
}

void BudExtractorRuntimeTestWidget::loadSettings()
{
  QSettings settings;
  //outputDirField->setText(settings.value("budextractortest/outputDir").toString());
}

BudExtractorRuntimeTestWidget::BudExtractorRuntimeTestWidget(ChtExtractorConfigDialog* chtDialog, ConvolutionalSegmenterParameterDialog* convSegDialog, QString *dataFile, QWidget *parent)
  :QWidget(parent)
{
  repeat = 10;
  this->dataFile = dataFile;
  //mode selection
  configDialog = chtDialog;
  //data selection
  dataLayout = new QFormLayout(this);
  segmentationBox = new QComboBox(this);
  segmentationBox->addItem("Annotated Data",BudExtractorTest::SEGMENTATION_ANNOTATED);
  segmentationBox->addItem("Convolutional Segmenter",BudExtractorTest::SEGMENTATION_CNN);
  segmentationBox->addItem("None",BudExtractorTest::SEGMENTATION_NONE);
  //
  startButton = new QPushButton("Start",this);
  connect(startButton,&QPushButton::clicked,this,&BudExtractorRuntimeTestWidget::startButtonPressed);
  //
  convolutionalSegmenterParameterDialog = convSegDialog;
  //add to main layout
  dataLayout->addRow("Segmentation:",segmentationBox);
  dataLayout->addRow(startButton);
  loadSettings();
}

void BudExtractorRuntimeTestWidget::startButtonPressed()
{
  float toleranceFactor = 1.5;
  budExtractorTest.setPositionToleranceFactor(toleranceFactor);
  budExtractorTest.setCurrentSegmentation((BudExtractorTest::SegmentationOptions)segmentationBox->currentData().toInt());
  budExtractorTest.setConvolutionalSegmenterParameters(convolutionalSegmenterParameterDialog->getParameters());

  if(budExtractorTest.loadInfos(*dataFile))
  {
    BudExtractor& extractor = configDialog->getExtractor();
    saveSettings();
    std::chrono::high_resolution_clock::duration timeTotal(0);
    for(int i=0;i<repeat;i++)
    {
      timeTotal += budExtractorTest.test(extractor);
    }
    long int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(timeTotal/repeat).count();
    QMessageBox::information(this,
                             "Results",
                             "Mean time: "+QString::number(milliseconds/1000.0f)+"s\n"
                             );
  }
  else
  {
    QMessageBox::critical(this,"Loading Error","Error: Could not load data file.");
  }
}
