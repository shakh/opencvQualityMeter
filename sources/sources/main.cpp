#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "psnr.h"
#include "ssim.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cout << "Usage: QualityMeter.exe ReferenceVideo UnderTestVideo" << endl;
        return -1;
    }

    const string referenceVideo = argv[1],
		         underTestVideo = argv[2];
    int frameNum = 0;

    VideoCapture captureReference(referenceVideo),
                 captureUnderTest(underTestVideo);

    if ( !captureReference.isOpened())
    {
        cout  << "Could not open reference " << referenceVideo << endl;
        return -1;
    }

    if( !captureUnderTest.isOpened())
    {
        cout  << "Could not open case test " << underTestVideo << endl;
        return -1;
    }

    Size referenceFrameSize = Size((int) captureReference.get(CV_CAP_PROP_FRAME_WIDTH),
                                   (int) captureReference.get(CV_CAP_PROP_FRAME_HEIGHT)),
         underTestFrameSize = Size((int) captureUnderTest.get(CV_CAP_PROP_FRAME_WIDTH),
                                   (int) captureUnderTest.get(CV_CAP_PROP_FRAME_HEIGHT));

    if (referenceFrameSize != underTestFrameSize)
    {
        cout << "Different frame size!" << endl;
        return -1;
    }

    Mat referenceFrame, underTestFrame;
    double psnr, 
		   avgPsnr = 0,
		   avgSsimY = 0,
		   avgSsimU = 0,
		   avgSsimV = 0;
    Scalar ssim;
	
	captureReference >> referenceFrame;
    captureUnderTest >> underTestFrame;
    
	while(!referenceFrame.empty() || !underTestFrame.empty())
    {
		cout << "Frame" << ++frameNum;
        
        psnr = getPSNR(referenceFrame,underTestFrame);
        cout<<" PSNR="<<psnr<<" dB";
		avgPsnr += psnr;

        ssim = getMSSIM(referenceFrame,underTestFrame);
		cout << "   SSIM: "<< "Y=" << ssim.val[2]<< " U=" << ssim.val[1]<< " V=" << ssim.val[0]<<endl;
		avgSsimY += ssim.val[2];
		avgSsimU += ssim.val[2];
		avgSsimV += ssim.val[2];

		captureReference >> referenceFrame;
        captureUnderTest >> underTestFrame;
    }
	cout << "-----------------------------------------------------------------" << endl;
	cout << "Avg PSNR = " << avgPsnr/frameNum << "  Avg SSIM: Y = " << avgSsimY/frameNum << "  U = " << avgSsimU/frameNum << "  V = " << avgSsimV/frameNum;
    return 0;
}