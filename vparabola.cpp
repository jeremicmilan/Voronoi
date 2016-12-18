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

void VParabola::Display(Model *model) const
{
    if (this == nullptr)
    {
        return;
    }

    if (isLeaf)
    {
        model->DrawPoint(site, true /* isSpecial */);
        Draw(model, site, model->AnimationParameter());
    }
    else
    {
        Left()->Display(model);
        Right()->Display(model);
    }
}

void VParabola::Draw(Model *model, const VPoint *focus,
    double directrixHeight) const
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

    while (par->Left() == pLast)
    {
        if (!par->parent)
        {
            return 0;
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
