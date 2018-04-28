#ifndef EXTRACTORCONFIGDIALOG_H
#define EXTRACTORCONFIGDIALOG_H

#include <QDialog>
#include <QString>
#include "budextractor.h"

class QPushButton;
class QHBoxLayout;
class QSettings;

class ExtractorConfigDialog : public QDialog
{
  Q_OBJECT
  QString errorString;
  QString settingsPrefix;
protected:
  void setError(const QString& errorString);
  const QString& getError();
  QHBoxLayout* buttonLayout;
  QPushButton* okButton;
  QPushButton* cancelButton;
  virtual bool extractorToFields() = 0;
  virtual bool fieldsToExtractor() = 0;
  virtual void saveToSettings();
  virtual void loadFromSettings();

  void setValue(int& dst, const QVariant& variant);
  void setValue(float& dst, const QVariant& variant);
  ExtractorConfigDialog(QWidget* parent, Qt::WindowFlags f = Qt::WindowFlags());
public:
  virtual const QString& name() = 0;
  virtual BudExtractor& getExtractor() = 0;
  QString getSettingsPrefix() const;
  void setSettingsPrefix(const QString &value);

private slots:
  void okPressed();
  void cancelPressed();
};

#endif // EXTRACTORCONFIGDIALOG_H
