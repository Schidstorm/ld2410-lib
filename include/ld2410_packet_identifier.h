#pragma once

#include <vector>
#include <tuple>
#include <variant>
#include <optional>

#include "ld2410_packet_definition.h"
#include "ld2410_reader.h"

namespace ld2410 {

class PacketDefinitionNode;
class PacketDefinitionNode {    
private:
    using next_data_t = std::variant<PacketDefinitionNode*, const PacketDefinition*>;
    using next_t = std::tuple<uint8_t, next_data_t>;
    std::vector<next_t> next;

    std::vector<uint8_t> definition_id_as_bytes(const PacketDefinition* def) {
        auto header = def->header();
        uint8_t* header_pointer = (uint8_t*)((void*)(&header));
        auto type = def->type();
        uint8_t* type_pointer = (uint8_t*)((void*)&type);

        return {
            header_pointer[3],header_pointer[2],header_pointer[1],header_pointer[0],
            type_pointer[0], type_pointer[1]
        };
    }

public:
    void add(const PacketDefinition* definition) {
        auto id_bytes = definition_id_as_bytes(definition);
        add(definition, id_bytes.data(), id_bytes.data()+id_bytes.size());
    }

private:
    void add(const PacketDefinition* definition, const uint8_t* begin, const uint8_t* end) {
        auto size = end - begin;

        if (size == 1) {
            next.push_back({begin[0], definition});
        }
        else {
            for(size_t i = 0; i < next.size(); i++) {
                if (std::get<uint8_t>(next[i]) == begin[0]) {
                    std::get<PacketDefinitionNode*>(std::get<next_data_t>(next[i]))->add(definition, begin+1, end);
                    return;
                }
            }

            auto nextNode = new PacketDefinitionNode();
            nextNode->add(definition, begin+1, end);
            next.push_back({begin[0], nextNode});
        }
    }

public:
    using data_size_t = uint16_t;

    std::optional<std::tuple<data_size_t, const PacketDefinition*>> read(reader_t &reader, size_t depth = 0, data_size_t data_size = 0) {
        const size_t packetHeaderByteSize = 4;
        if (depth == packetHeaderByteSize) {
            // the next 2 bytes are the data size field
            data_size = readUint16(reader);
        }
        auto red = reader();

        for(size_t i = 0; i < next.size(); i++) {
            auto nextByte = std::get<uint8_t>(next[i]);
            if (nextByte == red) {
                auto nextData = std::get<next_data_t>(next[i]);
                if (std::holds_alternative<PacketDefinitionNode*>(nextData)) {
                    return std::get<PacketDefinitionNode*>(nextData)->read(reader, depth+1, data_size);
                } else {
                    return std::tuple<data_size_t, const PacketDefinition*>(data_size, std::get<const PacketDefinition*>(nextData));
                }
            }
        }

        return std::nullopt;
    }

private:
    
};

class PacketDefinitionTree {
private:
    PacketDefinitionNode* m_root;

public:
    constexpr PacketDefinitionTree(): m_root(nullptr) {

    }

    constexpr decltype(m_root) root() {
        return m_root;
    }

    static PacketDefinitionTree build(const std::vector<PacketDefinition> &definitions = definiedPackets) {
        auto tree = PacketDefinitionTree();
        tree.m_root = new PacketDefinitionNode();
        for (size_t i = 0; i < definitions.size(); i++) {
            auto ptr = &definitions[i];
            tree.m_root->add(ptr);
        }

        return tree;
    }
};

struct IdentifiedPacket {
    uint16_t data_size;
    const PacketDefinition* definition;
};

class PacketIdentifier {
private:
    PacketDefinitionTree m_tree;

public:
    PacketIdentifier(PacketDefinitionTree tree): m_tree(tree) {
        
    }

    PacketIdentifier(): PacketIdentifier(PacketDefinitionTree::build()) {

    }

    std::optional<const IdentifiedPacket> read(reader_t reader) {
        auto result = m_tree.root()->read(reader);

        if (!result.has_value()) return std::nullopt;
        return IdentifiedPacket{
            std::get<PacketDefinitionNode::data_size_t>(*result),
            std::get<const PacketDefinition*>(*result),
        };
    }
};

}