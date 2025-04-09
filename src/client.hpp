//
// Created by ender on 25-4-10.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <oatpp-curl/RequestExecutor.hpp>

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/client/ApiClient.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "dto/api_request_dto.hpp"
#include <map>

namespace imouse {
    class imouse_client {
#include OATPP_CODEGEN_BEGIN(ApiClient)

        class imouse_api_client final : public oatpp::web::client::ApiClient {
            API_CLIENT_INIT(imouse_api_client)
            API_CALL("GET", "post", list_groups,
                     BODY_DTO(Object<dto::list_groups_request_dto>, body))
        };

#include OATPP_CODEGEN_END(ApiClient)

    public:
        explicit imouse_client(const std::string &base_url);

        /**
         * list all groups
         * @return gid to group_name std::map
         */
        auto list_groups() const -> std::map<std::string, std::string>;

    private:
        std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> obj_mapper;

        std::shared_ptr<oatpp::web::client::RequestExecutor> executor;

        std::shared_ptr<imouse_api_client> api_client;

        static std::shared_ptr<oatpp::web::client::RequestExecutor> create_curl_executor(const std::string &base_url);
    };
}


#endif //CLIENT_HPP
