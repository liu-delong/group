#ifndef PERSON_H
#define PERSON_H
#include <map>
#include <string>
#include <QString>
#include <vector>
using namespace std;
class Person
{

public:
    vector<int> _mutex_group;
    vector<int> _together_group;
    QString _name;
    Person();
    Person(QString name,vector<int> mutex_group,vector<int> together_group):_mutex_group(mutex_group),_together_group(together_group),_name(name){}
    operator QString()
    {
        return _name;
    }
};

#endif // PERSON_H
