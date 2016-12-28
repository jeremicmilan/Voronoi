#include "vparabola.h"
#include "vpoint.h"

#include "model.h"

double ParabolaGetYFromX(const VPoint *focus, double directrixHeight, double x)
{
    return (pow((x - focus->x), 2) + pow(focus->y, 2) - pow(directrixHeight, 2))
           / (2 * (focus->y - directrixHeight));
}

VParabola::VParabola()
{
    site = 0;

    isLeaf = false;
    cEvent = 0;
    edge = 0;
    parent = 0;
    _left = nullptr;
    _right = nullptr;
}

VParabola::VParabola(VPoint *s)
{
    site = s;
    isLeaf = true;
    cEvent = 0;
    edge = 0;
    parent = 0;
    _left = nullptr;
    _right = nullptr;
}

VParabola *VParabola::DeepCopy() const
{
    if (this == nullptr)
    {
        return nullptr;
    }

    VParabola *newParabola = new VParabola();
    newParabola->isLeaf = isLeaf;

    if (isLeaf)
    {
        newParabola->site = new VPoint(site->x, site->y);
        return newParabola;
    }

    if (Left() != nullptr)
    {
        newParabola->SetLeft(Left()->DeepCopy());
    }

    if (Right() != nullptr)
    {
        newParabola->SetRight(Right()->DeepCopy());
    }

    return newParabola;
}

void VParabola::DeepDelete()
{
    if (this == nullptr || isLeaf)
    {
        return;
    }

    Left()->DeepDelete();
    Right()->DeepDelete();
    delete this;
}

void VParabola::Display(Model *model)
{
    if (this == nullptr)
    {
        return;
    }

    if (isLeaf)
    {
        const VParabola *left = GetLeft(this);
        const VParabola *right = GetRight(this);
        double sweepingLine = model->AnimationParameter();

        //Draw(model, site, sweepingLine);

        double xFrom = 0;
        double xTo = model->Width();

        if (nullptr != left)
        {
            bool firstIntersection = left->site->y > site->y;

            xFrom = Intersect(left, this, sweepingLine, firstIntersection).x;
        }

        if (nullptr != right)
        {
            bool firstIntersection = site->y > right->site->y;

            xTo = Intersect(this, right, sweepingLine, firstIntersection).x;
        }

        DrawFromTo(model, site, sweepingLine, xFrom, xTo);

        model->DrawPoint(site, true /* isMarked */);
    }
    else
    {
        Left()->Display(model);
        Right()->Display(model);
    }
}

void VParabola::Draw(
    Model *			model,
    const VPoint *	focus,
    double			directrixHeight) const
{
    double previousY = ParabolaGetYFromX(focus, directrixHeight, 0);
    double previousX = 0;

    for (double x = PARABOLA_DRAW_STEP; x < model->Width();
        x += qMax(PARABOLA_DRAW_STEP, qAbs(focus->x - x) / PARABOLA_PRECISION))
    {
        double currentY = ParabolaGetYFromX(focus, directrixHeight, x);

        if (currentY <= model->Height() && currentY >= 0
            || previousY <= model->Height() && previousY >= 0)
        {
            model->DrawLine(previousX, previousY, x, currentY);
        }

        previousY = currentY;
        previousX = x;
    }
}

void VParabola::DrawFromTo(
    Model *			model,
    const VPoint *	focus,
    double			directrixHeight,
    double			xFrom,
    double			xTo) const
{
    xFrom = qMax(xFrom, 0.0);
    xTo = qMin(xTo, model->Width());

    double previousY = ParabolaGetYFromX(focus, directrixHeight, xFrom);
    double previousX = xFrom;

    for (double x = xFrom + PARABOLA_DRAW_STEP; x < xTo;
        x += qMax(PARABOLA_DRAW_STEP, qAbs(focus->x - x) / PARABOLA_PRECISION))
    {
        double currentY = ParabolaGetYFromX(focus, directrixHeight, x);

        model->DrawLine(previousX, previousY, x, currentY,
            true /* isBeachLine */);

        previousY = currentY;
        previousX = x;
    }

    if (xFrom < xTo)
    {
        model->DrawLine(previousX, previousY, xTo,
            ParabolaGetYFromX(focus, directrixHeight, xTo),
            true /* isBeachLine */);
    }

#ifdef DEBUG
    std::cout << "focus: " << focus->x << " - draw from " << xFrom << " to " <<
        xTo << std::endl;
#endif
}

/*
 *   Tree operations (described in the header file)
 */
VParabola *VParabola::GetLeft(VParabola *p)
{
    return GetLeftChild(GetLeftParent(p));
}

VParabola *VParabola::GetRight(VParabola *p)
{
    return GetRightChild(GetRightParent(p));
}

VParabola *VParabola::GetLeftParent(VParabola *p)
{
    VParabola *par = p->parent;
    VParabola *pLast = p;

    if (par == nullptr)
    {
        return nullptr;
    }

    while (par->Left() == pLast)
    {
        if (!par->parent)
        {
            return nullptr;
        }

        pLast = par;
        par = par->parent;
    }

    return par;
}

VParabola *VParabola::GetRightParent(VParabola *p)
{
    VParabola *par = p->parent;
    VParabola *pLast = p;

    if (par == nullptr)
    {
        return nullptr;
    }

    while (par->Right() == pLast)
    {
        if (!par->parent)
        {
            return 0;
        }

        pLast = par; par = par->parent;
    }

    return par;
}

VParabola *VParabola::GetLeftChild(VParabola *p)
{
    if (!p)
    {
        return 0;
    }

    VParabola *par = p->Left();

    while (!par->isLeaf)
    {
        par = par->Right();
    }

    return par;
}

VParabola *VParabola::GetRightChild(VParabola *p)
{
    if (!p)
    {
        return 0;
    }

    VParabola *par = p->Right();

    while (!par->isLeaf)
    {
        par = par->Left();
    }

    return par;
}

VPoint VParabola::Intersect(
    const VParabola *	p1,
    const VParabola *	p2,
    double				sweepingLine,
    bool				firstIntersection)
{
    double a1 = p1->site->x;
    double b1 = p1->site->y;

    double a2 = p2->site->x;
    double b2 = p2->site->y;

    double c1 = sweepingLine;
    double c2 = sweepingLine;

    double D = b1 * b1 - c1 * c1;
    double E = 2 * (b1 - c1);
    double F = b2 * b2 - c2 * c2;
    double G = 2 * (b2 - c2);

    double A = G - E;
    double B = -2 * a1 * G + 2 * a2 * E;
    double C = G * a1 * a1 + D * G - E * a2 * a2 - E * F;

    double discriminant = B * B - 4 * A * C;

    if (discriminant > 0)
    {
        double x1 = (-B - sqrt(discriminant)) / (2 * A);
        double x2 = (-B + sqrt(discriminant)) / (2 * A);

        double x = firstIntersection ? qMin(x1, x2) : qMax(x1, x2);
        double y = ParabolaGetYFromX(p1->site, sweepingLine, x);

        return VPoint(x, y);
    }

    return VPoint(-1, -1);
}
