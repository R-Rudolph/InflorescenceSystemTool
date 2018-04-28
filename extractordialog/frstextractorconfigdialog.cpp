#include "frstextractorconfigdialog.h"
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QFormLayout>

const QString FrstExtractorConfigDialog::extractorName = "Fast Radial Symmetry Transform";

bool FrstExtractorConfigDialog::extractorToFields()
{
  gaussianPreRadiusField->setValue(extractor.getGaussianPreSize());
  gaussianPostStdDevField->setValue(extractor.getGaussianPostStdDev());
  radialStrictnessField->setValue(extractor.getRadialStrictness());
  gradientThresholdField->setValue(extractor.getGradientThreshold());
  rangeMinField->setValue(extractor.getRangeMin());
  rangeMaxField->setValue(extractor.getRangeMax());
  return true;
}

bool FrstExtractorConfigDialog::fieldsToExtractor()
{
  extractor.setGaussianPreSize(gaussianPreRadiusField->value());
  extractor.setGaussianPostStdDev(gaussianPostStdDevField->value());
  extractor.setRadialStrictness(radialStrictnessField->value());
  extractor.setGradientThreshold(gradientThresholdField->value());
  int min = rangeMinField->value();
  int max = rangeMaxField->value();
  if(min>max)
  {
    setError("Minimal range is greater than maximal range.");
    return false;
  }
  extractor.setRanges(min,max);
  return true;
}

void FrstExtractorConfigDialog::saveToSettings()
{

}

void FrstExtractorConfigDialog::loadFromSettings()
{

}

FrstExtractorConfigDialog::FrstExtractorConfigDialog(QWidget *parent, Qt::WindowFlags f)
  : ExtractorConfigDialog(parent,f)
{
  //fields
  gaussianPreRadiusField = new QSpinBox(this);
  gaussianPreRadiusField->setMinimum(1);
  gaussianPostStdDevField = new QDoubleSpinBox(this);
  gaussianPostStdDevField->setMinimum(0);
  gaussianPostStdDevField->setSingleStep(0.1);
  radialStrictnessField = new QDoubleSpinBox(this);
  radialStrictnessField->setMinimum(1);
  radialStrictnessField->setSingleStep(0.1);
  gradientThresholdField = new QDoubleSpinBox(this);
  gradientThresholdField->setMinimum(0);
  gradientThresholdField->setMaximum(1);
  gradientThresholdField->setSingleStep(0.01);
  rangeMinField = new QSpinBox(this);
  rangeMinField->setMinimum(1);
  rangeMaxField = new QSpinBox(this);
  rangeMaxField->setMinimum(1);
  //layout
  layout = new QFormLayout(this);
  layout->addRow("Gaussian Radius (On Input)",gaussianPreRadiusField);
  layout->addRow("Gaussian StdDev (On S)",gaussianPostStdDevField);
  layout->addRow("Radial Strictness",radialStrictnessField);
  layout->addRow("Gradient Threshold",gradientThresholdField);
  layout->addRow("Range (Min)",rangeMinField);
  layout->addRow("Range (Max)",rangeMaxField);
  layout->addRow(buttonLayout);
  extractorToFields();
}

BudExtractor &FrstExtractorConfigDialog::getExtractor()
{
  return extractor;
}

const QString &FrstExtractorConfigDialog::name()
{
  return extractorName;
}
