#ifndef IPSEEKER_H
#define IPSEEKER_H

#include <stdint.h>
#include <vector>
#include <string>

class IPSeeker
{
public:
    IPSeeker();
	void seek(uint32_t ip, std::string & country, std::string & area);
	void seek(const std::string & ip, std::string & country, std::string & area);
    bool loadData(const std::string & path);
	void test();

private:
	void getRecord(uint32_t index, std::string & country, std::string & area);
    uint32_t find(uint32_t ip);
	std::string readArea(uint32_t offset);
	uint8_t getUint8(uint32_t pos);
	uint32_t getUint24(uint32_t pos);
	uint32_t getUint32(uint32_t pos);
	std::string getString(uint32_t pos);
private:
    std::vector<char> data_;
    uint32_t index_begin_;
    uint32_t index_end_;
};

#endif // IPSEEKER_H
