#include "histogram.h"



Histogram::Histogram()
{

}
Histogram::Histogram(vector<int>data)
{
    vector<int> vec=data;
    //sort(vec.begin(), vec.end());
    int maxValue = *max_element(vec.begin(),vec.end());
    int minValue = *min_element(vec.begin(),vec.end());
    counts = new int[maxValue-minValue+1]();
    for (int i=0;i<vec.size();i++)
    {
        counts[vec[i]]++;
        Rawdata.push_back(vec[i]);
    }
    bmin = minValue;
    bmax = maxValue;

    binWidth = 1;
    binCount = maxValue-minValue+1;
    lowerOutlierCount = 0;
    upperOutlierCount = 0;

}
Histogram::Histogram(vector<double>data)
{
    vector<double> vec=data;
    //sort(vec.begin(), vec.end());
    double maxValue = *max_element(vec.begin(),vec.end());
    double minValue = *min_element(vec.begin(),vec.end());
    bmin = (int)minValue;
    bmax = (int)maxValue+1;
    counts = new int[bmax-bmin+1]();

    binWidth = 1;
    binCount = bmax-bmin+1;
    lowerOutlierCount = 0;
    upperOutlierCount = 0;

    for (int i=0;i<vec.size();i++)
    {
        record(vec[i]);
        Rawdata.push_back((vec[i]-minValue)*1000/(maxValue-minValue));
    }


}


Histogram::Histogram(double min, double max, int numberOfBins)
{
    bmin = min;
    bmax = max;
    binCount = numberOfBins;
    binWidth = (max-min)/numberOfBins;

}
Histogram::Histogram(double min, double max, double binWidth)
{
    bmin = min;
    bmax = max;
    Histogram::binWidth = binWidth;
    binCount = (int)ceil((max-min) / binWidth); // requires <cmath>


}

Histogram::~Histogram()
{
	if (counts)
		delete[] counts;
}
void Histogram::ResetHistogram(int numberOfBins, double WidthOfBins,double min, double max)
{
    if(counts)
        delete [] counts;
    if(fabs(max-min) > 0.000001)//not equal (user have reset)
    {
        bmin = min;
        bmax = max;
    }
    if(numberOfBins>0)
    {
        binCount = numberOfBins;
        binWidth = (bmax-bmin)/numberOfBins;
    }
    else if(WidthOfBins>0)
    {
        binWidth = WidthOfBins;
        //binCount = (int)ceil((bmax-bmin) / binWidth);
        binCount = (int)((bmax-bmin) / binWidth+1);
    }
    counts = new int[binCount]();
    //update hist counts[]
    for(int i=0;i<Rawdata.size();i++)
    {
        record(Rawdata[i]);
    }

}

void Histogram::record(double datum) {
    int bin = (int)((datum - bmin) / binWidth);
    if (bin < 0) {
        lowerOutlierCount++;
    } else if (bin >= binCount) {
        upperOutlierCount++;
    } else {
        counts[bin]++;
    }
}
int Histogram::bins() const
{
    return binCount;

}// Get the number of bins
double Histogram::getBinWidth() const
{
	return binWidth;
}// Get the width of bins
int Histogram::count(int bin) const
{
    return counts[bin];

}// Get the number of data points in some bin
int Histogram::countLowerOutliers() const
{
    return lowerOutlierCount;

}// Get count of values below the minimum
int Histogram::countUpperOutliers() const
{
    return upperOutlierCount;

}// Get count of values at or above the maximum
double Histogram::lowerBound() const
{
    return bmin;
}// Get the inclusive lower bound of a bin
double Histogram::upperBound() const
{
    return bmax;
}// Get the exclusive upper bound of a bin
int Histogram::getHistVec(vector<int> &res)
{
    res.clear();
    for(int i=0;i<binCount;i++)
    {
        res.push_back(counts[i]);
    }
    return 1;
}
