//
// Created by ender on 25-4-10.
//

#ifndef START_STREAMING_INNER_DTO_HPP
#define START_STREAMING_INNER_DTO_HPP


#include <oatpp/codegen/dto/base_define.hpp>

#include "device_id_dto.hpp"
#include OATPP_CODEGEN_BEGIN(DTO)

namespace imouse::dto {
    namespace request {
        class start_streaming_inner_dto final : public device_id_dto {
            DTO_INIT(start_streaming_inner_dto, device_id_dto)

            DTO_FIELD(Int32, time);

            DTO_FIELD(Boolean, isjpg);
        };
    }
}

#include OATPP_CODEGEN_END(DTO)


#endif //START_STREAMING_INNER_DTO_HPP
