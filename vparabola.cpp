#include "vparabola.h"
#include "vpoint.h"

/*
 *   Constructors
 */

VParabola::VParabola()
{
    site = 0;
    isLeaf = false;
    cEvent = 0;
    edge = 0;
    parent = 0;
}

VParabola::VParabola(VPoint *s)
{
    site = s;
    isLeaf = true;
    cEvent = 0;
    edge = 0;
    parent = 0;
}

VParabola::VParabola(const VParabola *other)
{
    site = other->site;
    isLeaf = other->isLeaf;
    cEvent = other->cEvent;
    edge = other->edge;
    parent = nullptr;
}

VParabola *VParabola::DeepCopy() const
{
    if (this == nullptr)
    {
        return nullptr;
    }

    VParabola *newParabola = new VParabola(this);

    newParabola->SetLeft(Left()->DeepCopy());
    newParabola->SetRight(Right()->DeepCopy());

    return newParabola;
}

void VParabola::DeepDelete()
{
    Left()->DeepDelete();
    Right()->DeepDelete();
    delete this;
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
