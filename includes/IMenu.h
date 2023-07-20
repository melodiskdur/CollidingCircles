#pragma once

class IMenu
{
public:
    virtual const char* name() const = 0;
    virtual void draw() = 0;
};