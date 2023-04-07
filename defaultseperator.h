#ifndef DEFAULTSEPERATOR_H
#define DEFAULTSEPERATOR_H

#include "seperator.h"

class DefaultSeperator : public Seperator
{
public:
    DefaultSeperator()=delete;
    DefaultSeperator(vector<int> mutex_nums,vector<int> together_nums):_mutex_nums(mutex_nums),_together_nums(together_nums){}
    bool valid(vector<vector<Person>> groups) override;
private:
    vector<int> _mutex_nums;
    vector<int> _together_nums;
};

#endif // DEFAULTSEPERATOR_H
