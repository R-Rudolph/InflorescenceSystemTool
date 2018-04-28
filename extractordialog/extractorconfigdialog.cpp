#include "extractorconfigdialog.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QVariant>

QString ExtractorConfigDialog::getSettingsPrefix() const
{
  return settingsPrefix;
}

void ExtractorConfigDialog::setSettingsPrefix(const QString &value)
{
  settingsPrefix = value;
}

void ExtractorConfigDialog::setError(const QString &errorString)
{
  this->errorString = errorString;
}

const QString &ExtractorConfigDialog::getError()
{
  return errorString;
}

void ExtractorConfigDialog::saveToSettings()
{

}

void ExtractorConfigDialog::loadFromSettings()
{

}

void ExtractorConfigDialog::setValue(int &dst, const QVariant &variant)
{
  bool ok;
  int value = variant.toInt(&ok);
  if(ok)
    dst = value;
}

void ExtractorConfigDialog::setValue(float &dst, const QVariant &variant)
{
  bool ok;
  float value = variant.toFloat(&ok);
  if(ok)
    dst = value;
}

ExtractorConfigDialog::ExtractorConfigDialog(QWidget* parent, Qt::WindowFlags f)
  : QDialog(parent,f)
{
  buttonLayout = new QHBoxLayout();
  okButton = new QPushButton("Ok",this);
  cancelButton = new QPushButton("Cancel",this);
  buttonLayout->addWidget(okButton);
  buttonLayout->addWidget(cancelButton);
  connect(okButton,&QPushButton::clicked,this,&ExtractorConfigDialog::okPressed);
  connect(cancelButton,&QPushButton::clicked,this,&ExtractorConfigDialog::cancelPressed);
}

void ExtractorConfigDialog::okPressed()
{
  if(fieldsToExtractor())
  {
    fieldsToExtractor();
    saveToSettings();
    QDialog::accept();
  }
  else
    QMessageBox::warning(this,"Error",getError(),QMessageBox::Ok,QMessageBox::Ok);
}

void ExtractorConfigDialog::cancelPressed()
{
  if(extractorToFields())
  {
    QDialog::reject();
    extractorToFields();
  }
  else
    QMessageBox::warning(this,"Error",getError(),QMessageBox::Ok,QMessageBox::Ok);
}
