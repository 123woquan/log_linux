#pragma once
class nocpoy
{
    protected:
    nocpoy()
    {

    };
    ~nocpoy()
    {

    };
    private:
    nocpoy(const nocpoy & obj)=default;
    nocpoy & operator=(const nocpoy & obj)= default;
};