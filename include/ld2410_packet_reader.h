#pragma once

#include "ld2410_packets.h"

namespace ld2410 {
    namespace internal_helpers {
        struct read_from_reader_tmp_ids {
            bool removed;
            to_bytes_union<uint32_t> header;
            to_bytes_union<uint16_t> type;
        };
        
        template<typename T>
        constexpr std::vector<read_from_reader_tmp_ids> build_ids() {
            return { { false, T::header, T::type } };
        }

        template<typename T, typename T2, typename ...TArgs>
        constexpr std::vector<read_from_reader_tmp_ids> build_ids() {
            std::vector<read_from_reader_tmp_ids> v = build_ids<T>();
            std::vector<read_from_reader_tmp_ids> vargs = build_ids<T2, TArgs...>();
            v.insert(v.end(), vargs.begin(), vargs.end());
            return v;
        }

        template<std::size_t N>
        struct num { static const constexpr auto value = N; };

        template <class F, std::size_t... Is>
        void for_(F func, std::index_sequence<Is...>)
        {
            using expander = int[];
            (void)expander{0, ((void)func(num<Is>{}), 0)...};
        }

        template <std::size_t N, typename F>
        void for_(F func)
        {
            for_(func, std::make_index_sequence<N>());
        }

        template <std::size_t I, typename T, typename ...Ts>
        struct nth_element_impl {
            using type = typename nth_element_impl<I-1, Ts...>::type;
        };

        template <typename T, typename ...Ts>
        struct nth_element_impl<0, T, Ts...> {
            using type = T;
        };

        template <std::size_t I, typename ...Ts>
        using nth_element = typename nth_element_impl<I, Ts...>::type;
    }

    const size_t max_size_t = ~((size_t)0);
    template <typename ...T>
    std::optional<std::variant<T...>> read_from_reader_many(const ld2410::reader_t &reader) {
        using namespace internal_helpers;
        std::vector<read_from_reader_tmp_ids> ids = build_ids<T...>();
        
        for(size_t i = 0; i < sizeof(uint32_t); i++) {
            bool found = false;
            auto b = reader();
            for(size_t k = 0; k < ids.size(); ++k) {
                if (ids[k].removed) {
                    continue;
                }
                if (b == ids[k].header.val.u8[i]) {
                    found = true;
                } else {
                    ids[k].removed = true;
                }
            }
            if (!found) return std::nullopt;
        }

        ld2410::readUint16(reader); // just ignore data size

        for(size_t i = 0; i < sizeof(uint16_t); i++) {
            bool found = false;
            auto b = reader();
            for(size_t k = 0; k < ids.size(); ++k) {
                if (ids[k].removed) {
                    continue;
                }
                if (b == ids[k].type.val.u8[i]) {
                    found = true;
                } else {
                    ids[k].removed = true;
                }
            }
            if (!found) return std::nullopt;
        }

        std::optional<std::variant<T...>> result = std::nullopt;

        for_([&](auto i){
            if (!ids[i.value].removed) {
                auto v = nth_element<i.value, T...>{};
                v.read(reader);

                result = {v};
            }
        }, std::make_index_sequence<sizeof...(T)>());

        return result;
    }

    template <typename T>
    std::optional<T> read_from_reader(const ld2410::reader_t &reader) {
        std::optional<std::variant<T>> res = read_from_reader_many<T>(reader);
        if (!res.has_value()) return std::nullopt;
        return std::get<T>(*res);
    }

    template <typename T1, typename T2, typename ...T>
    std::optional<std::variant<T1, T2, T...>> read_from_reader(const ld2410::reader_t &reader) {
        return read_from_reader_many<T1, T2, T...>(reader);
    }

    
}