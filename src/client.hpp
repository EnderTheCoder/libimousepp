//
// Created by ender on 25-4-10.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <map>

#include <oatpp-curl/RequestExecutor.hpp>

#include "oatpp/web/client/HttpRequestExecutor.hpp"
#include "oatpp/network/tcp/client/ConnectionProvider.hpp"

#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/client/ApiClient.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "dto/api_request_dto.hpp"
#include "dto/api_response_dto.hpp"
#include "dto/device_dto.hpp"



namespace imouse {

    class

    class imouse_client {
#include OATPP_CODEGEN_BEGIN(ApiClient)

        class imouse_api_client final : public oatpp::web::client::ApiClient {
            API_CLIENT_INIT(imouse_api_client)
            API_CALL("GET", "post", list_groups,
                     BODY_DTO(Object<dto::list_groups_request_dto>, body))
            API_CALL("GET", "post", list_devices,
                     BODY_DTO(Object<dto::non_data_request_dto>, body))
        };

#include OATPP_CODEGEN_END(ApiClient)

    public:
        explicit imouse_client(const std::string &base_url);

        /**
         * list all groups
         * @return gid to group_name std::map
         */
        [[nodiscard]] auto list_groups() const -> std::map<std::string, std::string>;

        auto list_devices() const -> std::map<std::string, oatpp::Object<dto::device_dto>>;

    private:

        template<typename T>
        auto assert_res_dto_status(const oatpp::Object<dto::api_response_dto<T>>& dto) const -> void {
            if (dto->status != 0) throw std::runtime_error(std::format("invalid api response status: {}, message: {}", *dto->status, *dto->message));
        }

        std::shared_ptr<oatpp::parser::json::mapping::ObjectMapper> obj_mapper;

        std::shared_ptr<oatpp::web::client::RequestExecutor> executor;

        std::shared_ptr<imouse_api_client> api_client;

        static std::shared_ptr<oatpp::web::client::RequestExecutor> create_curl_executor(const std::string &base_url);
    };
}


#endif //CLIENT_HPP
