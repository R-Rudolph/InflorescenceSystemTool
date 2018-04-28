#include "chtextractorconfigdialog.h"

#include <QFormLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QSettings>
#include <QDoubleSpinBox>

const QString ChtExtractorConfigDialog::extractorName = "Circular Hough Transform";

bool ChtExtractorConfigDialog::extractorToFields()
{
  gaussianRadiusLcnInputBox->setValue(extractor.getLcnInputGaussRadius());
  gaussianRadiusLcnBox->setValue(extractor.getLcnGaussRadius());
  gaussianRadiusLcnOutputBox->setValue(extractor.getLcnOutputGaussRadius());
  cannyThresholdLowerBox->setValue(extractor.getLowerThreshold());
  chtThresholdBox->setValue(extractor.getChtThreshold());
  adjacentBlockingFactorBox->setValue(extractor.getAdjacentBlockingFactor());
  return true;
}

bool ChtExtractorConfigDialog::fieldsToExtractor()
{
  int gaussRadiusInput = gaussianRadiusLcnInputBox->value();
  int gaussRadiusLcn = gaussianRadiusLcnBox->value();
  int gaussRadiusOutput = gaussianRadiusLcnOutputBox->value();
  int cannyThresholdLower = cannyThresholdLowerBox->value();
  int cannyThresholdUpper = cannyThresholdLower*2;
  float chtThreshold = chtThresholdBox->value();
  float adjacentBlockingFactor = adjacentBlockingFactorBox->value();
  extractor.setLcnInputGaussRadius(gaussRadiusInput);
  extractor.setLcnGaussRadius(gaussRadiusLcn);
  extractor.setLcnOutputGaussRadius(gaussRadiusOutput);
  extractor.setLowerThreshold(cannyThresholdLower);
  extractor.setUpperThreshold(cannyThresholdUpper);
  extractor.setAdjacentBlockingFactor(adjacentBlockingFactor);
  extractor.setChtThreshold(chtThreshold);
  return true;
}

void ChtExtractorConfigDialog::saveToSettings()
{
  QSettings settings;
  settings.setValue(getSettingsPrefix()+"gaussRadiusInput",extractor.getLcnInputGaussRadius());
  settings.setValue(getSettingsPrefix()+"gaussRadiusLcn",extractor.getLcnGaussRadius());
  settings.setValue(getSettingsPrefix()+"gaussRadiusOutput",extractor.getLcnOutputGaussRadius());
  settings.setValue(getSettingsPrefix()+"cannyThresholdLower",extractor.getLowerThreshold());
  settings.setValue(getSettingsPrefix()+"cannyThresholdUpper",extractor.getUpperThreshold());
  settings.setValue(getSettingsPrefix()+"chtThreshold",extractor.getChtThreshold());
  settings.setValue(getSettingsPrefix()+"adjacentBlocking",extractor.getAdjacentBlockingFactor());
}

void ChtExtractorConfigDialog::loadFromSettings()
{
  QSettings settings;
  setValue(extractor.getLcnInputGaussRadiusRef(),settings.value(getSettingsPrefix()+"gaussRadiusInput"));
  setValue(extractor.getLcnGaussRadiusRef(),settings.value(getSettingsPrefix()+"gaussRadiusLcn"));
  setValue(extractor.getLcnOutputGaussRadiusRef(),settings.value(getSettingsPrefix()+"gaussRadiusOutput"));
  setValue(extractor.getLowerThresholdRef(),settings.value(getSettingsPrefix()+"cannyThresholdLower"));
  setValue(extractor.getUpperThresholdRef(),settings.value(getSettingsPrefix()+"cannyThresholdUpper"));
  setValue(extractor.getChtThresholdRef(),settings.value(getSettingsPrefix()+"chtThreshold"));
  setValue(extractor.getAdjacentBlockingFactorRef(),settings.value(getSettingsPrefix()+"adjacentBlocking"));
}

ChtExtractorConfigDialog::ChtExtractorConfigDialog(QWidget* parent, Qt::WindowFlags f)
  : ExtractorConfigDialog(parent, f)
{
  setWindowTitle(this->name());
  layout = new QFormLayout(this);
  //canny
  gaussianRadiusLcnInputBox = new QSpinBox(this);
  gaussianRadiusLcnInputBox->setMinimum(0);
  gaussianRadiusLcnInputBox->setMaximum(1000);
  gaussianRadiusLcnBox = new QSpinBox(this);
  gaussianRadiusLcnBox->setMinimum(0);
  gaussianRadiusLcnBox->setMaximum(1000);
  gaussianRadiusLcnOutputBox = new QSpinBox(this);
  gaussianRadiusLcnOutputBox->setMinimum(0);
  gaussianRadiusLcnOutputBox->setMaximum(1000);

  cannyThresholdLowerBox = new QSpinBox(this);
  cannyThresholdLowerBox->setMinimum(0);
  cannyThresholdLowerBox->setMaximum(1000);
  //cht
  chtThresholdBox = new QDoubleSpinBox(this);
  chtThresholdBox->setMinimum(0);
  chtThresholdBox->setMaximum(1);
  chtThresholdBox->setSingleStep(0.01);
  adjacentBlockingFactorBox = new QDoubleSpinBox(this);
  adjacentBlockingFactorBox->setMinimum(0);
  adjacentBlockingFactorBox->setMaximum(5);
  adjacentBlockingFactorBox->setSingleStep(0.1);
  adjacentBlockingFactorBox->setValue(1.5);
  //layout
  layout->addRow("Gaussian Radius (LCN-Input)",gaussianRadiusLcnInputBox);
  layout->addRow("Gaussian Radius (LCN)",gaussianRadiusLcnBox);
  layout->addRow("Gaussian Radius (LCN-Output)",gaussianRadiusLcnOutputBox);
  layout->addRow("Canny Threshold (Lower)",cannyThresholdLowerBox);
  layout->addRow("CHT Threshold",chtThresholdBox);
  layout->addRow("Adjacent Blocking Factor",adjacentBlockingFactorBox);
  layout->addRow(buttonLayout);
  //
  extractorToFields();
}

ChtExtractorConfigDialog::ChtExtractorConfigDialog(const QString &settingsPrefix, QWidget *parent, Qt::WindowFlags f)
  : ChtExtractorConfigDialog(parent,f)
{
  setSettingsPrefix(settingsPrefix);
  loadFromSettings();
  extractorToFields();
}

const QString &ChtExtractorConfigDialog::name()
{
  return extractorName;
}

BudExtractor &ChtExtractorConfigDialog::getExtractor()
{
  return extractor;
}
