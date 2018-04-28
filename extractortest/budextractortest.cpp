#include "budextractortest.h"
#include "imagelib/imagelibrary.h"
#include "convolutionalsegmenter/convolutionalsegmenter.h"

float BudExtractorTest::getPositionToleranceFactor() const
{
  return positionToleranceFactor;
}

void BudExtractorTest::setPositionToleranceFactor(float value)
{
  positionToleranceFactor = value;
}

QString BudExtractorTest::getLastInfoPath() const
{
  return lastInfoPath;
}

std::chrono::high_resolution_clock::duration BudExtractorTest::test(BudExtractor &extractor, const QString &outputDirPath)
{
  QDir outputDir(outputDirPath);
  if(!outputDir.exists())
  {
    QDir().mkpath(outputDirPath);
  }
  QFile file(outputDir.absoluteFilePath("results"));
  QTextStream ts(&file);
  bool writeResults = false;
  if(!outputDirPath.isEmpty())
  {
    if(file.open(QIODevice::WriteOnly))
    {
       writeResults = true;
       ts << "#filename" << " " << "f1score" << " " << "recall" << " " << "precision" << " " << "budsfound" << " " << "sigma(budsfound)" << " " << "actualbuds" << " " << "predictedbuds" << "\n";
    }
  }
  deviationStdDeviationValue = 0;
  meanDeviationValue = 0;
  truePositives  = 0;
  falsePositives = 0;
  falseNegatives  = 0;
  std::vector<double> deviations;
  std::chrono::high_resolution_clock::duration tTotal(0);
  size_t ctr = 0;
  foreach(const BudExtractorTestImage& tester, imageTesters)
  {
    std::string outputFile;
    if(!outputDirPath.isEmpty())
    {
      outputFile = outputDir.absoluteFilePath(QString::fromStdString(tester.getFilename())).toStdString();
    }
    size_t truePositivesLocal  = 0;
    size_t falsePositivesLocal = 0;
    size_t falseNegativesLocal  = 0;
    tTotal += tester.test(extractor,positionToleranceFactor,truePositivesLocal,falsePositivesLocal,falseNegativesLocal,outputFile);
    ctr++;
    double deviation = ((double)truePositivesLocal+(double)falsePositivesLocal)/(((double)truePositivesLocal+(double)falseNegativesLocal));
    if(writeResults)
    {
      double f1 = f1Score(truePositivesLocal,falsePositivesLocal,falseNegativesLocal);
      double rec = recall(truePositivesLocal,falsePositivesLocal,falseNegativesLocal);
      double prec = precision(truePositivesLocal,falsePositivesLocal,falseNegativesLocal);
      ts << QString::fromStdString(tester.getFilename()) << " " << f1 << " " << rec << " " << prec << " " << deviation << " " << truePositivesLocal+falseNegativesLocal << " " << truePositivesLocal+falsePositivesLocal << "\n";
      ts.flush();
    }
    deviations.push_back(deviation);
    truePositives   += truePositivesLocal;
    falsePositives  += falsePositivesLocal;
    falseNegatives  += falseNegativesLocal;
  }
  for(size_t i=0;i<deviations.size();i++)
     meanDeviationValue += deviations[i]/deviations.size();
  for(size_t i=0;i<deviations.size();i++)
  {
    deviationStdDeviationValue += std::pow(meanDeviationValue - deviations[i],2)/deviations.size();
  }
  deviationStdDeviationValue = std::sqrt(deviationStdDeviationValue);

  if(writeResults)
  {
    ts << "total" << " " << f1Score() << " " << recall() << " " << precision() << " " << meanDeviationValue << " " << deviationStdDeviationValue << " " << truePositives+falseNegatives << " " << truePositives+falsePositives << "\n";
    ts.flush();
    file.close();
  }

  return tTotal/ctr;
}

double BudExtractorTest::precision()
{
  if(truePositives+falsePositives == 0)
    return 0;
  return ((double)truePositives)/(truePositives+falsePositives);
}

double BudExtractorTest::recall()
{
  if(truePositives+falseNegatives == 0)
    return 0;
  return ((double)truePositives)/(truePositives+falseNegatives);
}

double BudExtractorTest::f1Score()
{
  return fScore(1);
}

double BudExtractorTest::fScore(float beta)
{
  double prec = precision();
  double rec = recall();
  if(((beta*beta*prec)+rec)==0)
    return 0;
  return (1+beta*beta)*(prec*rec)/((beta*beta*prec)+rec);
}

double BudExtractorTest::meanDeviation()
{
  return meanDeviationValue;
}

double BudExtractorTest::getDeviationStdDeviationValue() const
{
  return deviationStdDeviationValue;
}

void BudExtractorTest::setDeviationStdDeviationValue(double value)
{
  deviationStdDeviationValue = value;
}

BudExtractorTest::SegmentationOptions BudExtractorTest::getCurrentSegmentation() const
{
  return currentSegmentation;
}

void BudExtractorTest::setCurrentSegmentation(const SegmentationOptions &value)
{
  if(currentSegmentation != value)
  {
    settingsChanged = true;
    currentSegmentation = value;
  }
}

ConvolutionalSegmenterParameters BudExtractorTest::getConvolutionalSegmenterParameters() const
{
  return convolutionalSegmenterParameters;
}

void BudExtractorTest::setConvolutionalSegmenterParameters(const ConvolutionalSegmenterParameters &value)
{
  if(convolutionalSegmenterParameters != value)
  {
    settingsChanged = true;
  }
  convolutionalSegmenterParameters = value;
}

double BudExtractorTest::precision(size_t truePositives, size_t falsePositives, size_t falseNegatives)
{
  if(truePositives+falsePositives == 0)
    return 0;
  return ((double)truePositives)/(truePositives+falsePositives);
}

double BudExtractorTest::recall(size_t truePositives, size_t falsePositives, size_t falseNegatives)
{
  if(truePositives+falseNegatives == 0)
    return 0;
  return ((double)truePositives)/(truePositives+falseNegatives);
}

double BudExtractorTest::f1Score(size_t truePositives, size_t falsePositives, size_t falseNegatives)
{
  return fScore(truePositives,falsePositives,falseNegatives,1);
}

double BudExtractorTest::fScore(size_t truePositives, size_t falsePositives, size_t falseNegatives, float beta)
{
  double prec = precision(truePositives,falsePositives,falseNegatives);
  double rec = recall(truePositives,falsePositives,falseNegatives);
  if(((beta*beta*prec)+rec)==0)
    return 0;
  return (1+beta*beta)*(prec*rec)/((beta*beta*prec)+rec);
}

BudExtractorTest::BudExtractorTest()
{
  settingsChanged = true;
  currentSegmentation = SEGMENTATION_ANNOTATED;
  positionToleranceFactor = 1.5;
}
#include <QDebug>
bool BudExtractorTest::loadInfos(const QString &path, bool force)
{
  if(path==lastInfoPath && !force && !settingsChanged)
    return true;
  imageTesters.clear();
  ImageLibrary lib;
  if(lib.load(path))
  {
    lastInfoPath = path;
    ConvolutionalSegmenter* convseg = nullptr;
    if(currentSegmentation==SEGMENTATION_CNN)
      convseg = new ConvolutionalSegmenter(convolutionalSegmenterParameters);
    for(int i=0;i<lib.getInfoCount();i++)
    {
      if(!lib.getInfos()[i].isInTestSet())
        continue;
      BudExtractorTestImage image;
      image.setImage(lib.getInfos()[i].getImage());
      switch(currentSegmentation)
      {
        case SEGMENTATION_ANNOTATED:
          image.setMask(lib.getInfos()[i].getMap());
          break;
        case SEGMENTATION_CNN:
        {
          std::cout << "starting segmentation..." << std::endl;
          cv::Mat mask = convseg->segment(image.getImage(),1);
          std::cout << mask.size << std::endl;
          cv::threshold(mask,mask,.5,255,cv::THRESH_BINARY);
          mask.convertTo(mask,CV_8U);
          image.setMask(mask);
          break;
        }
        case SEGMENTATION_NONE:
        {
          cv::Mat mask(image.getImage().rows,image.getImage().cols,CV_8U);
          mask = 255;
          image.setMask(mask);
        }
      }
      image.setBudsTruth(lib.getInfos()[i].getPoints());
      image.setFilename(lib.getInfos()[i].getFilename().toStdString());
      imageTesters.push_back(image);
    }
    delete convseg;
    return true;
  }
  else
  {
    return false;
  }
}
