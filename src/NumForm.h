#ifndef NUMFORM_H
#define NUMFORM_H

#include "Menu.h"

// Must be inside a menu option
class NumForm : public Menu
{
public:
    NumForm(const char* title);
    ~NumForm();

    void set(uint8_t v);
    uint8_t get();

    void begin(uint8_t min, uint8_t max, const char* incTit = "INC",
               const char* decTit = "DEC", const char* backTit = "BACK");

    virtual void run();

private:
    
    char str[4];
    uint8_t val;
    uint8_t min = 0;
    uint8_t max = 0;
};

inline void NumForm::set(uint8_t v)
{
    if(v >= min && v <= max)
        val = v;
}

inline uint8_t NumForm::get()
{
    return val;
}

#endif