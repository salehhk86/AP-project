#ifndef OTHERITEM_HPP
#define OTHERITEM_HPP

#include "Item.hpp"

class OtherItem : public Item
{
public:
    //constructor
    OtherItem();
    OtherItem(long , const std::string &, const std::string &,double , double );

    //getter
    ItemType GetType() const override;
    void Print() const override;
};

#endif
