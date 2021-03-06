#include <core/Producer.h>
#include <core/Consumer.h>
#include <core/DequePool.h>
#include <core/ConsumerProducer.h>

#include <opencv2/opencv.hpp>

#include "NamedWindow.h"

class ImageProcessor : public ConsumerProducer<cv::Mat, cv::Mat>
{
public:
	ImageProcessor(std::shared_ptr<Producer<cv::Mat>> producer, size_t maxQueueSize)
	: ConsumerProducer(producer, maxQueueSize)
	{
	}

	void onStart()
	{}

	void onShutdown()
	{}

	std::unique_ptr<cv::Mat> process(std::unique_ptr<cv::Mat> inData, std::unique_ptr<cv::Mat> outData)
	{
		cv::Mat *out = outData.release();
		out->create(inData->size(), inData->type());
		inData->copyTo(*out);
		recycleData(std::move(inData));

		cv::putText(*out, "Processing frame ...", cvPoint(30,30), 5, 0.8, cvScalar(200,200,200), 1, CV_AA);

		NamedWindow::showNamedImage("ImageProcessor", *out);

		return std::unique_ptr<cv::Mat>(out);
	}
};
