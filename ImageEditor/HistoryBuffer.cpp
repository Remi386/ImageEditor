#include "HistoryBuffer.h"
#include <QImage>
#include <utility>

constexpr int MaxBufferSize = 16;

HistoryBuffer::HistoryBuffer()
{
	for(int i = 0; i < MaxBufferSize; ++i)
		buffer.append(QImage());
}

bool HistoryBuffer::getNext(QImage& imageCopyTo)
{
	if (index <= MaxBufferSize - 2) {
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
	if (index == (MaxBufferSize - 1)) {
		buffer.pop_front();
		buffer.append(img);
		return false;
	}
	
	buffer[++index] = img;
	
	imagesCount = qMin(index + 1, MaxBufferSize);
	return true;
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