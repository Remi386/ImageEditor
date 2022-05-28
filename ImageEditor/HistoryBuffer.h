#pragma once
#include <QList>

class QImage;

class HistoryBuffer {
public:

	HistoryBuffer();

	void clear();

	bool getNext(QImage& imgCopyTo);
	
	bool getPrev(QImage& imgCopyTo);

	bool addImage(const QImage& imgCopyFrom);

private:
	int index = -1;
	int imagesCount = 0;
	QList<QImage> buffer;
};