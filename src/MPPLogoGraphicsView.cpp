#include "MPPLogoGraphicsView.hpp"

#include <QDebug>

MPPLogoGraphicsView::MPPLogoGraphicsView(QWidget *parent):QGraphicsView(parent)
{
	this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // Disable scroll horizontally
		this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // Disable scroll vertically
		this->setAlignment(Qt::AlignCenter);                        // Make the contents of binding to the center
		this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);    // Stretch the widget content

		this->setMinimumHeight(100);
		this->setMinimumWidth(100);

		scene = new QGraphicsScene();   // Initialize the scene to render
		this->setScene(scene);          // Set the scene in a widget

		group_1 = new QGraphicsItemGroup(); // Initialize the first group of elements
		group_2 = new QGraphicsItemGroup(); // Initialize the elements of the second group

		scene->addItem(group_1);            // Add the first group into the scene

		timer = new QTimer();               // Initialize Timer


		connect(timer, SIGNAL(timeout()), this, SLOT(slotTimer()));
		timer->start(1);
		logoPixmap = new QPixmap(":/Graphics/Raster/MPPTile.png");

		int logoWidth = logoPixmap->width();
		int logoHeight = logoPixmap->height();

		for(int i=0; i<10; i++)
			for(int j=0; j<10; j++) {
				logoPixmapGraphicsItem[i][j] = new QGraphicsPixmapItem(*logoPixmap);
				logoPixmapGraphicsItem[i][j]->setPos(i*logoWidth, j*logoHeight);
				group_1->addToGroup(logoPixmapGraphicsItem[i][j]);
			}

		group_1->setPos(QPoint(60,60));

		group_1->setTransformOriginPoint(60,60);
}

MPPLogoGraphicsView::~MPPLogoGraphicsView()
{

}

void MPPLogoGraphicsView::slotTimer()
{
	currentRotationAngle += 0.1;
	if(currentRotationAngle >= 360)
		currentRotationAngle -= 360;
	group_1->setRotation(currentRotationAngle);

	currentScaleIndex += 0.001;
	if(currentScaleIndex >= 2*pi)
		currentScaleIndex -= 2*pi;
	group_1->setScale(std::sin(currentScaleIndex)*25.0+26.3);

}

void MPPLogoGraphicsView::resizeEvent(QResizeEvent *event)
{

}