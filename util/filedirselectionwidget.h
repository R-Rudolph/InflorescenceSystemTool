#ifndef FILEDIRSELECTIONWIDGET_H
#define FILEDIRSELECTIONWIDGET_H

#include <QWidget>
#include <QFileDialog>

class QHBoxLayout;
class QLineEdit;
class QPushButton;

class FileDirSelectionWidget : public QWidget
{
  Q_OBJECT
public:
  enum FileMode
  {
    File,
    Dir
  };
  explicit FileDirSelectionWidget(QWidget *parent = nullptr,
                                  QFileDialog::AcceptMode acceptMode = QFileDialog::AcceptOpen,
                                  FileDirSelectionWidget::FileMode fileMode = FileDirSelectionWidget::File,
                                  QFileDialog::Option options = (QFileDialog::Option) 0);
  QString text() const;
  void setText(const QString& value);
  QString getFilter() const;
  void setFilter(const QString &value);

private:
  QFileDialog::FileMode dialogFileMode;
  QFileDialog::AcceptMode acceptMode;
  FileDirSelectionWidget::FileMode fileMode;
  QFileDialog::Option options;
  QString lastPath;
  QString filter;

  QFileDialog* dialog;
  QHBoxLayout* layout;
  QLineEdit* lineEdit;
  QPushButton* browseButton;
  int callDialog(QStringList& dst);
signals:
  void textChanged(const QString&);
private slots:
  void fileTextChanged(const QString& string);
  void browse();
};

#endif // FILEDIRSELECTIONWIDGET_H
