#include "inflorescencesystemwindow.h"

#include <QTabWidget>
#include <QMenuBar>
#include <QFileDialog>
#include <QSettings>
#include "budextractorwidget.h"
#include "budextractortestwidget.h"
#include "parametertesterwidget.h"
#include "segmentationruntimetester.h"
#include "budextractorruntimetestwidget.h"
#include "segmentationtesterwidget.h"

#include "extractordialog/chtextractorconfigdialog.h"
#include "convolutionalsegmenter/convolutionalsegmenterparameterdialog.h"


QString InflorescenceSystemWindow::getDataFile() const
{
  return dataFile;
}

void InflorescenceSystemWindow::setDataFile(const QString &value)
{
  dataFile = value;
  updateTitle();
}

void InflorescenceSystemWindow::updateTitle()
{
  QString title;
  if(dataFile.isEmpty())
    title = "[NO FILE SELECTED] ";
  else
    title = "["+dataFile.split('/').last()+"] ";
  setWindowTitle(title + "InflorescenceSystemTool");
}

void InflorescenceSystemWindow::loadSettings()
{
  QSettings settings;
  setDataFile(settings.value("datafile","").toString());
}

void InflorescenceSystemWindow::saveSettings()
{
  QSettings settings;
  settings.setValue("datafile",dataFile);
}

InflorescenceSystemWindow::InflorescenceSystemWindow(QWidget *parent)
  : QMainWindow(parent)
{
  convSegDialog = new ConvolutionalSegmenterParameterDialog("convseg-dialog/",this);
  extractorDialog = new ChtExtractorConfigDialog("cht-dialog/",this);

  tabs = new QTabWidget(this);
  performanceTabs = new QTabWidget(this);
  runtimeTabs = new QTabWidget(this);
  extraction = new BudExtractorWidget(extractorDialog,convSegDialog,this);
  budExtractorTest = new BudExtractorTestWidget(extractorDialog,convSegDialog,&dataFile,this);
  segmentationTest = new SegmentationTesterWidget(convSegDialog,&dataFile,this);
  parameterTest = new ParameterTesterWidget(&dataFile,this);
  segmentationRuntimeTester = new SegmentationRuntimeTester(convSegDialog,&dataFile,this);
  budExtractorRuntimeTestWidget = new BudExtractorRuntimeTestWidget(extractorDialog,convSegDialog,&dataFile,this);
  tabs->addTab(extraction,"Usage");
  tabs->addTab(performanceTabs,"Performance Tests");
  tabs->addTab(runtimeTabs,"Runtime Tests");
  tabs->addTab(parameterTest,"Parameter Finder");
  //usageTabs->addTab(budextraktion,"Extraktion");
  runtimeTabs->addTab(segmentationRuntimeTester,"Segmentation");
  runtimeTabs->addTab(budExtractorRuntimeTestWidget,"Extraction");
  performanceTabs->addTab(segmentationTest,"Segmentation");
  performanceTabs->addTab(budExtractorTest,"Extraction");
  setCentralWidget(tabs);
  fileMenu = menuBar()->addMenu("File");
  fileMenu->addAction("Select Annotation File",this,&InflorescenceSystemWindow::selectDataFile);
  fileMenu->addSeparator();
  fileMenu->addAction("Exit",this,&InflorescenceSystemWindow::close);
  configMenu = menuBar()->addMenu("Config");
  configMenu->addAction("Circular Hough Transform",extractorDialog,&ChtExtractorConfigDialog::exec);
  configMenu->addAction("Convolutional Segmenter",convSegDialog,&ConvolutionalSegmenterParameterDialog::exec);
  loadSettings();
}

InflorescenceSystemWindow::~InflorescenceSystemWindow()
{

}

void InflorescenceSystemWindow::selectDataFile()
{
  QString newDataFile = QFileDialog::getOpenFileName(nullptr,"Select Data File",dataFile);
  if(!newDataFile.isEmpty())
  {
    setDataFile(newDataFile);
    saveSettings();
  }
}
