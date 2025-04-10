//
// Created by ender on 25-4-10.
//

#include "client.hpp"
#include "dto/api_request_dto.hpp"
#include "dto/api_response_dto.hpp"

namespace imouse {
    imouse_client::imouse_client(const std::string &base_url) {
        this->obj_mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
        this->executor = create_curl_executor(base_url);
        this->api_client = imouse_api_client::createShared(this->executor, obj_mapper);
    }

    auto imouse_client::list_groups() const -> std::map<std::string, std::string> {
        std::map<std::string, std::string> devices;

        const auto req_dto = dto::list_groups_request_dto::createShared();
        req_dto->fun = "get_group_list";
        req_dto->msgid = 0;
        const auto res = this->api_client->list_groups(req_dto);
        const auto res_dto = res->readBodyToDto<oatpp::Object<dto::list_groups_response_dto> >(this->obj_mapper);
        this->assert_res_dto_status(res_dto);
        for ( const auto &[gid,group_dto]: *res_dto->data) {
            devices.emplace(gid, group_dto->name);
        }
        return devices;
    }

    auto imouse_client::list_devices() const -> std::map<std::string, oatpp::Object<dto::device_dto>> {
        std::map<std::string, oatpp::Object<dto::device_dto>> devices = {};
        const auto req_dto = dto::non_data_request_dto::createShared();
        req_dto->fun = "get_device_list";
        req_dto->msgid = 0;
        const auto res = this->api_client->list_devices(req_dto);
        const auto res_dto = res->readBodyToDto<oatpp::Object<dto::list_devices_response_dto>>(this->obj_mapper);
        this->assert_res_dto_status(res_dto);
        for (const auto &[device_id, device_dto]: *res_dto->data) {
            devices.emplace(device_id, device_dto);
        }
        return devices;
    }

    std::shared_ptr<oatpp::web::client::RequestExecutor> imouse_client::create_curl_executor(
        const std::string &base_url) {
        OATPP_LOGD("App", "Using oatpp-curl RequestExecutor.");
        return oatpp::curl::RequestExecutor::createShared(base_url, false);
    }
}
