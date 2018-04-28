#include "convolutionalsegmenterparameterdialog.h"
#include <QSettings>
#include <QSpinBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <limits>
#include "util/filedirselectionwidget.h"

QString ConvolutionalSegmenterParameterDialog::getSettingsPrefix() const
{
  return settingsPrefix;
}

void ConvolutionalSegmenterParameterDialog::setSettingsPrefix(const QString &value)
{
  settingsPrefix = value;
}

void ConvolutionalSegmenterParameterDialog::ok()
{
  saveSettings();
  QDialog::accept();
}

void ConvolutionalSegmenterParameterDialog::cancel()
{
  loadSettings();
  QDialog::reject();
}

void ConvolutionalSegmenterParameterDialog::saveSettings()
{
  if(!settingsPrefix.isEmpty())
  {
    QSettings settings;
    settings.setValue(settingsPrefix+"cropsize",cropSizeField->value());
    settings.setValue(settingsPrefix+"prototxt",prototxtField->text());
    settings.setValue(settingsPrefix+"caffemodel",caffemodelField->text());
  }
  else
  {
    previousParameters = getParameters();
  }
}

void ConvolutionalSegmenterParameterDialog::loadSettings()
{
  if(!settingsPrefix.isEmpty())
  {
    QSettings settings;
    bool ok;
    int cropSize = settings.value(settingsPrefix+"cropsize",0).toInt(&ok);
    if(ok)
      cropSizeField->setValue(cropSize);
    prototxtField->setText(settings.value(settingsPrefix+"prototxt","").toString());
    caffemodelField->setText(settings.value(settingsPrefix+"caffemodel","").toString());
  }
  else
  {
    setParameters(previousParameters);
  }
}

ConvolutionalSegmenterParameterDialog::ConvolutionalSegmenterParameterDialog(QWidget *parent, Qt::WindowFlags f)
  : QDialog(parent,f)
{
  layout = new QFormLayout(this);
  buttonLayout = new QVBoxLayout();

  cropSizeField = new QSpinBox(this);
  prototxtField = new FileDirSelectionWidget(this);
  prototxtField->setFilter("Prototxt (*.prototxt)");
  caffemodelField = new FileDirSelectionWidget(this);
  caffemodelField->setFilter("Caffemodel (*.caffemodel)");

  okButton = new QPushButton("Ok",this);
  cancelButton = new QPushButton("Cancel",this);

  layout->addRow("Crop Size:",cropSizeField);
  cropSizeField->setMaximum(INT_MAX);
  layout->addRow("Prototxt File:",prototxtField);
  layout->addRow("Caffemodel File:",caffemodelField);
  layout->addRow(buttonLayout);

  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);

  connect(okButton,&QPushButton::clicked,this,&ConvolutionalSegmenterParameterDialog::ok);
  connect(cancelButton,&QPushButton::clicked,this,&ConvolutionalSegmenterParameterDialog::cancel);
}

ConvolutionalSegmenterParameterDialog::ConvolutionalSegmenterParameterDialog(const QString &settingsPrefix, QWidget *parent, Qt::WindowFlags f)
  : ConvolutionalSegmenterParameterDialog(parent,f)
{
  this->settingsPrefix = settingsPrefix;
  loadSettings();
}

ConvolutionalSegmenterParameters ConvolutionalSegmenterParameterDialog::getParameters()
{
  ConvolutionalSegmenterParameters parameters;
  parameters.cropSize = cropSizeField->value();
  parameters.prototxtPath = prototxtField->text().toStdString();
  parameters.caffemodelPath = caffemodelField->text().toStdString();
  return parameters;
}

void ConvolutionalSegmenterParameterDialog::setParameters(const ConvolutionalSegmenterParameters &parameters)
{
  cropSizeField->setValue(parameters.cropSize);
  prototxtField->setText(QString::fromStdString(parameters.prototxtPath));
  caffemodelField->setText(QString::fromStdString(parameters.caffemodelPath));
}
