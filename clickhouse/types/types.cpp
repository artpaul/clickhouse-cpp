#include "types.h"

#include <assert.h>

namespace clickhouse {

Type::Type(const Code code)
    : code_(code)
{
    if (code_ == Array) {
        array_ = new ArrayImpl;
    } else if (code_ == DateTime64) {
        date_time_64_ = new DateTime64Impl;
    } else if (code_ == Tuple) {
        tuple_ = new TupleImpl;
    } else if (code_ == Nullable) {
        nullable_ = new NullableImpl;
    } else if (code_ == Enum8 || code_ == Enum16) {
        enum_ = new EnumImpl;
    } else if (code_== Decimal || code_== Decimal32 || code_ == Decimal64 || code_ == Decimal128) {
        decimal_ = new DecimalImpl;
    }
}

Type::~Type() {
    if (code_ == Array) {
        delete array_;
    } else if (code_ == Tuple) {
        delete tuple_;
    } else if (code_ == Nullable) {
        delete nullable_;
    } else if (code_ == Enum8 || code_ == Enum16) {
        delete enum_;
    } else if (code_== Decimal || code_== Decimal32 || code_ == Decimal64 || code_ == Decimal128) {
        delete decimal_;
    }
}

Type::Code Type::GetCode() const {
    return code_;
}

TypeRef Type::GetItemType() const {
    if (code_ == Array) {
        return array_->item_type;
    }
    return TypeRef();
}

TypeRef Type::GetNestedType() const {
    if (code_ == Nullable) {
        return nullable_->nested_type;
    }
    return TypeRef();
}

std::vector<TypeRef> Type::GetTupleType() const {
    if (code_ == Tuple) {
        return tuple_->item_types;
    }
    return std::vector<TypeRef>();
}

std::string Type::GetName() const {
    switch (code_) {
        case Void:
            return "Void";
        case Int8:
            return "Int8";
        case Int16:
            return "Int16";
        case Int32:
            return "Int32";
        case Int64:
            return "Int64";
        case Int128:
            return "Int128";
        case UInt8:
            return "UInt8";
        case UInt16:
            return "UInt16";
        case UInt32:
            return "UInt32";
        case UInt64:
            return "UInt64";
        case UUID:
            return "UUID";
        case Float32:
            return "Float32";
        case Float64:
            return "Float64";
        case String:
            return "String";
        case FixedString:
            return "FixedString(" + std::to_string(string_size_) + ")";
        case IPv4:
            return "IPv4";
        case IPv6:
            return "IPv6";
        case DateTime:
            return "DateTime";
        case DateTime64:
            return "DateTime64(" + std::to_string(date_time_64_->precision) + ")";
        case Date:
            return "Date";
        case Array:
            return std::string("Array(") + array_->item_type->GetName() +")";
        case Nullable:
            return std::string("Nullable(") + nullable_->nested_type->GetName() + ")";
        case Tuple: {
            std::string result("Tuple(");
            for (size_t i = 0; i < tuple_->item_types.size(); ++i) {
                result += tuple_->item_types[i]->GetName();

                if (i + 1 != tuple_->item_types.size()) {
                    result += ", ";
                }
            }
            result += ")";
            return result;
        }
        case Enum8:
        case Enum16: {
            std::string result;
            if (code_ == Enum8) {
                result = "Enum8(";
            } else {
                result = "Enum16(";
            }
            for (auto ei = enum_->value_to_name.begin(); ; ) {
                result += "'";
                result += ei->second;
                result += "' = ";
                result += std::to_string(ei->first);

                if (++ei != enum_->value_to_name.end()) {
                    result += ", ";
                } else {
                    break;
                }
            }
            result += ")";
            return result;
        }
        case Decimal:
            return "Decimal(" + std::to_string(decimal_->precision) + "," + std::to_string(decimal_->scale) + ")";
        case Decimal32:
            return "Decimal32(" + std::to_string(decimal_->scale) + ")";
        case Decimal64:
            return "Decimal64(" + std::to_string(decimal_->scale) + ")";
        case Decimal128:
            return "Decimal128(" + std::to_string(decimal_->scale) + ")";
    }

    return std::string();
}

bool Type::IsEqual(const TypeRef& other) const {
    return this->GetName() == other->GetName();
}

TypeRef Type::CreateArray(TypeRef item_type) {
    TypeRef type(new Type(Type::Array));
    type->array_->item_type = item_type;
    return type;
}

TypeRef Type::CreateDate() {
    return TypeRef(new Type(Type::Date));
}

TypeRef Type::CreateDateTime() {
    return TypeRef(new Type(Type::DateTime));
}

TypeRef Type::CreateDateTime64(size_t precision) {
    TypeRef type(new Type(Type::DateTime64));
    type->date_time_64_->precision = precision;
    return type;
}

TypeRef Type::CreateDecimal(size_t precision, size_t scale) {
    TypeRef type(new Type(Type::Decimal));
    type->decimal_->precision = precision;
    type->decimal_->scale = scale;
    return type;
}

TypeRef Type::CreateIPv4() {
    return TypeRef(new Type(Type::IPv4));
}

TypeRef Type::CreateIPv6() {
    return TypeRef(new Type(Type::IPv6));
}

TypeRef Type::CreateNothing() {
    return TypeRef(new Type(Type::Void));
}

TypeRef Type::CreateNullable(TypeRef nested_type) {
    TypeRef type(new Type(Type::Nullable));
    type->nullable_->nested_type = nested_type;
    return type;
}

TypeRef Type::CreateString() {
    return TypeRef(new Type(Type::String));
}

TypeRef Type::CreateString(size_t n) {
    TypeRef type(new Type(Type::FixedString));
    type->string_size_ = n;
    return type;
}

TypeRef Type::CreateTuple(const std::vector<TypeRef>& item_types) {
    TypeRef type(new Type(Type::Tuple));
    type->tuple_->item_types.assign(item_types.begin(), item_types.end());
    return type;
}

TypeRef Type::CreateEnum8(const std::vector<EnumItem>& enum_items) {
    TypeRef type(new Type(Type::Enum8));
    for (const auto& item : enum_items) {
        type->enum_->value_to_name[item.value] = item.name;
        type->enum_->name_to_value[item.name] = item.value;
    }
    return type;
}

TypeRef Type::CreateEnum16(const std::vector<EnumItem>& enum_items) {
    TypeRef type(new Type(Type::Enum16));
    for (const auto& item : enum_items) {
        type->enum_->value_to_name[item.value] = item.name;
        type->enum_->name_to_value[item.name] = item.value;
    }
    return type;
}

TypeRef Type::CreateUUID() {
    return TypeRef(new Type(Type::UUID));
}


EnumType::EnumType(const TypeRef& type)
    : type_(type)
{
    assert(type_->GetCode() == Type::Enum8 ||
           type_->GetCode() == Type::Enum16);
}

const std::string& EnumType::GetEnumName(int16_t value) const {
    return type_->enum_->value_to_name[value];
}

int16_t EnumType::GetEnumValue(const std::string& name) const {
    return type_->enum_->name_to_value[name];
}

bool EnumType::HasEnumName(const std::string& name) const {
    return type_->enum_->name_to_value.find(name) != type_->enum_->name_to_value.end();
}

bool EnumType::HasEnumValue(int16_t value) const {
    return type_->enum_->value_to_name.find(value) != type_->enum_->value_to_name.end();
}

EnumType::ValueToNameIterator EnumType::BeginValueToName() const {
    return type_->enum_->value_to_name.begin();
}

EnumType::ValueToNameIterator EnumType::EndValueToName() const {
    return type_->enum_->value_to_name.end();
}

}
