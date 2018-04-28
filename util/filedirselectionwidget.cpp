#include "filedirselectionwidget.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

FileDirSelectionWidget::FileDirSelectionWidget(QWidget *parent,
                                               QFileDialog::AcceptMode acceptMode,
                                               FileDirSelectionWidget::FileMode fileMode,
                                               QFileDialog::Option options)
  :QWidget(parent)
{
  setContentsMargins(0,0,0,0);
  this->fileMode = fileMode;
  layout = new QHBoxLayout(this);
  layout->setContentsMargins(0,0,0,0);
  lineEdit = new QLineEdit(this);
  browseButton = new QPushButton("Browse",this);
  layout->addWidget(lineEdit,1);
  layout->addWidget(browseButton,0);

  connect(lineEdit,&QLineEdit::textChanged,this,&FileDirSelectionWidget::fileTextChanged);
  connect(browseButton,&QPushButton::clicked,this,&FileDirSelectionWidget::browse);

  dialogFileMode = QFileDialog::Directory;
  this->dialogFileMode = dialogFileMode;
  this->options = options;
  if((acceptMode == QFileDialog::QFileDialog::AcceptOpen) && (fileMode == FileDirSelectionWidget::File))
    dialogFileMode = QFileDialog::ExistingFile;
  if((acceptMode == QFileDialog::QFileDialog::AcceptSave) && (fileMode == FileDirSelectionWidget::File))
    dialogFileMode = QFileDialog::AnyFile;

  dialog = new QFileDialog(this);
  dialog->setOption(QFileDialog::DontUseNativeDialog,false);
  dialog->setAcceptMode(acceptMode);
  dialog->setFileMode(dialogFileMode);
  dialog->setOptions(options);
}

QString FileDirSelectionWidget::text() const
{
  return lineEdit->text();
}

void FileDirSelectionWidget::setText(const QString &value)
{
  lineEdit->setText(value);
}

QString FileDirSelectionWidget::getFilter() const
{
  return filter;
}

void FileDirSelectionWidget::setFilter(const QString &value)
{
  filter = value;
}

int FileDirSelectionWidget::callDialog(QStringList& dst)
{
  QString ret;
  if(dialogFileMode == QFileDialog::Directory)
  {
    ret = QFileDialog::getExistingDirectory(nullptr,"Select Directory",text());
  }
  else if(dialogFileMode == QFileDialog::ExistingFile)
  {
    ret = QFileDialog::getOpenFileName(nullptr,"Select File",text(),filter);
  }
  else if(dialogFileMode == QFileDialog::AnyFile)
  {
    ret = QFileDialog::getSaveFileName(nullptr,"Select File",text(),filter);
  }
  if(!ret.isEmpty())
  {
    dst = QStringList(ret);
    return 1;
  }
  return 0;
}

void FileDirSelectionWidget::fileTextChanged(const QString &string)
{
  if(!string.isEmpty())
  {
    if(fileMode == FileDirSelectionWidget::File)
    {
      QFile file(string);
      if(!file.exists())
      {
        lineEdit->setToolTip("File does not exist.");
        lineEdit->setStyleSheet("color:#FF0000;");
      }
      else
      {
        lineEdit->setToolTip("");
        lineEdit->setStyleSheet("color:#000000;");
      }
    }
    else if (fileMode == FileDirSelectionWidget::Dir)
    {
      QDir dir(string);
      if(!dir.exists())
      {
        lineEdit->setToolTip("Directory does not exist.");
        lineEdit->setStyleSheet("color:#FF0000;");
      }
      else
      {
        lineEdit->setToolTip("");
        lineEdit->setStyleSheet("color:#000000;");
      }
    }
  }
  else
  {
    lineEdit->setToolTip("");
    lineEdit->setStyleSheet("color:#000000;");
  }
  emit textChanged(string);
}

void FileDirSelectionWidget::browse()
{
  QStringList list;
  int ret = callDialog(list);
  if(ret)
  {
    if(list.size()>0)
    {
      lineEdit->setText(list.first());
    }
  }
}
