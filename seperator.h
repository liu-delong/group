#ifndef SEPERATOR_H
#define SEPERATOR_H
#include<vector>
#include"person.h"
class Seperator
{
    bool _stop=false;
public:
    Seperator();
    /*
     * @brief 将personList分成group_cnt组。如果成功，则返回分组结果。否则，返回包含0个元素的vector<vector<Person>>
    */
    virtual vector<vector<Person>> seperate(vector<Person> personList,int group_cnt);
    /*
     * 判断一次随机打乱是否合法
     */
    virtual bool valid(vector<vector<Person>> groups)=0;
    virtual void stop();

};

#endif // SEPERATOR_H
