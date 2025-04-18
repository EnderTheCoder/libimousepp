//
// Created by ender on 25-4-10.
//

#ifndef API_REQUEST_DTO_HPP
#define API_REQUEST_DTO_HPP

#include "start_streaming_inner_dto.hpp"
#include "stop_streaming_inner_dto.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include OATPP_CODEGEN_BEGIN(DTO)

namespace imouse::dto {
    template<typename DATA_T>
    class api_request_dto final : public oatpp::DTO {
        DTO_INIT(api_request_dto, DTO)

        DTO_FIELD(DATA_T, data) = {};

        DTO_FIELD(oatpp::String, fun);

        DTO_FIELD(oatpp::Int32, msgid) = 0;
    };

    typedef api_request_dto<oatpp::Object<oatpp::DTO> > non_data_request_dto;
    typedef non_data_request_dto list_groups_request_dto;
    typedef api_request_dto<oatpp::Object<request::start_streaming_inner_dto> > start_streaming_request;
    typedef api_request_dto<oatpp::Object<request::stop_streaming_inner_dto> > stop_streaming_request;
}

#include OATPP_CODEGEN_END(DTO)

#endif //API_REQUEST_DTO_HPP
