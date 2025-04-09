//
// Created by ender on 25-4-10.
//

#ifndef GROUP_DTO_HPP
#define GROUP_DTO_HPP

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"
#include OATPP_CODEGEN_BEGIN(DTO)

namespace imouse::dto {
    class group_dto final : public oatpp::DTO {
        DTO_INIT(group_dto, oatpp::DTO)

        DTO_FIELD(oatpp::String, gid);

        DTO_FIELD(oatpp::String, name);
    };
}

#include OATPP_CODEGEN_END(DTO)

#endif //GROUP_DTO_HPP
