#include "model.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

Model::Model(MainWindow *wind, int w, int h) :
    window(wind),
    width(w),
    height(h),
    numOfPoints(0),
    animationOngoing(false)
{
    voronoi = new Voronoi();
    vertices = new Vertices();
    scene = new QGraphicsScene(0, 0, w, h);

    animationParameter = ModelToDisplayY(0);

    timer.setInterval(1.0 / ANIMATION_FPS);
    connect(&timer, SIGNAL(timeout()), this, SLOT(Animate()));

    srand((unsigned)time(NULL));
}

double Model::Width() const
{
    return width;
}

void Model::SetWidth(double w)
{
    width = w;
}

double Model::Height() const
{
    return height;
}

void Model::SetHeight(double h)
{
    height = h;
}

int Model::NumOfPoints() const
{
    return numOfPoints;
}

double Model::MinHeight() const
{
    if (eventsData.size() > 0)
    {
        return qMin(eventsData.back().ly, -height);
    }
    else
    {
        return -height;
    }
}

double Model::GetYFromAP(double ap)
{
    double maxHeight = ModelToDisplayY(MinHeight());

    return ModelToDisplayY(ap * maxHeight);
}

double Model::GetAPFromY(double y)
{
    double maxHeight = ModelToDisplayY(MinHeight());

    return DisplayToModelY(y) / maxHeight;
}

double Model::ModelToDisplayX(double x)
{
    return x;
}

double Model::ModelToDisplayY(double y)
{
    return height - y;
}

double Model::DisplayToModelY(double y)
{
    return height - y;
}

void Model::SetAnimationParameter(double ap, bool updateSlider)
{
    animationParameter = ap;

    if (updateSlider)
    {
        int max = window->ui->hsAnimationParameter->maximum();
        window->ui->hsAnimationParameter->setValue(GetAPFromY(ap) * max);
    }
}

double Model::AnimationParameter() const
{
    return animationParameter;
}

QGraphicsScene *Model::Scene()
{
    return scene;
}

QTimer *Model::Timer()
{
    return &timer;
}

bool Model::AnimationOngoing() const
{
    return animationOngoing;
}

void Model::SetAnimationOngoing(bool ao)
{
    animationOngoing = ao;

    if (ao)
    {
        SetAnimationToOngoing(false);
        timer.start();
        window->ui->btnStart->setText("Stop");
    }
    else
    {
        timer.stop();
        window->ui->btnStart->setText("Start");
    }
}

void Model::SetAnimateTo(double at)
{
    animateToY = at;

    if (at)
    {
        SetAnimationOngoing(false);
    }
}

bool Model::AnimationToOngoing() const
{
    return animationToOngoing;
}

void Model::SetAnimationToOngoing(bool ato)
{
    animationToOngoing = ato;
}

void Model::Clear(bool clearAll)
{
    if (clearAll)
    {
        for (VPoint *point : *vertices)
        {
            delete point;
        }

        vertices->clear();
        numOfPoints = 0;

        scene->clear();
        toDeleteFromScene.clear();
    }

    for (EventData ed : eventsData)
    {
        ed.root->DeepDelete();
    }

    eventsData.clear();
}

void Model::Init()
{
    if (numOfPoints <= 0)
    {
        return;
    }

    Clear();

    edges = voronoi->GetEdges(vertices, (int)width, (int)height, eventsData);
    std::cout << "Voronoi diagram done!" << std::endl;
    std::cout << "Number of points: " << numOfPoints << std::endl;

#ifdef DEBUG
    for (EventData ed : eventsData)
    {
        std::cout << ed.ly << " ";
    }
    std::cout << std::endl;
#endif
}

void Model::DrawPoint(VPoint *point, bool isMarked, bool isEvent)
{
    double x = ModelToDisplayX(point->x);
    double y = ModelToDisplayY(point->y);

    QBrush brush(COLOR_DIAGRAM);

    if (isMarked)
    {
        if (isEvent)
        {
            brush.setColor(COLOR_EVENT);
        }
        else
        {
            brush.setColor(COLOR_BEACH_LINE);
        }
        brush.setStyle(Qt::BrushStyle::SolidPattern);
    }

    QGraphicsRectItem *item = scene->addRect(x - POINT_SIZE / 2,
            y - POINT_SIZE / 2,
            POINT_SIZE,
            POINT_SIZE,
            QPen(),
            brush);

    if (isMarked)
    {
        toDeleteFromScene.push_back(item);
    }
}

void Model::DrawLine(const VEdge *edge)
{
    DrawLine(edge->start->x, edge->start->y, edge->end->x, edge->end->y);
}

void Model::DrawLine(
    double	startX,
    double	startY,
    double	endX,
    double	endY,
    bool	isBeachLine)
{
    startX = ModelToDisplayX(startX);
    startY = ModelToDisplayY(startY);
    endX = ModelToDisplayX(endX);
    endY = ModelToDisplayY(endY);

    QGraphicsLineItem *item = scene->addLine(startX, startY, endX, endY,
            QPen(isBeachLine ? COLOR_BEACH_LINE : COLOR_DIAGRAM));

    if (isBeachLine)
    {
        toDeleteFromScene.push_back(item);
    }
}

void Model::AnimateToPrevious()
{
    EventsData::iterator it = FindEventData(animationParameter);

    if (it == eventsData.begin())
    {
        return;
    }

    double y = (it - 1)->ly;

    if (y == AnimationParameter())
    {
        if (it - 1 != eventsData.begin())
        {
            AnimateTo((it - 2)->ly);
        }
    }
    else
    {
        AnimateTo(y);
    }
}

void Model::AnimateToNext()
{
    EventsData::iterator it = FindEventData(animationParameter) + 1;

    if (it == eventsData.end())
    {
        return;
    }

    if (it + 1 == eventsData.end())
    {
        AnimateTo(MinHeight());
    }
    else
    {
        AnimateTo(it->ly);
    }
}

void Model::AnimateTo(double y)
{
    SetAnimationToOngoing(true);

    SetAnimateTo(y);
    AnimateTo();
}

void Model::AddPoint(double x, double y)
{
    if (numOfPoints < MAX_NUM_OF_POINTS)
    {
        vertices->push_front(new VPoint(x, y));
        numOfPoints++;
    }
    else
    {
        std::cerr	<< "Max number ("
                    << MAX_NUM_OF_POINTS
                    << ") of points exceeded!"
                    << std::endl;
    }
}

void Model::Display(bool clearAll)
{
    if (numOfPoints <= 0)
    {
        return;
    }

    if (clearAll)
    {
        scene->clear();
        toDeleteFromScene.clear();

        // Draw points
        for (VPoint *point : *vertices)
        {
            DrawPoint(point);
        }

        // Draw Edges
        for (VEdge *edge: *edges)
        {
            DrawLine(edge);

#ifdef DEBUG
            if (edge->start->x < 0 || edge->start->x > width ||
                edge->start->y < 0 || edge->start->y > height)
            {
                if (edge->end->x < 0 || edge->end->x > width ||
                    edge->end->y < 0 || edge->end->y > height)
                {
                    std::cout << "Invalid line" << std::endl;
                }
            }
#endif
        }
    }
    else
    {
        for (QGraphicsItem *item : toDeleteFromScene)
        {
            scene->removeItem(item);
            delete item;
        }

        toDeleteFromScene.clear();
    }

    EventData &eventData = *FindEventData(animationParameter);

    // Draw beach line
    eventData.root->Display(this);

    // Draw sweeping line
    double sweepingLineY = ModelToDisplayY(animationParameter);
    QGraphicsLineItem *line = scene->addLine(
        ModelToDisplayX(0), sweepingLineY,
        ModelToDisplayX(width), sweepingLineY,
        QPen(COLOR_EVENT));
    toDeleteFromScene.push_back(line);

#ifdef DEBUG
    std::cout << "Sweeping line: " << sweepingLineY << std::endl;
#endif

    // Draw Event
    if (eventData.isPointEvent)
    {
        if (ModelToDisplayY(eventData.ly) == sweepingLineY)
        {
            double x = ModelToDisplayX(eventData.pointInserted.x);
            double y = ModelToDisplayY(eventData.pointInserted.y);

            QGraphicsLineItem *line = scene->addLine(
                x, y,
                x, 0,
                QPen(COLOR_EVENT));
            toDeleteFromScene.push_back(line);

            DrawPoint(&eventData.pointInserted, true, true);
        }
    }
    else
    {
        if (ModelToDisplayY(eventData.ly) == sweepingLineY)
        {
            double x = ModelToDisplayX(eventData.circleCenter.x);
            double y = ModelToDisplayY(eventData.circleCenter.y);
            double r = y - sweepingLineY;

            QGraphicsEllipseItem *ellipse = scene->addEllipse(
                x - r, y - r,
                2 * r, 2 * r,
                QPen(COLOR_EVENT));
            toDeleteFromScene.push_back(ellipse);

            DrawPoint(&eventData.circleCenter, true, true);
        }
    }
}

void Model::Animate()
{
    if (animationParameter < MinHeight())
    {
        timer.stop();
        animationOngoing = false;
        return;
    }

    SetAnimationParameter(animationParameter - ANIMATION_SPEED);

    Display();
}

void Model::AnimateTo()
{
    if (animateToY != animationParameter && !animationOngoing &&
        animationToOngoing)
    {
        if (qAbs(animationParameter - animateToY) > ANIMATION_SPEED)
        {
            int dir = (animationParameter > animateToY ? -1 : 1);
            SetAnimationParameter(animationParameter + dir * ANIMATION_SPEED);
            QTimer::singleShot(1.0 / ANIMATION_FPS, this, SLOT(AnimateTo()));
        }
        else
        {
            animationParameter = animateToY;
        }

        Display();
    }
}

EventsData::iterator Model::FindEventData(double y)
{
    if (eventsData.size() > 0)
    {
        auto eventData = std::lower_bound(eventsData.begin(), eventsData.end(),
                EventData(y), [](const EventData &ed1, const EventData &ed2)
        {
            return !(ed1 <= ed2);
        }
                );

        if (eventData == eventsData.end())
        {
            return eventData - 1;
        }
        else
        {
            return eventData;
        }
    }

    return eventsData.end();
}
