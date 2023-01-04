#include "cachememory.h"

CACHEmemory::CACHEmemory()
{

}

void CACHEmemory::setV(int volume_)
{
    volume = volume_;

    for (int i=0; i<volume; i++) boxData.append(new stringBits(i+1));
}

void CACHEmemory::getData(bool* data)
{
    emit sendData(boxData.at(0));
}
