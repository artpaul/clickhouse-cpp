//
// Created by hj8088 on 19-7-21.
//

#include <arpa/inet.h>
#include "ipv4.h"
#include "utils.h"

#include "../base/wire_format.h"

namespace clickhouse {

    ColumnIPv4::ColumnIPv4()
            : Column(Type::CreateIPv4())
            , data_(std::make_shared<ColumnUInt32>())
    {
    }

    ColumnIPv4::ColumnIPv4(ColumnRef data)
            : Column(Type::CreateIPv4())
            , data_(data->As<ColumnUInt32>())
    {
        if (data_->Size() != 0) {
            throw std::runtime_error("number of entries must be even (32-bit numbers for each IPv4)");
        }
    }

    void ColumnIPv4::Append(const std::string& str) {
        in_addr_t addr = inet_addr(str.c_str());
        if(addr == INADDR_NONE){
            throw std::runtime_error("(Invalid IPv4 format, ip:" + str + ")");
        }
        data_->Append(htonl(addr));
    }

    void ColumnIPv4::Append(uint32_t ip){
        data_->Append(htonl(ip));
    }

    void ColumnIPv4::Clear() {
        data_->Clear();
    }

    uint32_t ColumnIPv4::At(size_t n) const {
        return data_->At(n);
    }

    std::string ColumnIPv4::to_string (size_t n) const{
        struct in_addr inAddr;
        inAddr.s_addr = ntohl(data_->At(n));
        return inet_ntoa(inAddr);
    }

    uint32_t ColumnIPv4::operator [] (size_t n) const {
        return data_->operator[](n);
    }

    void ColumnIPv4::Append(ColumnRef column) {
        if (auto col = column->As<ColumnIPv4>()) {
            data_->Append(column);
        }
    }

    bool ColumnIPv4::Load(CodedInputStream* input, size_t rows) {
        return data_->Load(input, rows);
    }

    void ColumnIPv4::Save(CodedOutputStream* output) {
        data_->Save(output);
    }

    size_t ColumnIPv4::Size() const {
        return data_->Size();
    }

    ColumnRef ColumnIPv4::Slice(size_t begin, size_t len) {
        return std::make_shared<ColumnIPv4>(data_->Slice(begin, len));
    }
}
