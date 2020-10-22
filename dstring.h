#ifndef DSTRING_H
#define DSTRING_H
#include <string>
#include <array>

class dstringtbl {
public:
    dstring gib(int id);
    dstring add(std::string str);
private:
    std::array<dstring> strings;
};

class dstring
{
    public:
        dstring();
        std::string dget();
        void dset(std::string);
    protected:

    private:
        std::string value;
};

#endif // DSTRING_H
