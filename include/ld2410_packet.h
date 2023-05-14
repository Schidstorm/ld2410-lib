#pragma once

#include <sstream>
#include <map>
#include <tuple>

#include "ld2410_reader.h"
#include "ld2410_packet_definition.h"


namespace ld2410 {
class Packet;

class Packet {
private:
    const PacketDefinition* m_definition;
    
    std::vector<uint8_t> m_data_buffer;
    std::vector<uint16_t> m_offsets;
    std::vector<PacketField> m_fields;

public:
    Packet(const PacketDefinition* definition, const std::vector<uint8_t> &data_buffer): m_definition(definition), m_data_buffer(data_buffer), m_offsets({}) {
        if (definition != nullptr) {
            uint16_t offset = 0;

            for (auto &def_name: m_definition->get_names()) {
                auto index = m_definition->find_field_definition(def_name);
                if (!index.has_value()) continue;
                auto packetFields = m_definition->get_field_definition(*index)->generate_fields([&](FieldName fieldName){
                    return read_first_field(fieldName).value_or(0);
                });
                for (auto &packetField: packetFields) {
                    m_offsets.push_back(offset);
                    m_fields.push_back(packetField);
                    offset += packetField.size();
                }
            }

            m_offsets.push_back(offset);
        }
    }

    explicit Packet(const PacketName packetName): m_definition(find_packet_definition(packetName)) {
        if (m_definition == nullptr) return;

        uint16_t offset = 0;
        for (auto &def_name: m_definition->get_names()) {
            auto index = m_definition->find_field_definition(def_name);
            if (!index.has_value()) continue;
            auto packetFields = m_definition->get_field_definition(*index)->generate_fields([&](FieldName){
                return 0;
            });
            for (auto &packetField: packetFields) {
                m_offsets.push_back(offset);
                m_fields.push_back(packetField);
                for(size_t i = 0; i < packetField.size(); i++) {
                    m_data_buffer.push_back(0);
                }
                offset += packetField.size();
            }
        }

        m_offsets.push_back(offset);
    }

private:
    const PacketDefinition* find_packet_definition(PacketName name) const {
        for(size_t i = 0; i < definiedPackets.size(); i++) {
            if (definiedPackets[i].name() == name) {
                return &definiedPackets[i];
            }
        }

        return nullptr;
    }
public:

    Packet(): Packet(nullptr, {}) {

    }

    PacketName name() const {
        if (m_definition == nullptr) return PacketName::empty;
        return m_definition->name();
    }

    uint32_t header() const {
        if (m_definition == nullptr) return 0;
        return m_definition->header();
    }

    uint32_t mfr() const {
        if (m_definition == nullptr) return 0;
        return m_definition->mfr();
    }

    PacketName packet_name() const {
        if (m_definition == nullptr) return PacketName::empty;
        return m_definition->name();
    }

    uint16_t type() const {
        if (m_definition == nullptr) return 0;
        return m_definition->type();
    }

    const std::vector<uint8_t> &data_buffer() const {
        return m_data_buffer;
    }

    bool write(FieldName name, uint32_t val) {
        auto index = get_nth_field_index(name, 0);
        if (!index.has_value()) return false;

        return write_by_raw_index(*index, val);
    }

    std::optional<uint32_t> read_first_field(FieldName name) const {
        return read_nth_field(name, 0);
    }

    std::optional<uint32_t> read_nth_field(FieldName name, uint16_t index) const {
        auto field_index = get_nth_field_index(name, index);
        if (!field_index.has_value()) return std::nullopt;
        return read_by_raw_index(*field_index);
    }

    template <typename AsT = uint32_t>
    std::vector<AsT> read_all_fields(FieldName name) const {
        std::vector<AsT> result{};
        for(size_t i = 0; i < m_fields.size(); ++i) {
            if (m_fields[i].name() == name) {
                result.push_back((AsT)*read_by_raw_index(i));
            }
        }
        return result;
    }

    std::optional<size_t> get_nth_field_index(FieldName name, uint16_t index) const {
        uint16_t counter = 0;
        for(size_t i = 0; i < m_fields.size(); ++i) {
            if (m_fields[i].name() == name) {
                if (counter == index) {
                    return i;
                } else{
                    ++counter;
                }
            }
        }

        return std::nullopt;
    }

    std::optional<uint32_t> read_by_raw_index(size_t index) const {
        
        auto offset = m_offsets[index];
        auto size = (uint16_t)(m_offsets[index + 1] - m_offsets[index]);

        if (offset + size > (uint16_t)m_data_buffer.size()) return std::nullopt;

        uint32_t result = 0;
        for(uint16_t i = 0; i < size; i++) {
            result |= m_data_buffer[offset + i] << (8 * i);
        }
        
        return result;
    }

    bool write_by_raw_index(size_t index,  uint32_t val, size_t size = 0) {
        auto offset = m_offsets[index];
        
        if (size == 0)
            size = (uint16_t)(m_offsets[index + 1] - m_offsets[index]);

        if (offset + size > (uint16_t)m_data_buffer.size()) return false;
        uint8_t* val_buffer = (uint8_t*)((void*)&val);
        for(size_t i = 0; i < size; i++) {
            m_data_buffer[offset + i] = val_buffer[i];
        }

        return true;
    }

    std::string to_string() const {
        if (m_definition == nullptr) {
            return "nullptr";
        }
        
        std::stringstream ss{};

        ss << m_definition->name() << " (header= " << n2hexstr(m_definition->header()) << ", data size=" << m_offsets[m_offsets.size()-1] << ", data type="<< n2hexstr(m_definition->type()) <<")\n";
        for(uint16_t i = 0; i < m_fields.size(); i++) {
            ss << "    " << m_fields[i].name() << ": ";
            ss << *read_by_raw_index(i);
            ss << "\n";
        }

        return ss.str();
    }

    std::string data_to_hexstring() const {
        std::stringstream ss{};
        ss << n2hexstr(m_definition->type()) << b2hexstr(m_data_buffer.data(), m_data_buffer.size());
        return ss.str();
    }

private:
    template <typename I> 
    std::string n2hexstr(I w) const {
        return b2hexstr(&w, sizeof(I));
    }

    template <typename T>
    std::string b2hexstr(const T* beginPointer, size_t byteSize) const {
        auto beginPointerBytes = (uint8_t*)((void*)beginPointer);
        static const char* digits = "0123456789ABCDEF";
        std::string rc(byteSize*2,'0');
        for (size_t i=0; i < byteSize; i++){
            rc[i*2] = digits[beginPointerBytes[i] >> 4];
            rc[i*2+1] = digits[beginPointerBytes[i] & 0x0F];
        }
        return rc;
    }

};

}