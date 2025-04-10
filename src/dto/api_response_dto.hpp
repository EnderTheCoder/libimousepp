//
// Created by ender on 25-4-10.
//

#ifndef API_RESPONSE_DTO_HPP
#define API_RESPONSE_DTO_HPP


#include "group_dto.hpp"
#include "device_dto.hpp"
#include "device_id_dto.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include OATPP_CODEGEN_BEGIN(DTO)

namespace imouse::dto {
    template<typename DATA_T>
    class api_response_dto : public oatpp::DTO {
        DTO_INIT(api_response_dto, oatpp::DTO)

        DTO_FIELD(oatpp::String, fun);

        DTO_FIELD(oatpp::Int32, msgid);

        DTO_FIELD(oatpp::Int32, status);

        DTO_FIELD(DATA_T, data);

        DTO_FIELD(oatpp::String, message);
    };
    typedef api_response_dto<oatpp::UnorderedFields<oatpp::Object<group_dto> > > list_groups_response_dto;
    typedef api_response_dto<oatpp::UnorderedFields<oatpp::Object<device_dto>>> list_devices_response_dto;
    typedef api_response_dto<oatpp::Object<device_id_dto>> device_id_response_dto;
}

#include OATPP_CODEGEN_END(DTO)

#endif //API_RESPONSE_DTO_HPP
