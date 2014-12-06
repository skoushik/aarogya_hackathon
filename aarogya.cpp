#include <opencv2/opencv.hpp>
#include <zbar.h>
#include <stdio.h>

using namespace std;
using namespace cv;
using namespace zbar;

int main()
{
	namedWindow("Example", CV_WINDOW_AUTOSIZE);
	VideoCapture cap(0);
	vector<string> stringDatabase;
	string currentData;
	if(!cap.isOpened()) return -1;
	Mat frame;
	Mat grey;

	int width, height;
	uchar *raw;
	
	ImageScanner scanner;
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

	while(1)
	{
		cap >> frame;

		if(!frame.data)
			break;

		cvtColor(frame, grey, CV_BGR2GRAY);
		width = frame.cols;
		height = frame.rows;
		raw = (uchar *) grey.data;

		Image image(width,height,"Y800", raw, width*height);

		int n = scanner.scan(image);

		for(Image::SymbolIterator symbol = image.symbol_begin();
			symbol != image.symbol_end();
			++ symbol)
		{
			vector<Point> vp;
			cout << symbol->get_data() << endl;
			int n = symbol->get_location_size();
			for(int i = 0; i<n ; i++)
			{
				vp.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));
			}
			RotatedRect r = minAreaRect(vp);
			Point2f pts[4];
			r.points(pts);
			for(int i = 0; i<4; i++)
			{
				line(frame, pts[i], pts[(i+1)%4], Scalar(255,0,0),3);
			}

		}
		imshow("Example", frame);
		if(waitKey(33) >= 0) break;
	}
	return 0;
}
