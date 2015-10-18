#include "ipseeker.h"

#include <fstream>
#include <iostream>
#include <cassert>
#include <sstream>

namespace {
template <class _Val, class _Src>
_Val getValue(_Src src)
{
    return *reinterpret_cast<_Val *>(&*src);
}

std::string ntos(uint32_t ip)
{
    std::stringstream ss;
    ss << ((ip & 0xFF000000) >> 24) << ".";
    ss << ((ip & 0x00FF0000) >> 16) << ".";
    ss << ((ip & 0x0000FF00) >> 8 ) << ".";
    ss << ((ip & 0x000000FF) >> 0 );
    return ss.str();
}

uint32_t ston(const std::string & ip)
{
    std::stringstream ss(ip);
	uint32_t ret = 0;
	uint32_t tmp;
	ss >> tmp;
	ret |= tmp << 24;
	ss.ignore(1);

	ss >> tmp;
	ret |= tmp << 16;
	ss.ignore(1);

	ss >> tmp;
	ret |= tmp << 8;
	ss.ignore(1);

	ss >> tmp;
	ret |= tmp;
	return ret;
}

}

IPSeeker::IPSeeker()
{
    if(!loadData(std::string("QQWry.Dat")))
        std::cout << "init error";
}

bool IPSeeker::loadData(const std::string & path)
{
    std::ifstream ifs(path.data(), std::ios::in|std::ios::binary);
    ifs.seekg(0, std::ios::end);
    uint32_t size = (uint32_t)ifs.tellg();
    ifs.seekg(0, std::ios::beg);
    if(size < 8)
        return false;
    data_.resize(size);
    ifs.read(&data_[0], data_.size());
    index_begin_ = getUint32(0);
    index_end_ = getUint32(4);
    if(data_.size() < index_end_ + 7)
        return false;
    return true;
}
	
void IPSeeker::seek(uint32_t ip, std::string & country, std::string & area)
{
	uint32_t index = find(ip);
	getRecord(index, country, area);
}

void IPSeeker::seek(const std::string & ip, std::string & country, std::string & area)
{
	return seek(ston(ip), country, area);
}
uint8_t IPSeeker::getUint8(uint32_t pos)
{
	return getValue<uint8_t>(&data_[pos]);
}

uint32_t IPSeeker::getUint24(uint32_t pos)
{
    return getValue<uint16_t>(&data_[pos]) | (uint32_t(getValue<uint8_t>(&data_[pos + 2])) << 16) ;
}

uint32_t IPSeeker::getUint32(uint32_t pos)
{
    return getValue<uint32_t>(&data_[pos]);
}

std::string IPSeeker::getString(uint32_t pos)
{
	return std::string(&data_[pos]);
}

uint32_t IPSeeker::find(uint32_t ip)
{
    uint32_t l = index_begin_;
    uint32_t r = index_end_;
    uint32_t m;

    while(true)
    {
        m = l + (r-l)/7/2*7;
        if(r - l <= 7)
        {
            if(ip < getUint32(r))
                return l;
            else
                return r;
        }
        if(ip < getUint32(m))
            r = m;
        else
            l = m;
    }
    return l;
}

void IPSeeker::getRecord(uint32_t index, std::string & country, std::string & area)
{
    uint32_t offset = getUint24(index + 4);
	offset += 4;
		
	while (true)
	{
		uint8_t flag = getUint8(offset);
		if (flag == 0x01)
		{
			//std::cout << "1->";
			offset = getUint24(offset + 1);
		}
		else if (flag == 0x02)
		{
			//std::cout << "2->";
			area = readArea(offset + 1 + 3);
			offset = getUint24(offset + 1);
			country = getString(offset);
			break;
		}
		else
		{
			//std::cout << "3->";
			country = getString(offset);
			area = readArea(offset + country.size() + 1);
			break;
		}
	}
}

std::string IPSeeker::readArea(uint32_t offset)
{
	if (offset == 0)
		return std::string("");
	uint8_t flag = getUint8(offset);
	if (flag == 0x01 || flag == 0x02)
	{
		offset = getUint24(offset + 1);
	}
	if (offset)
		return getString(offset);
	return std::string("");
}

void IPSeeker::test()
{
	std::string c, r;
	std::ofstream ofs("test.txt");
	for (uint32_t i = index_begin_; i <= index_end_; i += 7)
	{
		ofs << ntos(getUint32(i)) << "\t" << ntos(getUint32(getUint24(i + 4))) << std::endl;
		std::cout << ntos(getUint32(i)) << "\t" << ntos(getUint32(getUint24(i + 4))) << std::endl;
		getRecord(i, c, r);
		ofs << c << "," << r << std::endl;
		std::cout << c << "," << r << std::endl;
	}
}


