#include "parametertesterwidget.h"

#include "util/filedirselectionwidget.h"
#include <QFormLayout>
#include <QPushButton>
#include "extractordialog/chtextractorconfigdialog.h"
#include <limits>
#include <QTextStream>
#include <iostream>
#include <QMessageBox>
#include <QString>

void ParameterTesterWidget::loadSettings()
{

}

void ParameterTesterWidget::saveSettings()
{

}

void ParameterTesterWidget::addNewCandidates(std::priority_queue<ParameterTesterWidget::ParameterCandidate, std::vector<ParameterTesterWidget::ParameterCandidate> > &queue, const std::vector<ParameterTesterWidget::ParameterCandidate> &knownCandidates, const ParameterTesterWidget::ParameterCandidate candidate)
{
  std::vector<ParameterCandidate> candidates = candidate.generateNewCandidates();
  for(size_t i=knownCandidates.size() ; i --> 0 ;)
  {
    for(size_t j=0;j<candidates.size();j++)
    {
      if(candidates[j]==knownCandidates[i])
      {
        candidates.erase(candidates.begin()+j);
        j--;
      }
    }
  }
  for(size_t j=0;j<candidates.size();j++)
  {
    queue.push(candidates[j]);
  }
}

std::vector<ParameterTesterWidget::ParameterCandidate> ParameterTesterWidget::loadCandidates(QFile &file)
{
  std::vector<ParameterTesterWidget::ParameterCandidate> result;
  QTextStream ts(&file);
  while(!ts.atEnd())
  {
    QStringList line = ts.readLine().split(' ');
    if(line.size()<3)
      continue;
    bool ok;
    int cannyThreshold = line[0].toInt(&ok);
    if(!ok)
      continue;
    double chtThreshold = line[1].toDouble(&ok);
    if(!ok)
      continue;
    double f1 = line[2].toDouble(&ok);
    if(!ok)
      continue;
    result.push_back(ParameterCandidate(cannyThreshold,chtThreshold,f1));
  }
  return result;
}

ParameterTesterWidget::ParameterTesterWidget(QString *dataFile, QWidget *parent) : QWidget(parent)
{
  this->dataFile = dataFile;
  extractorDialog = new ChtExtractorConfigDialog("cht-parametertester/",this);
  configButton = new QPushButton("Config",this);
  startButton = new QPushButton("Start",this);
  dataLayout = new QFormLayout(this);
  dataLayout->addRow("Start Extractor:",configButton);
  outputFileField = new FileDirSelectionWidget(this,QFileDialog::AcceptSave,FileDirSelectionWidget::File);
  dataLayout->addRow("Output File:",outputFileField);
  dataLayout->addRow(startButton);

  connect(startButton,&QPushButton::clicked,this,&ParameterTesterWidget::startClicked);
  connect(configButton,&QPushButton::clicked,this,&ParameterTesterWidget::configClicked);

  loadSettings();
}

void ParameterTesterWidget::startClicked()
{
  tester.setPositionToleranceFactor(1.5);
  if(!tester.loadInfos(*dataFile))
  {
    return;
  }
  ChtBudExtractor extractor = static_cast<ChtBudExtractor&>(extractorDialog->getExtractor());
  std::vector<ParameterCandidate> checkedParameters;
  std::priority_queue<ParameterCandidate,std::vector<ParameterCandidate>> queue;

  QFile file(outputFileField->text());
  bool continueFile = false;
  if(file.open(QIODevice::ReadOnly))
  {
    std::vector<ParameterCandidate> loadedParameters = loadCandidates(file);
    if(loadedParameters.size()>0)
    {
      int answer = QMessageBox::question(this,"Import","Do you want to import the previous parameter results from the output file?",QMessageBox::Yes,QMessageBox::No);
      if(answer == QMessageBox::Yes)
      {
        continueFile = true;
        checkedParameters = loadedParameters;
        for(size_t i=checkedParameters.size() ; i --> 0 ;)
        {
          addNewCandidates(queue,checkedParameters,checkedParameters[i]);
        }
      }
    }
    file.close();
  }

  if(continueFile)
  {
    if(!file.open(QIODevice::Append))
    {
      return;
    }
  }
  else
  {
    if(!file.open(QIODevice::WriteOnly))
    {
      return;
    }
  }
  QTextStream output(&file);
  if(!continueFile)
  {
    output << "#LowerCannyThreshold ChtThreshold f1Score recall precision meanDeviation deviationStdDeviation\n";
    output.flush();
    file.flush();
  }

  queue.push(ParameterCandidate(extractor.getLowerThreshold(),extractor.getChtThreshold(),0));
  while(queue.size()!=0)
  {
    ParameterCandidate candidate = queue.top();
    queue.pop();
    bool found = false;
    for(size_t i=checkedParameters.size() ; i --> 0 ;)
    {
      if(candidate==checkedParameters[i])
      {
        found = true;
        break;
      }
    }
    if(found)
      continue;
    //test current candidate
    extractor.setChtThreshold(candidate.getHoughThreshold());
    extractor.setLowerThreshold(candidate.getCannyThreshold());
    extractor.setUpperThreshold(candidate.getCannyThreshold()*2);
    tester.test(extractor);
    float f1 = tester.f1Score();
    float recall = tester.recall();
    float precision = tester.precision();
    float meanDeviation = tester.meanDeviation();
    float deviationStdDeviation = tester.getDeviationStdDeviationValue();
    candidate.setEstimatedScore(f1);
    checkedParameters.push_back(candidate);
    //write to file
    output << candidate.getCannyThreshold() << " " << candidate.getHoughThreshold() << " " << f1 << " " << recall << " " << precision << " " << meanDeviation << " " << deviationStdDeviation << "\n";
    output.flush();
    file.flush();
    //generate new candidates
    addNewCandidates(queue,checkedParameters,candidate);
  }
  std::cout << "queue empty!" << std::endl;
  file.close();
}

void ParameterTesterWidget::configClicked()
{
  extractorDialog->exec();
}

int ParameterTesterWidget::ParameterCandidate::getCannyThreshold() const
{
  return cannyThreshold;
}

void ParameterTesterWidget::ParameterCandidate::setCannyThreshold(int value)
{
  cannyThreshold = value;
}

float ParameterTesterWidget::ParameterCandidate::getHoughThreshold() const
{
  return houghThreshold;
}

void ParameterTesterWidget::ParameterCandidate::setHoughThreshold(float value)
{
  houghThreshold = value;
}

float ParameterTesterWidget::ParameterCandidate::getEstimatedScore() const
{
  return estimatedScore;
}

void ParameterTesterWidget::ParameterCandidate::setEstimatedScore(float value)
{
  estimatedScore = value;
}

ParameterTesterWidget::ParameterCandidate::ParameterCandidate()
{

}

ParameterTesterWidget::ParameterCandidate::ParameterCandidate(int cannyThreshold, float houghThreshold, float estimatedScore)
{
  this->cannyThreshold = cannyThreshold;
  this->houghThreshold = houghThreshold;
  this->estimatedScore = estimatedScore;
}

bool ParameterTesterWidget::ParameterCandidate::operator <(const ParameterTesterWidget::ParameterCandidate &other) const
{
  return getEstimatedScore() < other.getEstimatedScore();
}

bool ParameterTesterWidget::ParameterCandidate::operator ==(const ParameterTesterWidget::ParameterCandidate &other) const
{
  return (this->getCannyThreshold()==other.getCannyThreshold()) &&
         (this->getHoughThreshold()==other.getHoughThreshold());
}

std::vector<ParameterTesterWidget::ParameterCandidate> ParameterTesterWidget::ParameterCandidate::generateNewCandidates() const
{
  std::vector<ParameterTesterWidget::ParameterCandidate> result;
  result.push_back(*this);
  result.push_back(*this);
  result.push_back(*this);
  result.push_back(*this);
  result[0].setCannyThreshold(getCannyThreshold()+1);
  result[1].setCannyThreshold(getCannyThreshold()-1);
  result[2].setHoughThreshold(getHoughThreshold()+0.01);
  result[3].setHoughThreshold(getHoughThreshold()-0.01);
  return result;
}
