#include "NumForm.h"

static uint8_t* numFormVal = NULL;
static uint8_t minFormVal = 0;
static uint8_t maxFormVal = 0;
static char* numFormStr = NULL;

static void numFormIncrement();
static void numFormDecrement();

NumForm::NumForm(const char* title) : Menu(4, title, NULL, MENU_ACTION_JUST_EXIT)
{
}

NumForm::~NumForm()
{
}

static void numFormIncrement()
{
    if((*numFormVal)<maxFormVal) 
        (*numFormVal)++; 
    sprintf(numFormStr, "%u", *numFormVal);
}

static void numFormDecrement()
{
    if((*numFormVal)>minFormVal) 
        (*numFormVal)--; 
    sprintf(numFormStr, "%u", *numFormVal);
}

void NumForm::run()
{
    numFormStr = str;
    numFormVal = &val;
    minFormVal = min;
    maxFormVal = max;
    Menu::run();
}

void NumForm::begin(uint8_t min, uint8_t max, const char* incTit,
                    const char* decTit, const char* backTit)
{
    this->min = min;
    this->max = max;
    val = min;
    sprintf(str, "%u", val);

    Menu::set_option(0, str, [](){});
	Menu::set_option(1, incTit, numFormIncrement);
	Menu::set_option(2, decTit, numFormDecrement);
	Menu::set_option(3, backTit, menu_force_close_current );
}
