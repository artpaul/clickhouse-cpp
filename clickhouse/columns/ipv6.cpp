//
// Created by hj8088 on 19-7-21.
//

#include <arpa/inet.h>
#include "ipv6.h"
#include "utils.h"

#include "../base/wire_format.h"

namespace clickhouse {

    ColumnIPv6::ColumnIPv6()
            : Column(Type::CreateIPv6())
            , data_(std::make_shared<ColumnUInt64>())
    {
    }

    ColumnIPv6::ColumnIPv6(ColumnRef data)
            : Column(Type::CreateIPv6())
            , data_(data->As<ColumnUInt64>())
    {
        if (data_->Size() != 0) {
            throw std::runtime_error("number of entries must be even (two 64-bit numbers for each IPv6)");
        }
    }

    void ColumnIPv6::Append(const std::string& ip) {
        char addrBuffer[16];
        if(1 != inet_pton(AF_INET6, ip.c_str(), addrBuffer)){
            throw std::runtime_error("(Invalid IPv6 format, ip:" + ip + ")");
        }

        Append(addrBuffer, 16);
    }

    void ColumnIPv6::Append(const char* addr, size_t len){
        if(16 != len){
            throw std::runtime_error("(Invalid IPv6 format, addr len:" + std::to_string(len) + ")");
        }

        data_->Append(*(uint64_t*)addr);
        data_->Append(*(uint64_t*)(addr+8));
    }

    void ColumnIPv6::Clear() {
        data_->Clear();
    }

    std::string ColumnIPv6::to_string (size_t n) const{
        uint64_t addr[2] = {data_->At(n * 2), data_->At(n * 2 + 1)};
        char strBuffer[INET6_ADDRSTRLEN];
        const char* ip_str = inet_ntop(AF_INET6, addr, strBuffer, INET6_ADDRSTRLEN);
        if(ip_str == nullptr){
            throw std::runtime_error("(Invalid IPv6 format, data[0]:" + std::to_string(addr[0]) + ", data[1]:" + std::to_string(addr[1]) + " )");
        }
        return ip_str;
    }

    UInt128 ColumnIPv6::At(size_t n) const {
        return UInt128(data_->At(n * 2), data_->At(n * 2 + 1));
    }

    UInt128 ColumnIPv6::operator [] (size_t n) const {
        return UInt128((*data_)[n * 2], (*data_)[n * 2 + 1]);
    }

    void ColumnIPv6::Append(ColumnRef column) {
        if (auto col = column->As<ColumnIPv6>()) {
            data_->Append(col->data_);
        }
    }

    bool ColumnIPv6::Load(CodedInputStream* input, size_t rows) {
        return data_->Load(input, rows * 2);
    }

    void ColumnIPv6::Save(CodedOutputStream* output) {
        data_->Save(output);
    }

    size_t ColumnIPv6::Size() const {
        return data_->Size() / 2;
    }

    ColumnRef ColumnIPv6::Slice(size_t begin, size_t len) {
        return std::make_shared<ColumnIPv6>(data_->Slice(begin * 2, len * 2));
    }
}
