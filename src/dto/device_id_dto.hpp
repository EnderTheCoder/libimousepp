//
// Created by ender on 25-4-10.
//

#ifndef DEVICE_ID_DTO_HPP
#define DEVICE_ID_DTO_HPP

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include OATPP_CODEGEN_BEGIN(DTO)

namespace imouse::dto {
    class device_id_dto : public oatpp::DTO {
        DTO_INIT(device_id_dto, oatpp::DTO)

        DTO_FIELD(String, deviceid);
    };
}

#include OATPP_CODEGEN_END(DTO)


#endif //DEVICE_ID_DTO_HPP
