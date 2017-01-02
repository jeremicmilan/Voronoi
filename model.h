#ifndef MODEL_H
#define MODEL_H

#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <vector>

#include "voronoi.h"
#include "vpoint.h"

#define MAX_NUM_OF_POINTS	100

#define ANIMATION_SPEED		1.0
#define ANIMATION_FPS		60

#define POINT_SIZE			5.0
#define PARABOLA_DRAW_STEP	3.0
#define PARABOLA_PRECISION	5.0

#define COLOR_DIAGRAM		Qt::black
#define COLOR_BEACH_LINE	Qt::green
#define COLOR_EVENT			Qt::blue

using namespace vor;

class MainWindow;

class Model : QObject
{
    Q_OBJECT

private:

    MainWindow *window;

    Voronoi *voronoi;
    Vertices *vertices;
    Edges *edges;
    EventsData eventsData;

    double width;
    double height;

    int numOfPoints;
    double animationParameter;

    QGraphicsScene *scene;
    std::vector <QGraphicsItem *> toDeleteFromScene;

    QTimer timer;
    bool animationOngoing;
    bool animationToOngoing;
    double animateToY;

    EventsData::iterator FindEventData(double y);

public:

    Model(MainWindow *window, int w, int h);

    double Width() const;

    void SetWidth(double w);

    double Height() const;

    void SetHeight(double h);

    int NumOfPoints() const;

    double MinHeight() const;

    double GetYFromAP(double ap);

    double GetAPFromY(double y);

    double ModelToDisplayX(double x);

    double ModelToDisplayY(double y);

    double DisplayToModelY(double y);

    void SetNumOfPoints(int n);

    void SetAnimationParameter(double ap, bool updateSlider = true);

    double AnimationParameter() const;

    QGraphicsScene *Scene();

    QTimer *Timer();

    bool AnimationOngoing() const;

    void SetAnimationOngoing(bool ao);

    void SetAnimateTo(double at);

    bool AnimationToOngoing() const;

    void SetAnimationToOngoing(bool ato);

    void Clear(bool clearAll = false);

    void Init();

    void Display(bool clearAll = false);

    void DrawPoint(VPoint *point, bool isMarked = false, bool isEvent = false);

    void DrawLine(const VEdge *edge);

    void DrawLine(
        double	startX,
        double	startY,
        double	endX,
        double	endY,
        bool	isBeachLine = false);

    void AnimateToPrevious();

    void AnimateToNext();

    void AnimateTo(double y);
    
    void AddPoint(double x, double y);

public slots:
    void Animate();

    void AnimateTo();
};

#endif // MODEL_H
