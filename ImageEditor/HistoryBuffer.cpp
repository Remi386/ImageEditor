#include "HistoryBuffer.h"
#include <QImage>
#include <utility>

constexpr int MaxBufferSize = 15;

HistoryBuffer::HistoryBuffer()
{
	for(int i = 0; i < MaxBufferSize; ++i)
		buffer.append(QImage());
}

bool HistoryBuffer::getNext(QImage& imageCopyTo)
{
	if (index < MaxBufferSize - 2) {
		imageCopyTo = buffer[++index];
	}

	return index == (imagesCount - 1) ? false : true;
}

bool HistoryBuffer::getPrev(QImage& imageCopyTo)
{
	if (index >= 1) {
		imageCopyTo = buffer[--index];
	}

	return index == 0 ? false : true;
}

bool HistoryBuffer::addImage(const QImage& img)
{
	bool returnFlag = true;

	if (index == (MaxBufferSize - 1)) {
		buffer.pop_front();
		--index;
		returnFlag = false;
	}
	
	buffer[++index] = img;

	imagesCount = qMin(imagesCount + 1, MaxBufferSize);
	return returnFlag;
}

void HistoryBuffer::clear()
{
	for(auto iter = buffer.begin(); iter != buffer.end(); ++iter)
	{
		*iter = QImage();
	}
	index = -1;
	imagesCount = 0;
}