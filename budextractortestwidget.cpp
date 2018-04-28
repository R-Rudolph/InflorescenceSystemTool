#include "budextractortestwidget.h"

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
#include "extractordialog/frstextractorconfigdialog.h"
#include "convolutionalsegmenter/convolutionalsegmenterparameterdialog.h"

void BudExtractorTestWidget::addExtractor(ExtractorConfigDialog *dialog)
{
  dialog->setParent(this);
  QPushButton* configButton = new QPushButton("Config",this);
  QRadioButton* radioButton = new QRadioButton(dialog->name()+":",this);
  int row = modeLayout->rowCount();
  modeLayout->addWidget(radioButton,row,0,1,1,Qt::AlignLeft);
  modeLayout->addWidget(configButton,row,1,1,1,Qt::AlignRight);
  modeGroup->addButton(radioButton);
  radioExtractorMap.insert(radioButton,dialog);
  configExtractorMap.insert(configButton,dialog);
  connect(configButton,&QPushButton::clicked,this,&BudExtractorTestWidget::configButtonPressed);
}

BudExtractor* BudExtractorTestWidget::getCurrentBudExtractor()
{
  /*
  QRadioButton* button = static_cast<QRadioButton*>(modeGroup->checkedButton());
  if(button==nullptr)
    return nullptr;
  return &(radioExtractorMap[button]->getExtractor());
  */
  return &chtDialog->getExtractor();
}

void BudExtractorTestWidget::saveSettings()
{
  QSettings settings;
  settings.setValue("budextractortest/outputDir",outputDirField->text());
}

void BudExtractorTestWidget::loadSettings()
{
  QSettings settings;
  outputDirField->setText(settings.value("budextractortest/outputDir").toString());
}

BudExtractorTestWidget::BudExtractorTestWidget(ChtExtractorConfigDialog* chtDialog, ConvolutionalSegmenterParameterDialog* convSegDialog, QString *dataFile, QWidget *parent)
  :QWidget(parent)
{
  mainLayout = new QVBoxLayout(this);
  this->dataFile = dataFile;
  //mode selection
  /*
  modeLayout = new QGridLayout();
  modeGroup = new QButtonGroup(this);
  addExtractor(new ChtExtractorConfigDialog("cht/",this));
  addExtractor(new FrstExtractorConfigDialog(this));
  */
  this->chtDialog = chtDialog;
  //data selection
  dataLayout = new QFormLayout();
  outputDirField = new FileDirSelectionWidget(this,QFileDialog::AcceptOpen,FileDirSelectionWidget::Dir);
  dataLayout->addRow("Output Directory:",outputDirField);
  positionToleranceBox = new QDoubleSpinBox(this);
  positionToleranceBox->setValue(budExtractorTest.getPositionToleranceFactor());
  dataLayout->addRow("Position Tolerance Factor:",positionToleranceBox);
  segmentationBox = new QComboBox(this);
  segmentationBox->addItem("Annotated Data",BudExtractorTest::SEGMENTATION_ANNOTATED);
  segmentationBox->addItem("Convolutional Segmenter",BudExtractorTest::SEGMENTATION_CNN);
  segmentationBox->addItem("None",BudExtractorTest::SEGMENTATION_NONE);
  dataLayout->addRow("Segmentation:",segmentationBox);
  //
  testButton = new QPushButton("Test",this);
  connect(testButton,&QPushButton::clicked,this,&BudExtractorTestWidget::testButtonPressed);
  //
  convolutionalSegmenterParameterDialog = convSegDialog;
  //add to main layout
  //mainLayout->addLayout(modeLayout);
  mainLayout->addLayout(dataLayout);
  mainLayout->addWidget(testButton);
  mainLayout->addWidget(new QWidget(this),1);
  loadSettings();
}

void BudExtractorTestWidget::configButtonPressed()
{
  QPushButton* button = static_cast<QPushButton*>(QObject::sender());
  if(configExtractorMap.contains(button))
  {
    ExtractorConfigDialog* dialog = configExtractorMap[button];
    dialog->exec();
  }
}

void BudExtractorTestWidget::testButtonPressed()
{
  float toleranceFactor = positionToleranceBox->value();
  budExtractorTest.setPositionToleranceFactor(toleranceFactor);
  budExtractorTest.setCurrentSegmentation((BudExtractorTest::SegmentationOptions)segmentationBox->currentData().toInt());
  budExtractorTest.setConvolutionalSegmenterParameters(convolutionalSegmenterParameterDialog->getParameters());

  if(budExtractorTest.loadInfos(*dataFile))
  {
    BudExtractor* extractor = getCurrentBudExtractor();
    if(extractor==nullptr)
    {
      QMessageBox::critical(this,"Error","Error: No extractor selected.");
      return;
    }
    saveSettings();
    std::chrono::high_resolution_clock::duration time = budExtractorTest.test(*extractor,outputDirField->text());
    long int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(time).count();
    QMessageBox::information(this,
                             "Results",
                             "f1: "+QString::number(budExtractorTest.f1Score())+"\n"
                             "recall: "+QString::number(budExtractorTest.recall())+"\n"
                             "precision: "+QString::number(budExtractorTest.precision())+"\n"
                             "found: "+QString::number(budExtractorTest.meanDeviation())+"\n"
                             "sigma: "+QString::number(budExtractorTest.getDeviationStdDeviationValue())+"\n"
                             "Time (extraction): "+QString::number(milliseconds/1000.0)+"s\n"
                             );
  }
  else
  {
    QMessageBox::critical(this,"Loading Error","Error: Could not load data file.");
  }
}
