#include "Picture.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>

Picture::Picture(Mat _image)
{
	image = _image;
}

Picture::Picture(String path)
{
	image = imread(path);
}

Mat Picture::getHistogram(uint histSizeX, uint histSizeY)
{
	std::vector<Mat>	bgr;
	int					histSize = 256;
	float				range[] = { 0, 256 };
	const float*		histRange = { range };
	Mat					hist_b, hist_g, hist_r;

	split(image, bgr);
	calcHist(&bgr[0], 1, 0, Mat(), hist_b, 1, &histSize, &histRange, true, false);
	calcHist(&bgr[1], 1, 0, Mat(), hist_g, 1, &histSize, &histRange, true, false);
	calcHist(&bgr[2], 1, 0, Mat(), hist_r, 1, &histSize, &histRange, true, false);

	int histWidth = cvRound((double)histSizeX / histSize);
	Mat histImage(histSizeY, histSizeX, CV_8UC3, Scalar(0, 0, 0));

	normalize(hist_b, hist_b, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(hist_g, hist_g, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(hist_r, hist_r, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(histWidth*(i - 1), histSizeY - cvRound(hist_b.at<float>(i - 1))), Point(histWidth*(i), histSizeY - cvRound(hist_b.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(histWidth*(i - 1), histSizeY - cvRound(hist_g.at<float>(i - 1))), Point(histWidth*(i), histSizeY - cvRound(hist_g.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(histWidth*(i - 1), histSizeY - cvRound(hist_r.at<float>(i - 1))), Point(histWidth*(i), histSizeY - cvRound(hist_r.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);
	}

	return histImage;
}

void Picture::resize(uint targetWidth)
{
	int		width = image.cols,
			height = image.rows;

	Mat		square = Mat::zeros(targetWidth, targetWidth, image.type());

	int		maxDim = (width >= height) ? width : height;
	float	scale = ((float)targetWidth) / maxDim;
	Rect	roi;

	if (width >= height)
	{
		roi.width = targetWidth;
		roi.x = 0;
		roi.height = height * scale;
		roi.y = (targetWidth - roi.height) / 2;
	}
	else
	{
		roi.y = 0;
		roi.height = targetWidth;
		roi.width = width * scale;
		roi.x = (targetWidth - roi.width) / 2;
	}

	cv::resize(image, square(roi), roi.size());

	image = square;
}
