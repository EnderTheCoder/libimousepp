//
// Created by ender on 25-4-10.
//

#ifndef STOP_STREAMING_INNER_DTO_HPP
#define STOP_STREAMING_INNER_DTO_HPP

#include "device_dto.hpp"
#include "device_id_dto.hpp"
#include OATPP_CODEGEN_BEGIN(DTO)

namespace imouse::dto {
    namespace request {
        class stop_streaming_inner_dto : public device_id_dto {
            DTO_INIT(stop_streaming_inner_dto, device_id_dto)

            DTO_FIELD(Boolean, stop) = true;
        };
    }
}

#include OATPP_CODEGEN_END(DTO)


#endif //STOP_STREAMING_INNER_DTO_HPP
