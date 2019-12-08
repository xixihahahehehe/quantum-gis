#ifndef HISTOGRAM_H
#define HISTOGRAM_H
#include "math.h"
#include "algorithm"
#include "vector"

using namespace std;
class Histogram
{
public:
//    Histogram();
    Histogram(vector<int>data);
    Histogram(double min, double max, int numberOfBins);
    Histogram(double min, double max, double binWidth);
    //void InputDataVector(vector<int>data);
    void ResetHistogram(int numberOfBins=-1, double WidthOfBins=-1,double min=0, double max=0);

    void record(double datum);
    int bins() const;                    // Get the number of bins
    double getBinWidth() const;                // Get the width of bins
    int count(int bin) const;            // Get the number of data points in some bin
    int countLowerOutliers() const;      // Get count of values below the minimum
    int countUpperOutliers() const;      // Get count of values at or above the maximum
    double lowerBound() const;    // Get the inclusive lower bound of a bin
    double upperBound() const;    // Get the exclusive upper bound of a bin
    int getHistVec(vector<int> &res);	//Get the histogram vector
    virtual ~Histogram();

  private:
    vector<double> Rawdata;
    double bmin,bmax;

    double binWidth;
    int binCount;
    int lowerOutlierCount, upperOutlierCount;
    int* counts;
};

#endif // HISTOGRAM_H
